#pragma once

#include <vector>
#include <atomic>
#include <latch>
#include <optional>

struct IJob;
class ThreadPool;
class JobGraph;

struct JobData {
	void (*func)(void*);
	void* context;
};

class JobNode {
	friend class JobGraph;

public:
	explicit JobNode(JobData& job, JobGraph* graph) 
		: _job(job), _graph(graph), _deps(0), _initDeps(0) {}

	void AddDependency(JobNode* dependency);
	void Execute();

	bool Ready() const { return _deps == 0; }
	void ResetDeps() { _deps = _initDeps; }

private:
	JobData _job;
	JobGraph* _graph;
	std::vector<JobNode*> _dependents;
	std::atomic<int> _deps;
	int _initDeps;
};

template<typename T>
concept JobType = std::is_base_of_v<IJob, T>;

class JobGraph {
	friend class JobNode;

public:
	explicit JobGraph(ThreadPool& pool) : _pool(pool), _latch(std::nullopt) {}
	~JobGraph();

	template<JobType Job, typename... Args>
	JobNode* CreateNode(Args&&... args);

	void Schedule(JobNode* node);
	void Build();
	void Run();

	void NotifyJobDone();

	const std::vector<IJob*>& GetJobs() const { return _allocatedJobs; }

private:
	ThreadPool& _pool;
	std::vector<JobNode*> _nodes;
	std::vector<IJob*> _allocatedJobs;

	std::optional<std::latch> _latch;
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
