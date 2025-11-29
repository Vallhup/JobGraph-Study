#include "LoginEventSystem.h"

#include "Framework.h"
#include "Entity.h"
#include "Component.h"

#include "Protocol.hpp"
#include "Protocols/Protocol.pb.h"

void LoginEventSystem::ConsumeEvents()
{
	ConnectEvent ev;
	while (Framework::Get().eventQueue.connectQ.try_pop(ev))
		ProcessEvent(ev);
}

void LoginEventSystem::ProcessEvent(const ConnectEvent& event)
{
#ifdef _DEBUG
	std::cout << "[LoginEventSystem] Player[" << event.sessionId << "] Login\n";
#endif

	Entity entity = ecs.entityMng.CreatePlayer(event.sessionId);

	// TODO : Entity에 Component 추가
	ecs.GetStorage<Transform>().AddComponent(entity);
	ecs.GetStorage<Velocity>().AddComponent(entity);
	ecs.GetStorage<AnimationState>().AddComponent(entity);
	ecs.GetStorage<AnimationRef>().AddComponent(entity);
	ecs.GetStorage<Animator>().AddComponent(entity);
	ecs.GetStorage<Collider>().AddComponent(entity);

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
	Framework::Get().network.Broadcast(data2.data());

	// TODO : 새로 들어온 Session에게 기존 Session ADD Data Send
}