#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cstdlib>

#ifdef WIN32
    #define ExportSymbol   __declspec( dllexport )
#else
    #define ExportSymbol
#endif

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

            ExportSymbol entity(entity const &other) noexcept = default;
            ExportSymbol entity(entity &&other) noexcept = default;
            ExportSymbol entity &operator=(entity const &other) noexcept = default;
            ExportSymbol entity &operator=(entity &&other) noexcept= default;

            ExportSymbol ~entity() = default;

            ExportSymbol operator std::size_t() const noexcept;

        private:
            ExportSymbol explicit entity(std::size_t id) noexcept;

            std::size_t _id;
    };
}

#endif //ENTITY_HPP
