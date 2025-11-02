#include "GameFramework.h"
#include "Instance.h"

GameFramework::GameFramework(size_t size)
	: _threadPool(size), _systemMng(_threadPool) 
{
	// TODO : Register System
	
}

void GameFramework::Update(const float dT)
{
	for (auto& instance : _instances)
		instance->Update(dT);
}

void GameFramework::UpdateParallel(const float dT)
{
	for (auto& instance : _instances)
		instance->UpdateParallel(dT);
}

void GameFramework::AddInstance()
{
	auto inst = std::make_unique<Instance>(_threadPool, _systemMng);
	inst->Initalize();

	_instances.push_back(std::move(inst));
}
