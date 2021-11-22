#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cstdlib>

namespace ecs
{
    class registry;

    /**
     * @class entity
     * @brief This class refers to an entity that will be used in the registry.
     * It will be bound to components and used by systems.
     */
    class entity
    {
        public:
            friend registry;

            entity(entity const &other) noexcept = default;
            entity(entity &&other) noexcept = default;
            entity &operator=(entity const &other) noexcept = default;
            entity &operator=(entity &&other) noexcept= default;

            ~entity() = default;

            operator std::size_t() const noexcept;

        private:
            explicit entity(std::size_t id) noexcept;

            std::size_t _id;
    };
}

#endif //ENTITY_HPP
