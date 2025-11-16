#include "SystemManager.h"
#include "ConnectEventSystem.h"

void SystemManager::Initalize(ECS& ecs)
{
	RegisterSystem<ConnectEventSystem>(0);
}
