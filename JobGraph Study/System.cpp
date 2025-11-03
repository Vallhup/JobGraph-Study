#include "System.h"
#include "ECS.h"
#include "Component.h"
#include "GameFramework.h"

#include <thread>
#include <iostream>

/*--------------------[ MovementSystem ]--------------------*/

void MovementSystem::Update(const float dT)
{
	auto& transform = _ecs.GetStorage<Transform>();
	auto& velocity = _ecs.GetStorage<Velocity>();

	const auto& entitiesT = transform.Entities();
	auto& denseT = transform.Dense();

	for (size_t i = 0; i < denseT.size(); ++i)
	{
		Entity e = entitiesT[i];
		int velIdx = velocity.DenseIndex(e);

		if (velIdx == -1) continue;

		auto& t = denseT[i];
		auto& v = velocity.Dense()[velIdx];

		t.x += v.dx * dT;
		t.y += v.dy * dT;
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
	const int entityCount = 1000;
	const int chunkSize = 100;
	auto& pool = GameFramework::Get()._threadPool;

	std::vector<float> positions(entityCount, 0.0f);
	std::vector<float> velocities(entityCount, 0.1f);

	pool.ParallelFor(entityCount, chunkSize, [&](int begin, int end)
		{
			for (int i = begin; i < end; ++i)
			{
				float v = velocities[i];
				float t = positions[i];
				// 캐시 내에서 반복 연산 유도
				for (int j = 0; j < 100; ++j)
					t = t * 0.98f + v * 0.001f * sinf(v + j);

				positions[i] = t;
			}
		});

#ifdef _DEBUG
	std::cout << "[TestSystem] Thread: " << std::this_thread::get_id()
		<< " dT: " << dT << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif
}