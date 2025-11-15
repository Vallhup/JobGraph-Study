#pragma once

#include "Entity.h"
#include "SystemManager.hpp"
#include "ComponentStorage.h"

class ThreadPool;

struct ECS {
	template<CompT T>
	ComponentStorage<T>& GetStorage()
	{
		static ComponentStorage<T> storage;
		return storage;
	}

	EntityManager entityMng;
	SystemManager systemMng;
};