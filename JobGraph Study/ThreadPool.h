#pragma once

#include <thread>
#include <atomic>
#include <vector>
#include <shared_mutex>

#include <concurrent_queue.h>

using namespace concurrency;

struct JobData;

class ThreadPool {
public:
	ThreadPool(size_t size = std::thread::hardware_concurrency());
	~ThreadPool() { Stop(); }

	void Push(const JobData& job);
	void Stop();

private:
	void WorkerLoop();

private:
	std::atomic<bool> _running;
	std::vector<std::thread> _workers;
	concurrent_queue<JobData> _jobQueue;

	std::shared_mutex _mtx;
	std::condition_variable _cv;
};

