#include "GameObject.h"
#include "Component.h"

void GameObject::Update(const float deltaTime)
{
	for (auto& c : _components)
		c->Update(deltaTime);
}
