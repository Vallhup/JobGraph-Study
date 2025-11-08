#include "Game.h"
#include "Instance.h"

Game::Game(size_t size)
	: _threadPool(size), _graph(_threadPool)
{
	_ecs.systemMng.Initalize(_ecs);
	_graph.AutoDependencyBuild(_ecs.systemMng.GetSystems(), &_deltaTime);
}

void Game::Update(const float dT)
{
	_deltaTime = dT;
	_graph.Run();
}
