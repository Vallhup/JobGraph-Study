#include "System.h"
#include "ECS.h"
#include "Component.h"

void MovementSystem::Update(const float dT)
{
	auto& transforms = _ecs.GetStorage<Transform>().GetComponents();
	auto& velocities = _ecs.GetStorage<Velocity>().GetComponents();

	for (auto& [id, t] : transforms)
	{
		auto it = velocities.find(id);
		if (it != velocities.end())
		{
			t.x += it->second.dx * dT;
			t.y += it->second.dy * dT;
		}
	}
}