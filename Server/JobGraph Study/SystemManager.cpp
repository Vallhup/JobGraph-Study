#include "SystemManager.h"
#include "LoginEventSystem.h"
#include "MovementSystem.h"

void SystemManager::Initalize(ECS& ecs)
{
	/* ------- [ Event System ] ------- */
	RegisterSystem<LoginEventSystem>(ecs, 0);

	/* ------- [ Logic System ] ------- */
	RegisterSystem<MovementSystem>(ecs, 1);
}
