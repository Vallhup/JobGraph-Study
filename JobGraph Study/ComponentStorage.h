#pragma once

#include <unordered_map>
#include "Entity.h"

class Component;

template<typename T>
concept CompT = std::is_base_of_v<Component, T>;

template<CompT T>
class ComponentStorage {
public:
	T* AddComponent(Entity entity)
	{
		auto [it, inserted] = _comps.try_emplace(entity.id, T{});
		return &(it->second);
	}

	T* GetComponent(Entity entity)
	{
		auto it = _comps.find(entity.id);
		return it != _comps.end() ? &(it->second) : nullptr;
	}

	void RemoveComponent(Entity entity)
	{
		_comps.erase(entity.id);
	}

	std::unordered_map<int, T>& GetComponents() 
	{ 
		return _comps; 
	}

private:
	std::unordered_map<int, T> _comps;
};

