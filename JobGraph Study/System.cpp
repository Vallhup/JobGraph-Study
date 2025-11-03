#include "System.h"
#include "ECS.h"
#include "Component.h"

#include <thread>
#include <iostream>

/*--------------------[ MovementSystem ]--------------------*/

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

#ifdef _DEBUG
	std::cout << "[MovementSystem] Thread: " << std::this_thread::get_id()
		<< " dT: " << dT << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif
}

std::vector<std::type_index> MovementSystem::ReadComponents() const
{
	return { typeid(Velocity) };
}

std::vector<std::type_index> MovementSystem::WriteComponents() const
{
	return { typeid(Transform) };
}

/*--------------------[ TestSystem ]--------------------*/

void TestSystem::Update(const float dT)
{
	volatile float x{ 0.0f };
	for (int i = 0; i < 1000; ++i)
	{
		x += sin(x * i);
	}

#ifdef _DEBUG
	std::cout << "[TestSystem] Thread: " << std::this_thread::get_id()
		<< " dT: " << dT << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif
}