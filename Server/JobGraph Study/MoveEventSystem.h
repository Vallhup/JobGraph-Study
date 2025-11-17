#pragma once

#include <iostream>
#include "ECS.h"
#include "System.h"
#include "Framework.h"
#include "Entity.h"
#include "Component.h"

class MoveEventSystem : public EventSystemBase {
public:
	MoveEventSystem(ECS& e, int p = 0) : EventSystemBase(e, p) {}

	virtual void ConsumeEvents() override
	{
		MoveEvent ev;
		while (Framework::Get().eventQueue.moveQ.try_pop(ev))
			ProcessEvent(ev);
	}

	void ProcessEvent(const MoveEvent& event)
	{
#ifdef _DEBUG
		std::cout << "[MoveEventSystem] Player[" << event.sessionId << "] move";
#endif

		auto& ste = Framework::Get().sessionToEntity;

		auto it = ste.find(event.sessionId);
		if (it == ste.end()) return;

		Entity entity = it->second;

		auto* velComp = ecs.GetStorage<Velocity>().GetComponent(entity);
		velComp->dirX = event.dirX;
		velComp->dirZ = event.dirZ;
	}
};

