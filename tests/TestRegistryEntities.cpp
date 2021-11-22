#include <catch2/catch_test_macros.hpp>
#include <registry.hpp>

TEST_CASE("entity spawning", "[Entities]")
{
    ecs::registry registry;
    auto entity = registry.spawn_entity();

    REQUIRE(entity == 0);
}

TEST_CASE("entity kill and respawn", "[Entities]")
{
    ecs::registry registry;
    auto entity = registry.spawn_entity();

    REQUIRE(entity == 0);
    registry.kill_entity(entity);
    entity = registry.spawn_entity();
    REQUIRE(entity == 0);
}
