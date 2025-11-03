#pragma once

#include <memory>
#include <vector>

#include "System.h"
#include "JobGraph.h"

class ThreadPool;

template<typename T>
concept SysT = std::is_base_of_v<System, T>;

class SystemManager {
public:
	template<SysT T, typename... Args>
	void RegisterSystem(Args&&... args)
	{
		auto sys = std::make_unique<T>(std::forward<Args>(args)...);
		T* ptr = sys.get();
		_systems.push_back(std::move(sys));
	}

	size_t GetSystemCount() const { return _systems.size(); }

private:
	std::vector<std::unique_ptr<System>> _systems;
	float _deltaTime;
};
