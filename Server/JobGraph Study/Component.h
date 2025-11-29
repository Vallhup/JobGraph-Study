#pragma once

#include <atomic>
#include <vector>
#include <DirectXMath.h>

#include "AnimationManager.h"

using namespace DirectX;

struct Component {
public:
	virtual ~Component() = default;
};

struct Transform : public Component {
	XMFLOAT3 position{ 0, 0, 0 };
	XMFLOAT4 rotation{ 0, 0, 0, 1 };
	XMFLOAT3 scale{ 1, 1, 1 };
};

struct Velocity : public Component {
	int inputX{ 0 };
	int inputZ{ 0 };
	float yaw{ 0.0f };
};

struct AnimationState : public Component {
	AnimationId id{ AnimationId::None };
	float time{ 0.0f };
	float speed{ 1.0f };
	bool looping{ true };
};

struct AnimationRef : public Component {
	const PrebakedAnimation* anim{ nullptr };
};

struct Animator : public Component {
	int currentFrame{ 0 };
};

struct Collider : public Component {
	std::vector<Capsule> localCapsules;
	std::vector<Capsule> worldCapsules;
};