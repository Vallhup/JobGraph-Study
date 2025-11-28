#pragma once

#include <atomic>
#include <vector>
#include <DirectXMath.h>

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

};

struct Collider : public Component {
	struct Capsule {
		XMFLOAT3 p1;
		XMFLOAT3 p2;
		float radius;
	};

	std::vector<Capsule> colliders;
};