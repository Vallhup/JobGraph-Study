#include "OutputEventSystem.h"
#include "Framework.h"

OutputEventSystem::OutputEventSystem(ECS& ecs, int p) : System(ecs, p)
{
	_handlers[DirtyType::Spawned] = [&](const OutputEvent& ev) { ProcessSpawn(ev); };
	_handlers[DirtyType::Moved] = [&](const OutputEvent& ev) { ProcessMove(ev); };
}

void OutputEventSystem::Execute(const float dT)
{
	OutputEvent ev;
	while (Framework::Get().outEventQueue.try_pop(ev))
	{
		auto it = _handlers.find(ev.type);
		if (it != _handlers.end())
			it->second(ev);

#ifdef _DEBUG
		else
			std::cout << "[OutputEventSystem] Unknown Type Event\n";
#endif
	}
}

void OutputEventSystem::ProcessSpawn(const OutputEvent& event)
{

}

void OutputEventSystem::ProcessMove(const OutputEvent& event)
{

}
