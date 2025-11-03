#include "SystemManager.h"

void SystemManager::Initalize(ECS& ecs)
{
	RegisterSystem<MovementSystem>(ecs);
	RegisterSystem<TestSystem>(ecs);
}
