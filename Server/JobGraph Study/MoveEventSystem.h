#pragma once

#include <iostream>
#include "ECS.h"
#include "System.h"
#include "Framework.h"
#include "Entity.h"
#include "Component.h"

#include "Protocol.hpp"
#include "Protocols/Protocol.pb.h"

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
		auto& velocity = ecs.GetStorage<Velocity>();

		for (const auto& [entity, vel] : velocity)
		{
			vel.inputX = event.inputX;
			vel.inputZ = event.inputZ;
			vel.yaw = event.yaw;
		}
	}
};

