#pragma once

#include <vector>
#include <memory>

#include "ThreadPool.h"
#include "Instance.h"

class GameFramework {
public:
	static GameFramework& Get()
	{
		static GameFramework instance;
		return instance;
	}

	GameFramework(size_t size = std::thread::hardware_concurrency());

	void Update(const float dT);

	ECS _ecs;
	JobGraph _graph;
	ThreadPool _threadPool;

private:
	
	float _deltaTime{ 0.0f };
};