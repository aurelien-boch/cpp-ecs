#include "registry.hpp"

namespace ecs
{
    registry::registry() noexcept :
        _components{},
        _systems{},
        _spawnedEntities(0)
    {}

    entity registry::spawn_entity() noexcept
    {
        const std::size_t num = _freedEntities.empty() ? _spawnedEntities++ : _freedEntities.back();

        if (!_freedEntities.empty())
            _freedEntities.pop_back();
        return entity(num);
    }

    entity registry::entity_from_index(std::size_t index)
    {
        if (index < _spawnedEntities) {
            auto it = std::find(_freedEntities.begin(), _freedEntities.end(), index);

            if (it == _freedEntities.end())
                throw std::runtime_error("entity already spawned");
            else
                _freedEntities.erase(it);
        } else
            for (std::size_t i = _spawnedEntities; i < index; ++i)
                _freedEntities.emplace_back(i);
        _spawnedEntities = index;
        return entity(index);
    }

    void registry::kill_entity(entity const &e) noexcept
    {
        _freedEntities.emplace_back(e);

        for (auto &[key, value] : _components)
            value.second(*this, e);
    }

    void registry::run_systems(double deltaTime)
    {
        for (auto const &system : _systems)
            system(*this, deltaTime);
    }
}