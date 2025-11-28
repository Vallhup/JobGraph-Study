#pragma once

#include <iostream>
#include "ECS.h"
#include "System.h"
#include "Framework.h"
#include "Entity.h"
#include "Component.h"

#include "Protocol.hpp"
#include "Protocols/Protocol.pb.h"

class LoginEventSystem : public EventSystemBase {
public:
	LoginEventSystem(ECS& e, int p = 0) : EventSystemBase(e, p) {}

	virtual void ConsumeEvents() override
	{
		ConnectEvent ev;
		while (Framework::Get().eventQueue.connectQ.try_pop(ev))
			ProcessEvent(ev);
	}

	void ProcessEvent(const ConnectEvent& event)
	{
#ifdef _DEBUG
		std::cout << "[LoginEventSystem] Player[" << event.sessionId << "] Login\n";
#endif

		Entity entity = ecs.entityMng.CreatePlayer(event.sessionId);

		// TODO : Entity¿¡ Component Ãß°¡
		ecs.GetStorage<Transform>().AddComponent(entity);
		ecs.GetStorage<Velocity>().AddComponent(entity);

		// TEMP : Login Packet Send
		int sessionId;
		auto& ets = Framework::Get().entityToSession;
		auto it = ets.find(entity);
		if (it != ets.end())
			sessionId = it->second;
		else
			return;

		Protocol::SC_LOGIN_PACKET login;
		login.set_sessionid(sessionId);

		SendBuffer data = PacketFactory::Serialize<Protocol::SC_LOGIN_PACKET>(
			PacketType::SC_LOGIN, login);
		Framework::Get().network.Send(sessionId, data.data());

		Protocol::SC_ADD_PACKET add;
		add.set_sessionid(sessionId);
		add.set_x(0);
		add.set_y(0);
		add.set_z(0);

		SendBuffer data2 = PacketFactory::Serialize<Protocol::SC_ADD_PACKET>(
			PacketType::SC_ADD, add);
		Framework::Get().network.Send(sessionId, data2.data());
	}
};