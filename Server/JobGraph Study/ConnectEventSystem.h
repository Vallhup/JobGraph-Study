#pragma once

#include <iostream>
#include "ECS.h"
#include "System.h"
#include "Framework.h"
#include "Entity.h"
#include "Component.h"

class ConnectEventSystem : public EventSystemBase {
public:
	ConnectEventSystem(ECS& e, int p = 0) : EventSystemBase(e, p) {}

	virtual void ConsumeEvents() override
	{
		ConnectEvent ev;
		while (Framework::Get().eventQueue.connectQ.try_pop(ev))
			ProcessEvent(ev);
	}

	void ProcessEvent(const ConnectEvent& event)
	{
#ifdef _DEBUG
		std::cout << "[ConnectEventSystem] Player[" << event.sessionId << "] Connected\n";
#endif

		Entity entity = ecs.entityMng.CreatePlayer(event.sessionId);

		// TODO : Entity¿¡ Component Ãß°¡
		ecs.GetStorage<Transform>().AddComponent(entity);
		ecs.GetStorage<Velocity>().AddComponent(entity);
	}
};