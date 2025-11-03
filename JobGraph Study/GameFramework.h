#pragma once

#include <vector>
#include <memory>

#include "ThreadPool.h"
#include "Instance.h"

class GameFramework {
public:
	GameFramework(size_t size = std::thread::hardware_concurrency());

	void Update(const float dT);

	void AddInstance();

private:
	ECS _ecs;
	JobGraph _graph;
	ThreadPool _threadPool;
	
	std::vector<std::unique_ptr<Instance>> _instances;
};