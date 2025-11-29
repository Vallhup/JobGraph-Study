#pragma once

#include "ECS.h"
#include "System.h"

class CollisionSystem : public LogicSystem {
public:
	CollisionSystem(ECS& e, int p = 0) : LogicSystem(e, p) {}
	virtual ~CollisionSystem() = default;

	virtual void Update(const float dT) override;

	virtual std::vector<std::type_index> ReadComponents() const override
	{
		return { typeid(Collider) };
	}
};

