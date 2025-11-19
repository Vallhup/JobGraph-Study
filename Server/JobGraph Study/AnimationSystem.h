#pragma once

#include "ECS.h"
#include "System.h"

class AnimationSystem : public LogicSystem {
public:
	AnimationSystem(ECS& e, int p = 0) : LogicSystem(e, p) {}
	virtual ~AnimationSystem() = default;

	virtual void Update(const float dT) override
	{
		// TODO : 애니메이션 수행 (보간 포함?)
	}
};

