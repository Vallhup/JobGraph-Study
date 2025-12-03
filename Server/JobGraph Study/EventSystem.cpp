#include "EventSystem.h"

#include "Framework.h"
#include "Entity.h"
#include "Component.h"

#include "Protocol.hpp"
#include "Protocols/Protocol.pb.h"
#include "EventSystem.h"

EventSystem::EventSystem(ECS& e, int p) : System(e, p) 
{
	_handlers[EventType::EV_CONNECT] = [&](const Event& ev) { ProcessConnect(ev); };
	_handlers[EventType::EV_DISCONNECT] = [&](const Event& ev) { ProcessDisconnect(ev); };
	_handlers[EventType::EV_MOVE] = [&](const Event& ev) { ProcessMove(ev); };
}

void EventSystem::Execute(const float dT)
{
	Event ev;
	while (Framework::Get().eventQueue.try_pop(ev))
	{
		auto it = _handlers.find(ev.type);
		if (it != _handlers.end())
			it->second(ev);

#ifdef _DEBUG
		else
			std::cout << "[EventSystem] Unknown Type Event\n";
#endif
	}
}

void EventSystem::ProcessConnect(const Event& event)
{
	const auto* p = std::get_if<ConnectEvent>(&event.payload);
	if (!p) return;

#ifdef _DEBUG
	std::cout << "[EventSystem] Player[" << p->sessionId << "] Login\n";
#endif

	Entity entity = ecs.entityMng.CreatePlayer(p->sessionId);

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

void EventSystem::ProcessDisconnect(const Event& event)
{
}

void EventSystem::ProcessMove(const Event& event)
{
	const auto* p = std::get_if<MoveEvent>(&event.payload);
	if (!p) return;

	auto& velocity = ecs.GetStorage<Velocity>();

	for (const auto& [entity, vel] : velocity)
	{
		vel.inputX = p->inputX;
		vel.inputZ = p->inputZ;
		vel.yaw = p->yaw;
	}
}
