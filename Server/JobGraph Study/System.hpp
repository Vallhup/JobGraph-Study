#pragma once

#include <typeindex>
#include <vector>
#include <atomic>

#include "Event.h"
#include "Framework.h"

class System {
public:
	System(int p = 0) : _priority(p) {}
	virtual ~System() = default;

	virtual std::vector<std::type_index> ReadComponents() const { return {}; }
	virtual std::vector<std::type_index> WriteComponents() const { return {}; }

	int GetPriority() const { return _priority; }

protected:
	int _priority;
};

class LogicSystem : public System {
public:
	LogicSystem(int p = 0) : System(p) {}
	virtual ~LogicSystem() = default;

	virtual void Update(const float dT) = 0;
};

class EventSystemBase : public System {
public:
	EventSystemBase(int p = 0) : System(p) {}
	virtual ~EventSystemBase() = default;

	virtual void ConsumeEvents() = 0;
};

template<typename EventT>
class EventSystem : public System {
public:
	EventSystem(int p = 0) : EventSystemBase(p) {}
	virtual ~EventSystem() = default;

	virtual void ProcessEvent(const EventT& e) = 0;
	void ConsumeEvents()
	{
		GameEvent event;
		while (Framework::Get().eventQueue.try_pop(event))
		{
			std::visit(
				[&](auto&& e)
				{
					ProcessEvent(e);
				}, event);
		}
	}
};
