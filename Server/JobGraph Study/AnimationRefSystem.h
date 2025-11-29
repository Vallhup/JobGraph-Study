#pragma once

#include "ECS.h"
#include "System.h"

class AnimationRefSystem : public LogicSystem {
public:
	AnimationRefSystem(ECS& e, int p = 0) : LogicSystem(e, p) {}
	virtual ~AnimationRefSystem() = default;

	virtual void Update(const float dT) override
	{
		auto& animRefs = ecs.GetStorage<AnimationRef>();
		auto& anims = ecs.GetStorage<Animator>();
		auto& colliders = ecs.GetStorage<Collider>();

		for (const auto& [entity, anim] : anims)
		{
			auto* animRef = animRefs.GetComponent(entity);
			auto* collider = colliders.GetComponent(entity);
			if (!animRef or !collider) continue;
			if (!animRef->anim) continue;

			const PrebakedAnimation* clip = animRef->anim;
			int frame = anim.currentFrame;
			if (frame < 0 or frame >= clip->numFrames) continue;

			const auto& localCapsules = clip->frames[frame];
			collider->localCapsules = localCapsules;
		}
	}

	virtual std::vector<std::type_index> ReadComponents() const
	{
		return { typeid(AnimationRef), typeid(Animator) };
	}

	virtual std::vector<std::type_index> WriteComponents() const
	{
		return { typeid(Collider) };
	}
};

