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

		for (int entity = 0; entity < 600; ++entity)
		{
			Entity e = game._ecs.entityMng.Create();
			e.instanceId = inst->GetId();

			auto* t = game._ecs.GetStorage<Transform>().AddComponent(e);
			auto* v = game._ecs.GetStorage<Velocity>().AddComponent(e);

			t->x = float(instance * 100 + entity * 10);
			t->y = 0.0f;
			v->dx = 1.0f + entity * 0.1f;
			v->dy = 0.0f;

			inst->AddEntity(e);
		}
	}

	for (int frame = 0; frame < 10; ++frame)
	{
		std::cout << "=== Frame " << frame << " ===\n";

		auto start = std::chrono::steady_clock::now();

		game.Update(0.016f);

		auto end = std::chrono::steady_clock::now();
		double frameTime = std::chrono::duration<double, std::milli>(end - start).count();
		std::cout << "Frame Time: " << frameTime << " ms\n";
	}
}