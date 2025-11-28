#pragma once

#include "System.h"
#include "Component.h"

class MovementSystem : public LogicSystem {
public:
	MovementSystem(ECS& e, int p = 0) : LogicSystem(e, p) {}
	virtual ~MovementSystem() = default;

	virtual void Update(const float dT) override;

	virtual std::vector<std::type_index> ReadComponents() const override
	{
		return { typeid(Velocity) };
	}

	virtual std::vector<std::type_index> WriteComponents() const override
	{
		return { typeid(Transform) };
	}
};

