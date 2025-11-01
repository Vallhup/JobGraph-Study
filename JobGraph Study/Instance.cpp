#include "Instance.h"
#include "Job.h"

//void Instance::AddObject(std::unique_ptr<GameObject> obj)
//{
//	_objects.push_back(std::move(obj));
//}
//
//void Instance::BuildGraph()
//{
//	for (auto& obj : _objects)
//	{
//		for (auto& comp : obj->GetComponents())
//		{
//			auto* node = _graph.CreateNode<ComponentUpdateJob>(comp.get());
//			// TODO : 의존성이 생기면 의존성 추가 필요
//		}
//	}
//
//	_graph.Build();
//}
//
//void Instance::Tick(const float deltaTime)
//{
//	for (auto* job : _graph.GetJobs())
//	{
//		auto* compUpdateJob = static_cast<ComponentUpdateJob*>(job);
//		compUpdateJob->deltaTime = deltaTime;
//	}
//
//	_graph.Run();
//}
