#define ASIO_STANDALONE
#include <asio.hpp>
#include <array>

#include "Network.h"

#include "Job.h"
#include "JobGraph.h"
#include "ThreadPool.h"

#include "ECS.h"
#include "System.h"
#include "Game.h"

#include "Constants.h"

#include "RingBuffer.hpp"
#include "ClaimStrategy.hpp"
#include "ProducerBarrier.hpp"
#include "ConsumerBarrier.hpp"
#include "Consumer.hpp"

void TestDisruptor()
{
    using namespace disruptor;

    constexpr int64_t bufferSize = 1024 * 64;     // 64K entries
    constexpr int producerCount = 4;
    constexpr int64_t totalMessages = 1'000'000; // 테스트할 메시지 수

    // 대기 전략 선택 (필요 시 BusyWaitStrategy / YieldWaitStrategy / ConditionWaitStrategy 교체 가능)
    BusyWaitStrategy waitStrategy;

    // 기본 disruptor 구성
    RingBuffer ring(bufferSize);
    ProducerBarrier producer(ring, &waitStrategy);

    AtomicCounter& cursor = ring.GetCursor();
    ConsumerBarrier consumerBarrier(ring, &waitStrategy, &cursor);

    Consumer consumer(consumerBarrier);

    // 소비자 스레드 시작
    std::thread consumerThread([&]()
        {
            consumer.ConsumeEntries();
        });

    // -----------------------------
    // 생산자: 벤치마크 측정
    // -----------------------------
    std::vector<std::thread> producers;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < producerCount; ++i)
    {
        producers.emplace_back(
            [&, i]() 
            {
                int64_t startSeq, endSeq;
                for (int64_t i = 0; i < totalMessages; i += 8)
                {
                    endSeq = producer.ClaimBatch(8, startSeq);
                    for (int64_t seq = startSeq; seq <= endSeq; ++seq)
                    {
                        disruptor::Entry& e = ring.GetEntry(seq);
                        e.temp = static_cast<int>(seq);
                    }
                    producer.Publish(endSeq);
                }
            });
    }

    for (auto& t : producers) t.join();

    auto end = std::chrono::high_resolution_clock::now();
    consumerThread.detach();

    // -----------------------------
    // 결과 출력
    // -----------------------------
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); // ms 단위
    double throughput = static_cast<double>(totalMessages) / elapsed; // entries per millisecond

    std::cout << "----------------------------------------\n";
    std::cout << "RingBuffer Benchmark Result\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Processed : " << totalMessages << " entries\n";
    std::cout << "Elapsed   : " << elapsed << " ms\n";
    std::cout << "Throughput: " << throughput << " K entries/ms\n";
    std::cout << "----------------------------------------\n";

    std::this_thread::sleep_for(std::chrono::seconds(100));
}

void TestMutexQueue()
{
    std::queue<int> q;
    std::mutex m;
    std::atomic<bool> start{ false };
    std::atomic<size_t> count{ 0 };

    constexpr size_t NUM_PRODUCERS = 4;
    constexpr size_t NUM_ITEMS_PER_THREAD = 250'000;

    std::vector<std::thread> producers;
    for (size_t i = 0; i < NUM_PRODUCERS; ++i)
    {
        producers.emplace_back([&, i]() {
            while (!start.load());
            for (size_t j = 0; j < NUM_ITEMS_PER_THREAD; ++j)
            {
                std::scoped_lock lock(m);
                q.push(i * NUM_ITEMS_PER_THREAD + j);
            }
            });
    }

    std::thread consumer([&]() {
        while (count < NUM_PRODUCERS * NUM_ITEMS_PER_THREAD)
        {
            std::scoped_lock lock(m);
            if (!q.empty())
            {
                q.pop();
                ++count;
            }
        }
        });

    start.store(true);
    auto t1 = std::chrono::high_resolution_clock::now();

    for (auto& t : producers) t.join();
    consumer.join();

    auto t2 = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "[std::queue + mutex] " << ms << " ms\n";
}

#include <concurrent_queue.h>
using namespace concurrency;

void TestConcurrentQueue()
{
    concurrent_queue<int> q;
    std::atomic<bool> start{ false };
    std::atomic<size_t> count{ 0 };

    constexpr size_t NUM_PRODUCERS = 4;
    constexpr size_t NUM_ITEMS_PER_THREAD = 250'000;

    std::vector<std::thread> producers;
    for (size_t i = 0; i < NUM_PRODUCERS; ++i)
    {
        producers.emplace_back([&, i]() {
            while (!start.load());
            for (size_t j = 0; j < NUM_ITEMS_PER_THREAD; ++j)
                q.push(i * NUM_ITEMS_PER_THREAD + j);
            });
    }

    std::thread consumer([&]() {
        int tmp;
        while (count < NUM_PRODUCERS * NUM_ITEMS_PER_THREAD)
        {
            if (q.try_pop(tmp))
                ++count;
        }
        });

    start.store(true);
    auto t1 = std::chrono::high_resolution_clock::now();

    for (auto& t : producers) t.join();
    consumer.join();

    auto t2 = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "[concurrent_queue] " << ms << " ms\n";
}

int main()
{
    /*TestMutexQueue();
    TestConcurrentQueue();*/
    TestDisruptor();
}