#include "Job.h"
#include "JobGraph.h"
#include "ThreadPool.h"

#include "ECS.h"
#include "System.h"
#include "GameFramework.h"

#include <array>

int max_threads{ 16 };

int main()
{
	GameFramework& game = GameFramework::Get();

	for (int instance = 0; instance < 100000; ++instance)
	{
		Instance* inst = game._world.CreateInstance();

		for (int entity = 0; entity < 1000; ++entity)
		{
			Entity e = game._ecs.entityMng.Create();
			e.instanceId = inst->GetId();
			inst->AddEntity(e);
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

	std::cout << "Frame Time Avg: " << total / 10.0 << " ms\n";
}