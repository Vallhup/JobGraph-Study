#pragma once

#include <vector>

struct Component {
public:
	virtual ~Component() = default;
};

struct Transform : public Component {
	float x{ 0.0f };
	float y{ 0.0f };
	float z{ 0.0f };
};

struct Velocity : public Component {
	float dirX{ 0.0f };
	float dirZ{ 0.0f };
};