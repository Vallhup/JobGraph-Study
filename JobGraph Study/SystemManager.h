#pragma once

#include <memory>
#include <vector>

class System;

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
		return ptr;
	}

	void Update(const float dT);

	size_t GetSystemCount() const { return _systems.size(); }

private:
	std::vector<std::unique_ptr<System>> _systems;
};
