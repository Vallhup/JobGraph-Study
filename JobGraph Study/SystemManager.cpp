#include "SystemManager.h"
#include "System.h"
#include "ThreadPool.h"
#include "JobGraph.h"
#include "Job.h"

void SystemManager::BuildDependency()
{
	_nodes[1]->AddDependency(_nodes[0]);
	_graph.Build();
}

void SystemManager::Update(const float dT)
{
	for (auto& sys : _systems)
		sys->Update(dT);
}

void SystemManager::UpdateParallel(const float dT)
{
	_deltaTime = dT;
	_graph.Run();
}