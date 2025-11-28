#include "ECS.h"
#include "Framework.h"
#include "MovementSystem.h"

#include "Protocol.hpp"
#include "Protocols/Protocol.pb.h"

void MovementSystem::Update(const float dT)
{
	auto& transforms = ecs.GetStorage<Transform>();
	auto& velocitys = ecs.GetStorage<Velocity>();
	auto& framework = Framework::Get();

	for (const auto& [entity, transform] : transforms)
	{
		if (auto* vel = velocitys.GetComponent(entity))
		{
			/*XMVECTOR dir = XMLoadFloat3(&vel->direction);
			float lenSq = XMVectorGetX(XMVector3LengthSq(dir));
			if (lenSq < 1e-6f) continue;

			dir = XMVector3Normalize(dir);

			const float speed = 2.0f;
			XMVECTOR pos = XMLoadFloat3(&transform.position);
			pos = XMVectorAdd(pos, XMVectorScale(dir, speed * dT));
			XMStoreFloat3(&transform.position, pos);*/

			auto sit = framework.entityToSession.find(entity);
			if (sit == framework.entityToSession.end()) 
				continue;
			int sessionId = sit->second;
			auto pit = framework.playerStates.find(sessionId);
			if (pit == framework.playerStates.end()) 
				continue;
			PlayerState* ps = pit->second.get();

			printf("%d\n", ps->lastSeq.load());

			int inputX = ps->inputX;
			int inputZ = ps->inputZ;
			float yaw = ps->yaw;
			int lastSeq = ps->lastSeq;

			XMVECTOR forward = XMVectorSet(sin(yaw), 0, cos(yaw), 0);
			XMVECTOR right = XMVector3Cross(XMVectorSet(0, 1, 0, 0), forward);

			XMVECTOR dir = XMVectorAdd(
				XMVectorScale(forward, inputZ),
				XMVectorScale(right, inputX)
			);

			if (XMVectorGetX(XMVector3LengthSq(dir)) > 1e-6f)
				dir = XMVector3Normalize(dir);
			else
				dir = XMVectorZero();

			const float speed = 2.0f;
			XMVECTOR pos = XMLoadFloat3(&transform.position);
			pos = XMVectorAdd(pos, XMVectorScale(dir, speed * dT));
			XMStoreFloat3(&transform.position, pos);

			XMVECTOR q = XMQuaternionRotationRollPitchYaw(0, yaw, 0);
			XMStoreFloat4(&transform.rotation, q);

			// TEMP : 이동 패킷 Send

			Protocol::SC_MOVE_PACKET move;
			move.set_sessionid(sessionId);
			move.set_x(transform.position.x);
			move.set_y(transform.position.y);
			move.set_z(transform.position.z);
			move.set_yaw(yaw);

			SendBuffer data = PacketFactory::Serialize<Protocol::SC_MOVE_PACKET>(
				PacketType::SC_MOVE_OBJECT, move);
			Framework::Get().network.Send(sessionId, data.data());
		}
	}
}