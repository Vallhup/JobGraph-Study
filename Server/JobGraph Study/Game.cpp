#include "Game.h"
#include "Instance.h"

Game::Game(size_t size)
	: threadPool(size), graph(threadPool)
{
	ecs.systemMng.Initalize(ecs);
	graph.AutoDependencyBuild(ecs.systemMng.GetSystems(), &_deltaTime);
}

void Game::Update(const float dT)
{
	_deltaTime = dT;
	graph.Run();
}
