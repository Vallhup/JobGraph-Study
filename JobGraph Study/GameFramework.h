#pragma once

#include <vector>
#include <memory>

#include "ThreadPool.h"
#include "GameWorld.h"
#include "Instance.h"
#include "ECS.h"

class GameFramework {
public:
	static GameFramework& Get()
	{
		static GameFramework instance{ 16 };
		return instance;
	}

	GameFramework(size_t size = std::thread::hardware_concurrency());

	void Update(const float dT);

	ECS _ecs;
	JobGraph _graph;
	ThreadPool _threadPool;
	GameWorld _world;

private:
	float _deltaTime{ 0.0f };
};