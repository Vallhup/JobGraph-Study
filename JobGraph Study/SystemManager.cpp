#include "SystemManager.h"

void SystemManager::Initalize(ECS& ecs)
{
	RegisterSystem<MovementSystem>(ecs);
	RegisterSystem<InputSystem>(ecs);
	RegisterSystem<AISystem>(ecs);
	RegisterSystem<CollisionSystem>(ecs);
	RegisterSystem<CombatSystem>(ecs);
	RegisterSystem<StatRegenSystem>(ecs);
	RegisterSystem<VisibilitySystem>(ecs);
}
