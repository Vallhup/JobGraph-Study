#include "GameFramework.h"
#include "Instance.h"

GameFramework::GameFramework(size_t size)
	: _threadPool(size), _graph(_threadPool)
{
	_ecs.systemMng.Initalize(_ecs);
	_graph.AutoDependencyBuild(_ecs.systemMng.GetSystems(), &_deltaTime);
}

void GameFramework::Update(const float dT)
{
	_deltaTime = dT;
	_graph.Run();
}
