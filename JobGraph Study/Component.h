#pragma once

class GameObject;

class Component {
public:
	virtual ~Component() = default;
	virtual void Update(const float deltaTime) = 0;

protected:
	Component(GameObject* owner) : _owner(owner) {}

	GameObject* _owner;
};

class TransformComponent : public Component {
public:
	explicit TransformComponent(GameObject* owner)
		: Component(owner), x(0), y(0) {}
	virtual ~TransformComponent() = default;

	virtual void Update(const float deltaTime) override;

private:
	float x;
	float y;
};