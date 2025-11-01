#pragma once

#include <vector>
#include <queue>

struct Entity {
	int id;
	int generation;
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

