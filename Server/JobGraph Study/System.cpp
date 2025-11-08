#include "System.h"
#include "ECS.h"
#include "Component.h"
#include "Game.h"

#include <thread>
#include <iostream>

/*--------------------[ MovementSystem ]--------------------*/

void MovementSystem::Update(const float dT)
{
	auto& transform = _ecs.GetStorage<Transform>();
	auto& velocity = _ecs.GetStorage<Velocity>();

	for (auto [entity, t] : transform)
	{
		int velIdx = velocity.DenseIndex(entity);
		if (velIdx == -1) continue;

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

/*--------------------[ InputSystem ]--------------------*/

void InputSystem::Update(const float dT)
{
	auto& inputs = _ecs.GetStorage<Input>();
	auto& velocity = _ecs.GetStorage<Velocity>();

	const auto& entities = inputs.Entities();
	for (size_t i = 0; i < entities.size(); ++i)
	{
		Entity e = entities[i];
		int velIdx = velocity.DenseIndex(e);
		if (velIdx == -1) continue;

		auto& in = inputs.Dense()[i];
		auto& v = velocity.Dense()[velIdx];

		v.dx = (in.right - in.left) * 3.0f;
		v.dy = (in.up - in.down) * 3.0f;
	}

#ifdef _DEBUG
	std::cout << "[InputSystem] Thread: " << std::this_thread::get_id()
		<< " dT: " << dT << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif
}

std::vector<std::type_index> InputSystem::ReadComponents() const
{
	return { typeid(Input) };
}

std::vector<std::type_index> InputSystem::WriteComponents() const
{
	return { typeid(Velocity) };
}

/*--------------------[ AISystem ]--------------------*/

void AISystem::Update(const float dT)
{
	auto& ai = _ecs.GetStorage<AIState>();
	auto& tr = _ecs.GetStorage<Transform>();

	const auto& entities = ai.Entities();
	for (size_t i = 0; i < entities.size(); ++i)
	{
		Entity e = entities[i];
		int trIdx = tr.DenseIndex(e);
		if (trIdx == -1) continue;

		auto& a = ai.Dense()[i];
		auto& t = tr.Dense()[trIdx];

		// 계산량 부하 유도
		float val = sinf(t.x * 0.1f) * cosf(t.y * 0.1f);
		a.state = (val > 0.5f ? 1 : 0);
	}

#ifdef _DEBUG
	std::cout << "[AISystem] Thread: " << std::this_thread::get_id()
		<< " dT: " << dT << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif
}

std::vector<std::type_index> AISystem::ReadComponents() const
{
	return { typeid(Transform) };
}

std::vector<std::type_index> AISystem::WriteComponents() const
{
	return { typeid(Velocity) };
}

/*--------------------[ CollisionSystem ]--------------------*/

void CollisionSystem::Update(const float dT)
{
	//auto& tr = _ecs.GetStorage<Transform>();
	//auto& dense = tr.Dense();
	//const size_t N = dense.size();
	//auto& pool = Game::Get()._threadPool;

	//_collisions = 0;

	//const int chunkSize = 500; // Batch 단위
	//pool.ParallelFor(N, chunkSize, [&](int begin, int end)
	//	{
	//		int localCount = 0;
	//		for (int i = begin; i < end; ++i)
	//		{
	//			for (size_t j = i + 1; j < N; ++j)
	//			{
	//				float dx = dense[i].x - dense[j].x;
	//				float dy = dense[i].y - dense[j].y;
	//				float dist2 = dx * dx + dy * dy;
	//				if (dist2 < 16.0f)
	//					localCount++;
	//			}
	//		}

	//		// localCount는 atomic 합산
	//		_collisions += localCount;
	//	});

#ifdef _DEBUG
	std::cout << "[CollisionSystem] Thread: " << std::this_thread::get_id()
		<< " dT: " << dT << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif
}

std::vector<std::type_index> CollisionSystem::ReadComponents() const
{
	return { typeid(Transform) };
}

std::vector<std::type_index> CollisionSystem::WriteComponents() const
{
	return { };
}

/*--------------------[ CombatSystem ]--------------------*/

void CombatSystem::Update(const float dT)
{
	auto& transform = _ecs.GetStorage<Transform>();
	auto& stat = _ecs.GetStorage<Stat>();
	auto& ai = _ecs.GetStorage<AIState>();

	const auto& entities = transform.Entities();
	auto& denseTr = transform.Dense();

	// 실제 서버에서는 "공격 범위 안에 있는 대상"을 찾지만,
	// 여기선 단순히 가까운 Entity에게 HP 데미지를 주는 방식으로 시뮬레이션
	const size_t N = denseTr.size();

	for (size_t i = 0; i < N; ++i)
	{
		Entity e1 = entities[i];
		int stIdx1 = stat.DenseIndex(e1);
		int aiIdx1 = ai.DenseIndex(e1);
		if (stIdx1 == -1 || aiIdx1 == -1) continue;

		auto& tr1 = denseTr[i];
		auto& s1 = stat.Dense()[stIdx1];
		auto& a1 = ai.Dense()[aiIdx1];

		// 공격 상태면 주변 탐색
		if (a1.state == 2 && s1.hp > 0)
		{
			for (size_t j = 0; j < N; ++j)
			{
				if (i == j) continue;

				float dx = tr1.x - denseTr[j].x;
				float dy = tr1.y - denseTr[j].y;
				float dist2 = dx * dx + dy * dy;

				if (dist2 < 16.0f)
				{
					Entity e2 = entities[j];
					int stIdx2 = stat.DenseIndex(e2);
					if (stIdx2 == -1) continue;

					auto& s2 = stat.Dense()[stIdx2];
					s2.hp -= 5; // 단순 데미지
					if (s2.hp < 0) s2.hp = 0;
				}
			}
		}
	}

#ifdef _DEBUG
	std::cout << "[CombatSystem] Thread: " << std::this_thread::get_id()
		<< " dT: " << dT << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif
}

std::vector<std::type_index> CombatSystem::ReadComponents() const
{
	return { typeid(Transform), typeid(AIState), typeid(Stat) };
}

std::vector<std::type_index> CombatSystem::WriteComponents() const
{
	return { typeid(Stat) };
}

/*--------------------[ StatRegenSystem ]--------------------*/

void StatRegenSystem::Update(const float dT)
{
	auto& stats = _ecs.GetStorage<Stat>();
	for (auto& s : stats.Dense())
	{
		s.hp += 1;
		if (s.hp > 100) s.hp = 100;
	}

#ifdef _DEBUG
	std::cout << "[StatRegenSystem] Thread: " << std::this_thread::get_id()
		<< " dT: " << dT << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif
}

std::vector<std::type_index> StatRegenSystem::ReadComponents() const
{
	return { };
}

std::vector<std::type_index> StatRegenSystem::WriteComponents() const
{
	return { typeid(Stat) };
}

/*--------------------[ VisibilitySystem ]--------------------*/

void VisibilitySystem::Update(const float dT)
{
	/*auto& transform = _ecs.GetStorage<Transform>();
	auto& view = _ecs.GetStorage<View>();

	const auto& entities = transform.Entities();
	auto& denseTr = transform.Dense();
	auto& denseView = view.Dense();
	const size_t N = denseTr.size();
	auto& pool = Game::Get()._threadPool;

	const int chunk = 500;
	pool.ParallelFor(N, chunk, [&](int begin, int end)
		{
			for (int i = begin; i < end; ++i)
			{
				Entity e = entities[i];
				int vIdx = view.DenseIndex(e);
				if (vIdx == -1) continue;

				auto& tr = denseTr[i];
				auto& v = denseView[vIdx];
				v.nearbyEntities.clear();

				for (size_t j = 0; j < N; ++j)
				{
					if (i == j) continue;
					float dx = tr.x - denseTr[j].x;
					float dy = tr.y - denseTr[j].y;
					if (dx * dx + dy * dy < _viewRadiusSq)
						v.nearbyEntities.push_back(entities[j].id);
				}
			}
		});*/

#ifdef _DEBUG
	std::cout << "[VisibilitySystem] Thread: " << std::this_thread::get_id()
		<< " dT: " << dT << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif
}

std::vector<std::type_index> VisibilitySystem::ReadComponents() const
{
	return { typeid(Transform) };
}

std::vector<std::type_index> VisibilitySystem::WriteComponents() const
{
	return { typeid(View) };
}