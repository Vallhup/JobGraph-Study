#include "SystemManager.h"
#include "ConnectEventSystem.h"
#include "MoveEventSystem.h"
#include "MovementSystem.h"

void SystemManager::Initalize(ECS& ecs)
{
	/* ------- [ Event System ] ------- */
	RegisterSystem<ConnectEventSystem>(ecs, 0);
	RegisterSystem<MoveEventSystem>(ecs, 0);

	/* ------- [ Logic System ] ------- */
	RegisterSystem<MovementSystem>(ecs, 1);
}
