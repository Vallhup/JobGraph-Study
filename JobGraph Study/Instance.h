#pragma once

#include "ECS.h"
#include "SystemManager.h"

class ThreadPool;

class Instance {
public:
	Instance(ThreadPool& pool, SystemManager& sys) 
		: _threadPool(pool), _ecs(sys) {}

	void Initalize() { _ecs.Initialize(); }
	void Update(const float dT) { _ecs.Update(dT); }
	void UpdateParallel(const float dT) { _ecs.UpdateParallel(dT); }

private:
	ECS _ecs;
	ThreadPool& _threadPool;
};