#pragma once

#include "ECS.h"
#include "System.h"

class CollisionSystem : public LogicSystem {
public:
	CollisionSystem(ECS& e, int p = 0) : LogicSystem(e, p) {}
	virtual ~CollisionSystem() = default;

	virtual void Update(const float dT) override
	{
		// TODO : Collider와 Transform을 모두 가진
		//        Entity 집합 찾아서 충돌처리
	}

	virtual std::vector<std::type_index> ReadComponents() const override
	{
		return { typeid(Transform), typeid(Collider) };
	}
};

