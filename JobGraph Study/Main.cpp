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