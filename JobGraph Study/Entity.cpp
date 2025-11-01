#include "Entity.h"

Entity EntityManager::Create()
{
    int id;

    if (not _freeIds.empty())
    {
        id = _freeIds.front();
        _freeIds.pop();
    }

    else
    {
        id = static_cast<int>(_generations.size());
        _generations.push_back(0);
    }

    return Entity{ id, _generations[id] };
}

void EntityManager::Destroy(Entity entity)
{
    const int id = entity.id;
    if (id >= _generations.size()) return;

    ++_generations[id];
    _freeIds.push(id);
}

bool EntityManager::IsAlive(Entity entity) const
{
    return entity.id < _generations.size() and
        _generations[entity.id] == entity.generation;
}
