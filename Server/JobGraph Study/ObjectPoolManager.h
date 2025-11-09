#pragma once

#include "ObjectPool.hpp"

class SendBuffer;

class ObjectPoolManager {
public:
	static ObjectPoolManager& Get()
	{
		static ObjectPoolManager instance;
		return instance;
	}

	static thread_local ObjectPool<SendBuffer, 8192> sendBuffers;
	// TODO : ObjectPool Ãß°¡

private:
	ObjectPoolManager() = default;
	~ObjectPoolManager() = default;

	ObjectPoolManager(const ObjectPoolManager&) = delete;
	ObjectPoolManager& operator=(const ObjectPoolManager&) = delete;
};

