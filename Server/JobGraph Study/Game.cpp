#include "Game.h"
#include "Instance.h"

#include "CollisionSystem.h"
#include "OutputEventSystem.h"

Game::Game(size_t size)
	: threadPool(size), graph(threadPool)
{
	ecs.systemMng.Initalize(ecs, graph);
	graph.AutoDependencyBuild(ecs.systemMng.GetSystems(), &_deltaTime);

	auto* colSys = ecs.systemMng.GetSystem<CollisionSystem>();
	auto* outSys = ecs.systemMng.GetSystem<OutputEventSystem>();
	if (colSys && outSys)
		graph.AddManualDependency(colSys, outSys);

	graph.Build();
}

void Game::Update(const float dT)
{
	_deltaTime = dT;
	graph.Run();
}
