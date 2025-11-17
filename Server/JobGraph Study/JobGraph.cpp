#include "JobGraph.h"
#include "ThreadPool.h"
#include <fstream>

#include <ranges>

/*--------------------[ JobNode ]--------------------*/

void JobNode::AddDependency(JobNode* dependency)
{
	_deps.fetch_add(1);
	dependency->_dependents.push_back(this);
}

void JobNode::Execute()
{
	bool expected{ false };
	if (_done.compare_exchange_strong(expected, true)) 
	{
		_job.func(_job.context);

		for (JobNode* dep : _dependents)
		{
			if (dep->_deps.fetch_sub(1) == 1)
				_graph->Schedule(dep);
		}

		_graph->NotifyJobDone();
	}
}

/*--------------------[ JobGraph ]--------------------*/

JobGraph::~JobGraph()
{
	for (Job* job : _allocatedJobs)
		delete job;

	for (JobNode* node : _nodes)
		delete node;
}

void JobGraph::AutoDependencyBuild(const std::vector<LogicSystem*>& systems, float* dTRef)
{
	std::unordered_map<LogicSystem*, JobNode*> nodeMap;

	for (auto& sys : systems)
	{
		JobNode* node = CreateNode<LogicSystemJob>(sys, dTRef);
		node->_layer = JobLayer::LOGIC;
		node->_name = typeid(*sys).name();
		nodeMap[sys] = node;
	}

	// TODO : O(N^2) 순회 
	//     -> 나중에 시간나면 고치기... 어짜피 런타임에 돌아갈 함수 아님
	for (auto& A : systems)
	{
		for (auto& B : systems)
		{
			if (A == B) continue;

			bool conflict{ false };
			for (auto& write : A->WriteComponents())
			{
				const auto& bReads = B->ReadComponents();
				const auto& bWrites = B->WriteComponents();

				if(std::find(bReads.begin(), bReads.end(), write) != bReads.end()
					or std::find(bWrites.begin(), bWrites.end(), write) != bWrites.end())
				{
					conflict = true;
					break;
				}
			}

			if (conflict)
			{
				int priA = A->GetPriority();
				int priB = B->GetPriority();

				if (priA == priB)
				{
					if (A < B)
						nodeMap[B]->AddDependency(nodeMap[A]);

					else
						nodeMap[A]->AddDependency(nodeMap[B]);
				}

				else if (priA < priB)
					nodeMap[B]->AddDependency(nodeMap[A]);

				else
					nodeMap[A]->AddDependency(nodeMap[B]);
			}
		
#ifdef _DEBUG
			if (conflict)
			{
				std::cout << "[Dependency] "
					<< typeid(*A).name() << " -> "
					<< typeid(*B).name() << std::endl;
			}
#endif
		}
	}

	Build();
}

void JobGraph::AddManualDependency(LogicSystem* before, LogicSystem* after)
{
	auto beforeIt = std::find_if(_nodes.begin(), _nodes.end(), 
		[&](auto* n) 
		{
			auto* job = static_cast<LogicSystemJob*>(n->_job.context);
			return job->system == before;
		});

	auto afterIt = std::find_if(_nodes.begin(), _nodes.end(), 
		[&](auto* n) 
		{
			auto* job = static_cast<LogicSystemJob*>(n->_job.context);
			return job->system == after;
		});

	if (beforeIt != _nodes.end() && afterIt != _nodes.end())
		(*afterIt)->AddDependency(*beforeIt);
}

void JobGraph::AddEventSystems(const std::vector<EventSystemBase*>& systems)
{
	for (auto* system : systems)
	{
		auto* node = CreateNode<EventSystemJob>(system);
		node->_layer = JobLayer::EVENT;
		node->_name = typeid(*system).name();
	}
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
	for (auto* A : _nodes)
	{
		for (auto* B : _nodes)
		{
			if (A == B) continue;
			if (A->_layer == JobLayer::EVENT and
				B->_layer == JobLayer::LOGIC)
				B->AddDependency(A);
		}
	}

	for (JobNode* node : _nodes)
		node->_initDeps = node->_deps;

#ifdef _DEBUG
	DumpDot("graph.gv");
	for (JobNode* node : _nodes)
	{
		std::cout << "deps = " << node->_deps.load()
			<< (node->Ready() ? " [READY]" : " [WAIT]") << std::endl;
	}
#endif
}

void JobGraph::Run()
{
	new (&_latch) std::latch(static_cast<int>(_nodes.size()));

	for (JobNode* node : _nodes)
		node->ResetDeps();

	for (JobNode* node : _nodes)
	{
		if (node->Ready())
			Schedule(node);
	}

	_latch.wait();
}

void JobGraph::NotifyJobDone()
{
	_latch.count_down();
}

void JobGraph::DumpDot(std::string_view filename) const
{
	std::ofstream file(filename.data());
	file << "digraph JobGraph {\n";
	for (auto* node : _nodes)
	{
		for (auto* dep : node->Dependents())
			file << "  \"" << node->_name << "\" -> \"" << dep->_name << "\";\n";
		if (node->Dependents().empty())
			file << "  \"" << node->_name << "\";\n";
	}
	file << "}\n";
	file.close();

	std::cout << "[JobGraph] Dumped to " << filename << std::endl;
}