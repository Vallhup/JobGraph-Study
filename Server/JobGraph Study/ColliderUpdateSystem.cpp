#include "ColliderUpdateSystem.h"
#include "Math.h"

void ColliderUpdateSystem::Update(const float dT)
{
	auto& transforms = ecs.GetStorage<Transform>();
	auto& colliders = ecs.GetStorage<Collider>();

	for (const auto& [entity, collider] : colliders)
	{
		auto* trans = transforms.GetComponent(entity);
		if (!trans) continue;

		XMMATRIX world = TransformHelper::ToMatrix(*trans);

		collider.worldCapsules.resize(collider.localCapsules.size());
		for (size_t i = 0; i < collider.localCapsules.size(); ++i)
		{
			const Capsule& localCapsule = collider.localCapsules[i];
			Capsule worldCapsule = localCapsule;

			XMVECTOR p0 = XMVector3Transform(localCapsule.P0(), world);
			XMVECTOR p1 = XMVector3Transform(localCapsule.P1(), world);

			XMStoreFloat3(&worldCapsule.p0, p0);
			XMStoreFloat3(&worldCapsule.p1, p1);

			worldCapsule.radius *= 0.01f;
			collider.worldCapsules[i] = worldCapsule;
		}
	}
}