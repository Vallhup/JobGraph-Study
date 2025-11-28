#include "SystemManager.h"
#include "LoginEventSystem.h"
#include "MoveEventSystem.h"
#include "MovementSystem.h"

void SystemManager::Initalize(ECS& ecs)
{
	/* ------- [ Event System ] ------- */
	RegisterSystem<LoginEventSystem>(ecs, 0);
	RegisterSystem<MoveEventSystem>(ecs, 1);

	/* ------- [ Logic System ] ------- */
	RegisterSystem<MovementSystem>(ecs, 2);
}
