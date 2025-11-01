#pragma once

#include <vector>
#include <atomic>

class ThreadPool;

struct JobData {
	void (*func)(void*);
	void* context;
};

class JobNode {
public:
	explicit JobNode(JobData& job, JobGraph* graph) 
		: _job(job), _graph(graph), _deps(0) {}

	void AddDependency(JobNode* dependency);
	void Execute();

	bool Ready() const { return _deps == 0; }

private:
	JobData _job;
	JobGraph* _graph;
	std::vector<JobNode*> _dependents;
	std::atomic<int> _deps;
};

class JobGraph {
public:
	explicit JobGraph(ThreadPool& pool) : _pool(pool) {}

	void Schedule(JobNode* node);

private:
	ThreadPool& _pool;
};

