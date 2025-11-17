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

	virtual std::vector<std::type_index> ReadComponents() const { return {}; }
	virtual std::vector<std::type_index> WriteComponents() const { return {}; }

	int GetPriority() const { return _priority; }

protected:
	ECS& ecs;
	int _priority;
};

class LogicSystem : public System {
public:
	LogicSystem(ECS& e, int p = 0) : System(e, p) {}
	virtual ~LogicSystem() = default;

	virtual void Update(const float dT) = 0;
};

class EventSystemBase : public System {
public:
	EventSystemBase(ECS& e, int p = 0) : System(e, p) {}
	virtual ~EventSystemBase() = default;

	virtual void ConsumeEvents() = 0;
};

//template<typename EventT>
//class EventSystem : public EventSystemBase {
//public:
//	EventSystem(ECS& e, int p = 0) : EventSystemBase(e, p) {}
//	virtual ~EventSystem() = default;
//
//	virtual void ProcessEvent(const EventT& e) = 0;
//	virtual void ConsumeEvents() override;
//};