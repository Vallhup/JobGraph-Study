#pragma once

#include "ECS.h"
#include "System.h"
#include "Component.h"

class MovementSystem : public LogicSystem {
public:
	MovementSystem(ECS& e, int p = 0) : LogicSystem(e, p) {}
	virtual ~MovementSystem() = default;

	virtual void Update(const float dT) override
	{
		auto& transforms = ecs.GetStorage<Transform>();
		auto& velocitys = ecs.GetStorage<Velocity>();

		for (const auto& [entity, transform] : transforms)
		{
			if (auto* velComp = velocitys.GetComponent(entity))
			{
				transform.x += velComp->dirX;
				transform.z += velComp->dirZ;
			}
		}
	}
};

