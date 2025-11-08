#pragma once

#include <vector>
#include <queue>

struct Entity {
	int id;
	int generation;
	int instanceId;

	Entity(int id = -1, int g = 0, int inst = 0) 
		: id(id), generation(g), instanceId(inst) {}
};

class EntityManager {
public:
	Entity Create();

	void Destroy(Entity entity);
	bool IsAlive(Entity entity) const;

private:
	std::vector<int> _generations;
	std::queue<int> _freeIds;
};

