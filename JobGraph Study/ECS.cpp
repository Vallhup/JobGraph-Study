#include "ECS.h"
#include "System.h"

void ECS::Initialize()
{
	// TEMP : GameFramework로 옮겨야 함
	systemMng.RegisterSystem<MovementSystem>(*this);
	systemMng.RegisterSystem<TestSystem>();

	systemMng.BuildDependency();
}

void ECS::Update(float dT)
{
	systemMng.Update(dT);
}

void ECS::UpdateParallel(float dT)
{
	systemMng.UpdateParallel(dT);
}
