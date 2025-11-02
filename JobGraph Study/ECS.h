#pragma once

#include "Entity.h"
#include "SystemManager.h"
#include "ComponentStorage.h"

class ThreadPool;

struct ECS {
	ECS(SystemManager& sys) : systemMng(sys) {}

	template<CompT T>
	ComponentStorage<T>& GetStorage()
	{
		static ComponentStorage<T> storage;
		return storage;
	}

	void Initialize();

	void Update(const float dT);
	void UpdateParallel(float dT);

	EntityManager entityMng;
	SystemManager& systemMng;
};

