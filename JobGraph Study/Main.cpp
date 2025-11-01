#include "Job.h"
#include "JobGraph.h"
#include "ThreadPool.h"

#include "ECS.h"
#include "System.h"

#include <array>

const int playerCount = 20'000;

int main()
{
	ECS ecs;
	ecs.systemMng.RegisterSystem<MovementSystem>(ecs);

	Entity e1 = ecs.entityMng.Create();
	Entity e2 = ecs.entityMng.Create();

	auto* t1 = ecs.GetStorage<Transform>().AddComponent(e1);
	auto* v1 = ecs.GetStorage<Velocity>().AddComponent(e1);
	v1->dx = 1.0f; v1->dy = 0.0f;

	auto* t2 = ecs.GetStorage<Transform>().AddComponent(e2);
	auto* v2 = ecs.GetStorage<Velocity>().AddComponent(e2);
	v2->dx = -0.5f; v2->dy = 0.0f;

	for (int frame = 0; frame < 5; ++frame)
	{
		ecs.Update(1.0f);

		std::cout << "Frame " << frame << ": \n";
		for (auto& [id, t] : ecs.GetStorage<Transform>().GetComponents())
			std::cout << "  Entity " << id << " pos = ("
			<< t.x << ", " << t.y << ")\n";
	}
}