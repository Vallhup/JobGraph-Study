#pragma once

#include "ECS.h"
#include "System.h"

class AnimationStateSystem : public System {
public:
	AnimationStateSystem(ECS& e, int p = 0) : System(e, p) {}
	virtual ~AnimationStateSystem() = default;

	virtual void Execute(const float dT) override
	{
		auto& states = ecs.GetStorage<AnimationState>();
		auto& velocities = ecs.GetStorage<Velocity>();

		for (const auto& [entity, state] : states)
		{
			auto* vel = velocities.GetComponent(entity);
			if (!vel) continue;

			AnimationId desired{ state.id };

			// TODO : 상태에 따라 애니메이션 상태도 변화
			if (vel->inputX == 0 && vel->inputZ == 0)
				desired = AnimationId::Knight_Idle;
			else
				desired = AnimationId::Knight_Walk;

			ChangeAnimation(entity, desired);
		}
	}

	virtual std::vector<std::type_index> ReadComponents() const
	{
		return { typeid(Velocity) };
	}

	virtual std::vector<std::type_index> WriteComponents() const
	{
		return { typeid(AnimationState) };
	}

private:
	void ChangeAnimation(Entity e, AnimationId id)
	{
		auto* state = ecs.GetStorage<AnimationState>().GetComponent(e);
		if (!state) return;
		if (state->id == id) return;

		state->id = id;
		state->time = 0.0f;

		switch (id) {
		case AnimationId::Knight_Idle:
		case AnimationId::Knight_Walk:
		case AnimationId::Knight_Run:
			state->looping = true;
			break;
		}
	}
};

