#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include <vector>
#include <any>
#include <vector>
#include <functional>
#include <typeindex>
#include <stdexcept>
#include <exceptions/component_already_registered_exception.hpp>

#include "sparse_array.hpp"
#include "entity.hpp"
#include "exceptions/component_not_registered_exception.hpp"
#include "_internal.hpp"

//TODO unregister systems and components from the registry


namespace ecs
{
    /**
     * @brief This class refers to the Core of this ECS.
     * It will spawn and destroy entity, manage components and call registered systems.
     */
    class registry
    {
        public:
            ExportSymbol registry() noexcept;

            /**
             * @brief This method creates an entity. When entity is about to get destroyed,
             * kill_entity must be called.
             * @return The created entity.
             */
            [[nodiscard]] ExportSymbol entity spawn_entity() noexcept;

            /**
             * @brief This method created an entity from the given index.
             * If you add components to this entity, you must kill it with kill_entity, otherwise, you got nothing to do
             * @param [in] index This parameter refers to the index to be used to create entity.
             * @return The created entity.
             * @throw May throw a runtime_error "entity already spawned." if the given index is an already spawned
             * entity.
             */
            ExportSymbol entity entity_from_index(std::size_t index);

            /**
             * @brief This methods kills the given entity.
             * @param [in] e This parameter refers to the entity to kill.
             */
            ExportSymbol void kill_entity(entity const &e) noexcept;

            /**
             * @brief This method adds a component to the given entity.
             * @tparam Component This template refers to the component to add to the entity.
             * @param [in] entity This parameter refers to the entity to add the component to.
             * @param [in] value This parameter refers to the value to assign to the component using move.
             * @return A reference to the component contained in the sparse_array.
             * @throw If The component is not registered inside the registry,
             * the method will throw a component_not_registered_exception.
             */
            template <typename Component>
            ExportSymbol typename containers::sparse_array<Component>::reference_type add_component(
                entity const &entity,
                Component &&value)
            {
                try {
                    auto &res = _components.at(std::type_index(typeid(Component)));
                    auto &arr = std::any_cast<containers::sparse_array<Component> &>(res.first);

                    return arr.insert_at(entity, std::forward<Component>(value));
                } catch (std::out_of_range &) {
                    throw _generate_component_not_registered<Component>();
                }
            }

            /**
             * @brief This method constructs a component and adds it to the given entity.
             * @tparam Component This template refers to the component to emplace to the entity.
             * @tparam Params This variadic template refers to the type of the parameters to pass to the component's
             * constructor.
             * @param [in] entity This parameter refers to the entity to add the component to.
             * @param [in] p This parameter refers to the value of the parameters to pass to the component's constructor.
             * @return A reference to the component contained in the sparse_array.
             * @throw If the component is not registered into the registry, the function will throw a
             * component_not_registered_exception
             */
            template <typename Component, typename ... Params>
            ExportSymbol typename containers::sparse_array<Component>::reference_type emplace_component(
                entity const &entity,
                Params &&... p)
            {
                try {
                    auto &res = _components.at(std::type_index(typeid(Component)));
                    auto &arr = std::any_cast<containers::sparse_array<Component> &>(res.first);

                    return arr.emplace_at(entity, std::forward<Params>(p)...);
                } catch (std::out_of_range &) {
                    throw _generate_component_not_registered<Component>();
                }
            }

            /**
             * @brief This method removes a component from an entity.
             * @tparam Component This template refers to the component to remove from the entity.
             * @param [in] entity This parameter refers to the entity to remove the component from.
             * @throw If the component is not registered into the registry, the function will throw a
             * component_not_registered_exception
             */
            template <typename Component>
            ExportSymbol void remove_component(entity const &entity)
            {
                try {
                    auto res = _components.at(std::type_index(typeid(Component)));

                    res.second(*this, entity);
                } catch (std::out_of_range &) {
                    throw _generate_component_not_registered<Component>();
                }
            }

            /**
             * @brief This method registers a component into the registry.
             * @tparam Component This template refers to the component to register into the registry
             * @return A reference to the sparse_array of components.
             * @throw todo, then
             */
            template <class Component>
            ExportSymbol containers::sparse_array<Component> &register_component()
            {
                 auto [it, res] = _components.try_emplace(
                    std::type_index(std::type_index(typeid(Component))),
                    containers::sparse_array<Component>(),
                    [](registry &r, entity const &other) {
                        auto &component = r.get_component<Component>();

                        if (component.size() > other)
                            component[other] = std::nullopt;
                    }
                );

                if (!res)
                    throw _generate_component_already_registered<Component>();
                return std::any_cast<containers::sparse_array<Component> &>(it->second.first);
            }

            /**
             * @brief This method gets a sparse_array of a given component.
             * @tparam Component This template refers to the component type to get.
             * @return A reference to a sparseArray of Component.
             * @throw If the component is not registered into the registry, the function will throw a
             * component_not_registered_exception
             */
            template <class Component>
            [[nodiscard]] ExportSymbol containers::sparse_array<Component> &get_component()
            {
                try {
                    auto &component = _components.at(std::type_index(typeid(Component)));

                    return std::any_cast<containers::sparse_array<Component> &>(component.first);
                } catch (std::out_of_range &) {
                    throw _generate_component_not_registered<Component>();
                }
            }

            /**
             * @brief This method gets a sparse_array of a given component.
             * @tparam Component This template refers to the component type to get.
             * @return A constant reference to a sparseArray of Component.
             * @throw If the component is not registered into the registry, the function will throw a
             * component_not_registered_exception
             */
            template <class Component>
            [[nodiscard]] ExportSymbol containers::sparse_array<Component> const &get_component() const
            {
                try {
                    auto const &component = _components.at(std::type_index(typeid(Component)));

                    return std::any_cast<containers::sparse_array<Component> const &>(component.first);
                } catch (std::out_of_range &) {
                    throw _generate_component_not_registered<Component>();
                }
            }

            /**
             * @brief This method registers a system into the registry by moving it.
             * @tparam Components This variadic template refers to the components to be used by the system.
             * @tparam Function This template refers to the type of the system (it MUST implement the () operator).
             * @param [in] f This parameter refers to an rvalue reference to the system.
             */
            template <class ... Components, typename Function>
            ExportSymbol void add_system(Function &&f) noexcept
            {
                _systems.emplace_back([f = std::forward<Function>(f)](registry &r, double deltaTime) {
                    f(r, deltaTime, r.get_component<Components>()...);
                });
            }

            /**
             * @brief This method registers a system into the registry by copying the target of the reference.
             * @tparam Components This variadic template refers to the components to be used by the system.
             * @tparam Function This template refers to the type of the system (it MUST implement the () operator).
             * @param [in] f This parameter refers to an reference to the system.
             */
            template <class ... Components , typename Function>
            ExportSymbol void add_system(Function const &f) noexcept
            {
                _systems.emplace_back([f](registry &r, double deltaTime) {
                    f(r, deltaTime, r.get_component<Components>()...);
                });
            }

            /**
             * @brief This method runs all systems registered into the registry.
             */
            ExportSymbol void run_systems(double deltaTime);

        private:
            using entity_eraser = std::function<void (registry &, entity const &)>;

            std::unordered_map<
                std::type_index,
                std::pair<std::any, entity_eraser>
            > _components;

            std::vector<std::function<void (registry &r, double deltaTime)>> _systems;

            std::size_t _spawnedEntities;

            std::vector<std::size_t> _freedEntities{};

            template <class Component>
            [[nodiscard]] ExportSymbol exceptions::component_not_registered_exception _generate_component_not_registered() const
            {
                std::vector<std::type_index> indexes;

                for (auto const &e : _components)
                    indexes.emplace_back(e.first);
                return {
                    typeid(Component),
                    indexes
                };
            }

            template <class Component>
            [[nodiscard]] ExportSymbol exceptions::component_already_registered_exception _generate_component_already_registered() const
            {
                std::vector<std::type_index> indexes;

                for (auto const &e : _components)
                    indexes.emplace_back(e.first);
                return {
                    typeid(Component),
                    indexes
                };
            }

    };
}

#endif //REGISTRY_HPP
