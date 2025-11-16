#pragma once

#include <vector>
#include <atomic>
#include <latch>
#include <optional>
#include <unordered_set>

#include "Job.h"

class ThreadPool;
class JobGraph;

class LogicSystem;
class EventSystemBase;

enum class JobLayer {
	NONE = 0,
	EVENT = 1,
	LOGIC = 2
};

struct JobData {
	void (*func)(void*);
	void* context;
};

class alignas(64) JobNode {
	friend class JobGraph;

public:
	explicit JobNode(const JobData& job, JobGraph* graph) 
		: _job(job), _graph(graph), _deps(0), _initDeps(0), 
		_done(false), _layer(JobLayer::NONE) {}

	void AddDependency(JobNode* dependency);
	void Execute();

	bool Ready() const { return _deps == 0; }
	void ResetDeps() { _deps = _initDeps; _done = false; }
	const std::vector<JobNode*>& Dependents() const { return _dependents; }
	
protected:
	JobData _job;
	JobLayer _layer;
	JobGraph* _graph;
	std::vector<JobNode*> _dependents;
	std::atomic<int> _deps;
	int _initDeps;

	std::atomic<bool> _done;
	std::string _name;
};

template<typename T>
concept JobT = std::is_base_of_v<Job, T>;

class JobGraph {
	friend class JobNode;

public:
	explicit JobGraph(ThreadPool& pool) 
		: _pool(pool), _latch(0) {}
	~JobGraph();

	JobGraph(const JobGraph&) = delete;
	JobGraph& operator=(const JobGraph&) = delete;

	template<JobT T, typename... Args>
	JobNode* CreateNode(Args&&... args);

	void AutoDependencyBuild(const std::vector<LogicSystem*>& systems, float* dTRef);
	void AddManualDependency(LogicSystem* before, LogicSystem* after);
	void AddEventSystems(const std::vector<EventSystemBase*>& systems);

	void Run();

	const std::vector<Job*>& GetJobs() const { return _allocatedJobs; }

private:
	void Schedule(JobNode* node);
	void Build();
	void NotifyJobDone();

	void DumpDot(std::string_view fileName) const;

private:
	ThreadPool& _pool;
	std::vector<JobNode*> _nodes;
	std::vector<Job*> _allocatedJobs;
	std::latch _latch;
};

template<JobT T, typename... Args>
inline JobNode* JobGraph::CreateNode(Args&&... args)
{
	T* job = new T(std::forward<Args>(args)...);
	_allocatedJobs.push_back(job);

	JobData data{ T::Execute, job};
	JobNode* node = new JobNode(data, this);
	_nodes.push_back(node);

	return node;
}
