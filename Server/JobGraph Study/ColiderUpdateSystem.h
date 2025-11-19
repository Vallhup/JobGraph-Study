#pragma once

#include "ECS.h"
#include "System.h"


class ColiderUpdateSystem : public LogicSystem {
public:
	ColiderUpdateSystem(ECS& e, int p = 0) : LogicSystem(e, p) {}
	virtual ~ColiderUpdateSystem() = default;

	virtual void Update(const float dT) override
	{
		auto& transforms = ecs.GetStorage<Transform>();
		auto& colliders = ecs.GetStorage<Collider>();

		for (const auto& [entity, collider] : colliders)
		{
			for (auto& c : collider.colliders)
			{
				// TODO : Transform에 맞게 월드 변환 수행
			}
		}
	}

	virtual std::vector<std::type_index> ReadComponents() const override
	{
		return { typeid(Transform) };
	}

	virtual std::vector<std::type_index> WriteComponents() const override
	{
		return { typeid(Collider) };
	}
};

