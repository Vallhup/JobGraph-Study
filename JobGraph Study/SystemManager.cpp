#include "SystemManager.h"
#include "System.h"

void SystemManager::Update(const float dT)
{
	for (auto& sys : _systems)
		sys->Update(dT);
}