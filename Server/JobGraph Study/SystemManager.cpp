#include "SystemManager.h"
#include "LoginEventSystem.h"
#include "MoveEventSystem.h"
#include "MovementSystem.h"
#include "AnimationTimeSystem.h"
#include "AnimationRefSystem.h"
#include "ColliderUpdateSystem.h"
#include "CollisionSystem.h"

void SystemManager::Initalize(ECS& ecs)
{
	/* ------- [ Event System ] ------- */
	RegisterSystem<LoginEventSystem>(ecs, 0);
	RegisterSystem<MoveEventSystem>(ecs, 1);

	/* ------- [ Logic System ] ------- */
	RegisterSystem<MovementSystem>(ecs, 2);
	RegisterSystem<AnimationTimeSystem>(ecs, 3);
	RegisterSystem<AnimationRefSystem>(ecs, 4);
	RegisterSystem<ColliderUpdateSystem>(ecs, 5);
	RegisterSystem<CollisionSystem>(ecs, 6);
}
