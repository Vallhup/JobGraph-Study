#pragma once

#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>

class Component;

template<typename T>
concept ComponentT = std::is_base_of_v<Component, T>;

class GameObject {
public:
	template<ComponentT T, typename... Args>
	T* AddComponent(Args&&... args)
	{
		if (GetComponent<T>()) {
			return nullptr;
		}

		auto component = std::make_unique<T>(*this, std::forward<Args>(args)...);
		T* raw = component.get();

		_types[std::type_index(typeid(T))] = raw;
		_components.push_back(std::move(component));

		return raw;
	}

	template<ComponentT T>
	T* GetComponent() const
	{
		auto it = _types.find(std::type_index(typeid(T)));
		if (it != _types.end()) {
			return static_cast<T*>(it->second);
		}

		return nullptr;
	}

	void Update(const float deltaTime);

private:
	std::vector<std::unique_ptr<Component>> _components;
	std::unordered_map<std::type_index, Component*> _types;
};
