#include "Game.h"
#include "Instance.h"

Game::Game(size_t size)
	: threadPool(size), graph(threadPool)
{
	ecs.systemMng.Initalize(ecs);
	graph.AddEventSystems(ecs.systemMng.GetEventSystems());
	graph.AutoDependencyBuild(ecs.systemMng.GetLogicSystems(), &_deltaTime);
}

void Game::Update(const float dT)
{
	_deltaTime = dT;
	graph.Run();
}
