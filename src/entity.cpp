#include "entity.hpp"

ecs::entity::operator std::size_t() const noexcept
{
    return _id;
}

ecs::entity::entity(std::size_t id) noexcept :
    _id(id)
{}
