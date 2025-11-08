#include "Job.h"
#include "JobGraph.h"
#include "ThreadPool.h"

#include "ECS.h"
#include "System.h"
#include "Game.h"

#include <array>

size_t max_threads{ 16 };

void benchmark() 
{
	Game game{ std::thread::hardware_concurrency() };

	for (int instance = 0; instance < 1000; ++instance)
	{
		Instance* inst = game._world.CreateInstance();

		for (int entity = 0; entity < 10; ++entity)
		{
			Entity e = game._ecs.entityMng.Create();
			e.instanceId = inst->GetId();
			inst->AddEntity(e);

			auto& ecs = game._ecs;

			ecs.GetStorage<Transform>().AddComponent(e);
			ecs.GetStorage<Velocity>().AddComponent(e);
			ecs.GetStorage<Stat>().AddComponent(e);
			ecs.GetStorage<AIState>().AddComponent(e);
			ecs.GetStorage<View>().AddComponent(e);
			ecs.GetStorage<Input>().AddComponent(e);
		}
	}

	double total{ 0.0 };
	for (int frame = 0; frame < 100; ++frame)
	{
		auto start = std::chrono::steady_clock::now();

		game.Update(0.016f);

		auto end = std::chrono::steady_clock::now();
		total += std::chrono::duration<double, std::milli>(end - start).count();
	}

	std::cout << "Frame Time Avg: " << total / 100.0 << " ms\n";
}

#define ASIO_STANDALONE
#include <asio.hpp>

#include "Network.h"

int main()
{
	try {
		int threadCount = std::thread::hardware_concurrency();
		asio::io_context ioCtx(threadCount);

		Network net(7777, ioCtx);
		net.Start();

		std::vector<std::thread> workers;
		for (int i = 0; i < threadCount; ++i)
		{
			workers.emplace_back([&ioCtx]() {
				try {
					ioCtx.run();
				}
				catch (std::exception& e) {
					std::cerr << "Worker exception: " << e.what() << '\n';
				}
				});
		}

		std::cout << "Asio Echo Server running on port 7777" << std::endl;
		std::cout << "Press Enter to stop..." << std::endl;
		std::cin.get(); // 엔터 입력 시 종료

		ioCtx.stop();
		for (auto& t : workers)
			if (t.joinable()) t.join();

		std::cout << "Server stopped." << std::endl;
	}

	catch (const std::exception& e)
	{
		std::cerr << "Fatal error: " << e.what() << std::endl;
	}
}