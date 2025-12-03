#pragma once

#include <typeindex>
#include <vector>
#include <atomic>

#include "Event.h"

struct ECS;

class System {
public:
	System(ECS& e, int p = 0) : ecs(e), _priority(p) {}
	virtual ~System() = default;

	virtual void Execute(const float dT) = 0;

	virtual std::vector<std::type_index> ReadComponents() const { return {}; }
	virtual std::vector<std::type_index> WriteComponents() const { return {}; }

	int GetPriority() const { return _priority; }

protected:
	ECS& ecs;
	int _priority;
};