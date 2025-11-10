#pragma once

#include <vector>
#include <memory>

#include "ThreadPool.h"
#include "GameWorld.h"
#include "Instance.h"
#include "ECS.h"

#include "Constants.h"

class Game {
public:
	Game(size_t size);

	void Update(const float dT);

	ECS _ecs;
	JobGraph _graph;
	ThreadPool _threadPool;
	GameWorld _world;

private:
	float _deltaTime{ 0.0f };
};