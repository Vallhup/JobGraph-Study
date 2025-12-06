#pragma once

#include <iostream>
#include "ECS.h"
#include "System.h"

class OutputEventSystem : public System {
	using EventHandler = std::function<void(const OutputEvent&)>;

public:
	OutputEventSystem(ECS& ecs, int p = 0);
	virtual ~OutputEventSystem() = default;

	virtual void Execute(const float dT) override;

private:
	void ProcessSpawn(const OutputEvent& event);
	void ProcessMove(const OutputEvent& event);

	std::unordered_map<DirtyType, EventHandler> _handlers;
};

