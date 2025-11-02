#pragma once

#include <vector>
#include <atomic>
#include <latch>
#include <optional>
#include <barrier>

struct Job;
class ThreadPool;
class JobGraph;

template<typename T>
concept JobT = std::is_base_of_v<Job, T>;

template<JobT T>
struct JobDataT {
	static void Execute(void* ctx)
	{
		static_cast<T*>(ctx)->Execute();
	}
};

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

class JobGraph {
	friend class JobNode;

public:
	explicit JobGraph(ThreadPool& pool) : _pool(pool), _latch(std::nullopt) {}
	~JobGraph();

	template<JobT T, typename... Args>
	JobNode* CreateNode(Args&&... args);

	void Schedule(JobNode* node);
	void Build();
	void Run();

	void NotifyJobDone();

	const std::vector<Job*>& GetJobs() const { return _allocatedJobs; }

private:
	ThreadPool& _pool;
	std::vector<JobNode*> _nodes;
	std::vector<Job*> _allocatedJobs;

	std::optional<std::latch> _latch;
};

template<JobT T, typename... Args>
inline JobNode* JobGraph::CreateNode(Args&&... args)
{
	T* job = new T(std::forward<Args>(args)...);
	_allocatedJobs.push_back(job);

	JobData data{ &JobDataT<T>::Execute, job };
	JobNode* node = new JobNode(data, this);
	_nodes.push_back(node);

	return node;
}
