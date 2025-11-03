#pragma once

#include <vector>

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

struct Input : public Component {
	bool up{ false };
	bool down{ false };
	bool left{ false };
	bool right{ false };
};

struct Stat : public Component {
	int hp;
	int mp;
};

struct AIState : public Component {
	int state;
};

struct View : public Component {
	std::vector<int> nearbyEntities;
};