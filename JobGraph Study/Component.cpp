#include "Component.h"

void TransformComponent::Update(const float deltaTime)
{
	x += 1.0f * deltaTime;
	y += 1.0f * deltaTime;

#ifdef _DEBUG
	std::cout << "[Transform] position: (" << x << ', ' << y << ")\n";
#endif
}