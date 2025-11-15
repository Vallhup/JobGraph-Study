#pragma once

#include <memory>
#include <vector>

#include "System.hpp"
#include "JobGraph.h"

class ThreadPool;

template<typename T>
concept SysT = std::is_base_of_v<System, T>;

class SystemManager {
public:
	template<SysT T, typename... Args>
	T* RegisterSystem(Args&&... args)
	{
		auto sys = std::make_unique<T>(std::forward<Args>(args)...);
		T* ptr = sys.get();
		_systems.push_back(std::move(sys));

		if constexpr (std::is_base_of_v<EventSystemBase, T>)
			_eventSystems.push_back(static_cast<EventSystemBase*>(ptr));

		else
			_logicSystems.push_back(static_cast<LogicSystem*>(ptr));

		return ptr;
	}

	void Initalize(ECS& ecs);

	const std::vector<LogicSystem*>& GetLogicSystems() const { return _logicSystems; }
	const std::vector<EventSystemBase*>& GetEventSystems() const { return _eventSystems; }

private:
	std::vector<std::unique_ptr<System>> _systems;
	std::vector<LogicSystem*> _logicSystems;
	std::vector<EventSystemBase*> _eventSystems;
};
