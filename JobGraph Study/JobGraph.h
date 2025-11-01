#pragma once

#include <vector>
#include <atomic>

struct IJob;
class ThreadPool;
class JobGraph;

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

template<typename T>
concept JobType = std::is_base_of_v<IJob, T>;

class JobGraph {
public:
	explicit JobGraph(ThreadPool& pool) : _pool(pool) {}
	~JobGraph();

	template<JobType Job, typename... Args>
	JobNode* CreateNode(Args&&... args);

	void Schedule(JobNode* node);
	void Build();

private:
	ThreadPool& _pool;
	std::vector<JobNode*> _nodes;
	std::vector<IJob*> _allocatedJobs;
};

template<JobType Job, typename... Args>
inline JobNode* JobGraph::CreateNode(Args&&... args)
{
	Job* job = new Job(std::forward<Args>(args)...);
	_allocatedJobs.push_back(job);

	JobData data{ Job::Execute, job };
	JobNode* node = new JobNode(data, this);
	_nodes.push_back(node);

	return node;
}
