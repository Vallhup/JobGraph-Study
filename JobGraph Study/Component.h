#pragma once

struct Component {
public:
	virtual ~Component() = default;
};

struct Transform : public Component {
	float x{ 0.0f };
	float y{ 0.0f };
};

struct Velocity : public Component {
	float dx{ 0.0f };
	float dy{ 0.0f };
};