#include <catch2/catch_test_macros.hpp>
#include <registry.hpp>

struct velocityComponent
{
    int x;
    int y;
};

TEST_CASE("Add not registered component to entity", "[Registry Components]")
{
    ecs::registry r;
    auto entity = r.spawn_entity();

    REQUIRE_THROWS_AS(r.add_component<velocityComponent>(entity, {}), ecs::exceptions::component_not_registered_exception);
}

TEST_CASE("Add component to entity", "[Registry Components]")
{
    ecs::registry r;
    auto entity = r.spawn_entity();

    r.register_component<velocityComponent>();
    r.add_component<velocityComponent>(entity, {});
}

TEST_CASE("Emplace not registered component to entity", "[Registry Components]")
{
    ecs::registry r;
    auto entity = r.spawn_entity();

    REQUIRE_THROWS_AS(r.emplace_component<velocityComponent>(entity), ecs::exceptions::component_not_registered_exception);
}

TEST_CASE("Emplace component to entity", "[Registry Components]")
{
    ecs::registry r;
    auto entity = r.spawn_entity();

    r.register_component<velocityComponent>();
    r.emplace_component<velocityComponent>(entity);
}

TEST_CASE("Remove component from entity", "[Registry Components]")
{
    ecs::registry r;
    auto entity = r.spawn_entity();

    r.register_component<velocityComponent>();
    r.emplace_component<velocityComponent>(entity);
    r.remove_component<velocityComponent>(entity);
}

TEST_CASE("Remove not registered component from entity", "[Registry Components]")
{
    ecs::registry r;
    auto entity = r.spawn_entity();

    REQUIRE_THROWS_AS(r.remove_component<velocityComponent>(entity), ecs::exceptions::component_not_registered_exception);

}

TEST_CASE("Get component", "[Registry Components]")
{
    ecs::registry r;

    r.register_component<velocityComponent>();
    r.get_component<velocityComponent>();
}

TEST_CASE("Get const component", "[Registry Components]")
{
    ecs::registry r;

    r.register_component<velocityComponent>();
    [](ecs::registry const &registry) {
        registry.get_component<velocityComponent>();
    }(r);
}

TEST_CASE("Get not registered component", "[Registry Components]")
{
    ecs::registry r;

    REQUIRE_THROWS_AS(r.get_component<velocityComponent>(), ecs::exceptions::component_not_registered_exception);
}

TEST_CASE("Get const not registered component", "[Registry Components]")
{
    ecs::registry r;

    [](ecs::registry const &registry) {
        REQUIRE_THROWS_AS(
            registry.get_component<velocityComponent>(),
                ecs::exceptions::component_not_registered_exception);
    }(r);
}

TEST_CASE("Register component", "[Registry Components]")
{
    ecs::registry r;

    r.register_component<velocityComponent>();
}

TEST_CASE("Register already registered component", "[Registry Components]")
{
    ecs::registry r;

    r.register_component<velocityComponent>();
    REQUIRE_THROWS_AS(r.register_component<velocityComponent>(), ecs::exceptions::component_already_registered_exception);
}
