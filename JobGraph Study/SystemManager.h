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
	SystemManager(ThreadPool& pool)
		: _threadPool(pool), _graph(pool), _deltaTime(0.0f) {}

	template<SysT T, typename... Args>
	void RegisterSystem(Args&&... args)
	{
		auto sys = std::make_unique<T>(std::forward<Args>(args)...);
		T* ptr = sys.get();
		_systems.push_back(std::move(sys));

		auto* node = _graph.CreateNode<SystemJob>(ptr, &_deltaTime);
		_nodes.push_back(node);
	}

	void BuildDependency();

	void Update(const float dT);
	void UpdateParallel(const float dT);

	size_t GetSystemCount() const { return _systems.size(); }

private:
	ThreadPool& _threadPool;
	JobGraph _graph;
	std::vector<JobNode*> _nodes;
	std::vector<std::unique_ptr<System>> _systems;

	float _deltaTime;
};
