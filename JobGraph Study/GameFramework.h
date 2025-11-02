#pragma once

#include <vector>
#include <memory>

#include "ThreadPool.h"
#include "Instance.h"

class GameFramework {
public:
	GameFramework(size_t size = std::thread::hardware_concurrency());

	void Update(const float dT);
	void UpdateParallel(const float dT);

	void AddInstance();

private:
	ThreadPool _threadPool;
	SystemManager _systemMng;
	std::vector<std::unique_ptr<Instance>> _instances;
};