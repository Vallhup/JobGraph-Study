#include "JobGraph.h"
#include "ThreadPool.h"

/*--------------------[ JobNode ]--------------------*/

void JobNode::AddDependency(JobNode* dependency)
{
	_deps.fetch_add(1);
	dependency->_dependents.push_back(this);
}

void JobNode::Execute()
{
	_job.func(_job.context);
	for (auto* dep : _dependents)
	{
		if (dep->_deps.fetch_sub(1) == 1)
			_graph->Schedule(dep);
	}

	_graph->NotifyJobDone();
}

/*--------------------[ JobGraph ]--------------------*/

JobGraph::~JobGraph()
{
	for (IJob* job : _allocatedJobs)
		delete job;

	for (JobNode* node : _nodes)
		delete node;
}

void JobGraph::Schedule(JobNode* node)
{
	JobData job{
		[](void* ctx) -> void
		{
			JobNode* n = static_cast<JobNode*>(ctx);
			n->Execute();
		}, node };

	_pool.Push(job);
}

void JobGraph::Build()
{
	if (_latch) delete _latch;
	_latch = new std::latch(static_cast<int>(_nodes.size()));

	for (JobNode* node : _nodes)
	{
		if (node->Ready())
			Schedule(node);
	}

	_latch->wait();

	delete _latch;
	_latch = nullptr;
}

void JobGraph::NotifyJobDone()
{
	if (_latch)
		_latch->count_down();
}
