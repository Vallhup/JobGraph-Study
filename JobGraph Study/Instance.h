#pragma once

#include <memory>
#include "JobGraph.h"

class ThreadPool;

//class Instance {
//public:
//	explicit Instance(ThreadPool& pool) : _pool(pool), _graph(pool) {}
//
//	void AddObject(std::unique_ptr<GameObject> obj);
//	void BuildGraph();
//	void Tick(const float deltaTime);
//
//private:
//	ThreadPool& _pool;
//	JobGraph _graph;
//
//	std::vector<std::unique_ptr<GameObject>> _objects;
//};

