#include "SystemManager.h"
#include "ConnectEventSystem.h"
#include "MoveEventSystem.h"
#include "MovementSystem.h"

void SystemManager::Initalize(ECS& ecs)
{
	RegisterSystem<ConnectEventSystem>(ecs, 0);
	RegisterSystem<MoveEventSystem>(ecs, 0);
	RegisterSystem<MovementSystem>(ecs, 1);
}
