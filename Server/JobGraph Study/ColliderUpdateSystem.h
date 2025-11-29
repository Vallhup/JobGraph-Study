#pragma once

#include "ECS.h"
#include "System.h"

class ColliderUpdateSystem : public LogicSystem {
public:
	ColliderUpdateSystem(ECS& e, int p = 0) : LogicSystem(e, p) {}
	virtual ~ColliderUpdateSystem() = default;

	virtual void Update(const float dT) override;

	virtual std::vector<std::type_index> ReadComponents() const override
	{
		return { typeid(Transform) };
	}

	virtual std::vector<std::type_index> WriteComponents() const override
	{
		return { typeid(Collider) };
	}
};

