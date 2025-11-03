#include "GameFramework.h"
#include "Instance.h"

GameFramework::GameFramework(size_t size)
	: _threadPool(size), _graph(_threadPool)
{
	// TODO : Register System
	
}

void GameFramework::Update(const float dT)
{
	_ecs.systemMng;
}

void GameFramework::AddInstance()
{
	auto inst = std::make_unique<Instance>();
	_instances.push_back(std::move(inst));
}
