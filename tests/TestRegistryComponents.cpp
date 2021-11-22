#include <catch2/catch_test_macros.hpp>
#include <registry.hpp>

struct velocity {
    int x;
    int y;

    velocity(int x, int y) : x(x), y(y) {};
};

TEST_CASE("Register component", "[Components]")
{
    ecs::registry registry;

    REQUIRE_NOTHROW(registry.register_component<velocity>());
}

TEST_CASE("Emplace component to entity", "[Components]")
{

    ecs::registry registry;
    auto entity = registry.spawn_entity();

    registry.register_component<velocity>();
    REQUIRE_NOTHROW(registry.emplace_component<velocity>(entity, 1, 2));
}

TEST_CASE("Emplace component to entity that already has the same component", "[Components]")
{
    ecs::registry registry;
    auto entity = registry.spawn_entity();

    registry.register_component<velocity>();
    registry.emplace_component<velocity>(entity, 1, 2);
    REQUIRE_NOTHROW(registry.emplace_component<velocity>(entity, 1, 2));
}

TEST_CASE("Add component to entity", "[Components]")
{

    ecs::registry registry;
    auto entity = registry.spawn_entity();

    registry.register_component<velocity>();
    REQUIRE_NOTHROW(registry.add_component<velocity>(entity, {1, 2}));
}

TEST_CASE("Add component to entity that already has the same component", "[Components]")
{
    ecs::registry registry;
    auto entity = registry.spawn_entity();

    registry.register_component<velocity>();
    registry.add_component<velocity>(entity, {1, 2});
    REQUIRE_NOTHROW(registry.add_component<velocity>(entity, {1, 2}));
}

TEST_CASE("Remove component from entity with emplaced component", "[Components]")
{
    ecs::registry registry;
    auto entity = registry.spawn_entity();

    registry.register_component<velocity>();
    registry.emplace_component<velocity>(entity, 1, 2);
    REQUIRE_NOTHROW(registry.remove_component<velocity>(entity));
}

TEST_CASE("Remove component from entity with added component", "[Components]")
{
    ecs::registry registry;
    auto entity = registry.spawn_entity();

    registry.register_component<velocity>();
    registry.add_component<velocity>(entity, {1, 2});
    REQUIRE_NOTHROW(registry.remove_component<velocity>(entity));
}

TEST_CASE("Remove component from entity that does not have the component", "[Components]")
{
    ecs::registry registry;
    auto entity = registry.spawn_entity();

    registry.register_component<velocity>();
    REQUIRE_NOTHROW(registry.remove_component<velocity>(entity));
}

TEST_CASE("Get component", "[Components]")
{
    ecs::registry registry;

    registry.register_component<velocity>();
    auto &res = registry.get_component<velocity>();

    REQUIRE(std::is_same<decltype(res), ecs::containers::sparse_array<velocity> &>::value);
}

TEST_CASE("Get not existing component", "[Components]")
{
    ecs::registry registry;

    registry.register_component<velocity>();
    REQUIRE_THROWS_AS(registry.get_component<int>(), ecs::exceptions::component_not_registered_exception);
}
