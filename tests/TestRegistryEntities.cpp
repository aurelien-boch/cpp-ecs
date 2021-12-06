#include <catch2/catch_test_macros.hpp>
#include <registry.hpp>

TEST_CASE("Spawn entity", "[Registry Entities]")
{
    ecs::registry r;
    auto entity = r.spawn_entity();

    REQUIRE(entity == 0);
}

TEST_CASE("Kill entity", "[Registry Entities]")
{
    ecs::registry r;
    auto entity = r.spawn_entity();

    r.kill_entity(entity);
}

TEST_CASE("Reuse killed entity", "[Registry Entities]")
{
    ecs::registry registry;
    auto entity = registry.spawn_entity();

    REQUIRE(entity == 0);
    registry.kill_entity(entity);
    entity = registry.spawn_entity();
    REQUIRE(entity == 0);
}

TEST_CASE("Generate entity from index", "[Registry Entities]")
{
    ecs::registry registry;
    auto entity = registry.entity_from_index(10);

    REQUIRE(entity == 10);
}

TEST_CASE("Generate entity from index, ensure there are no entity leak", "[Registry Entities]")
{
    ecs::registry registry;
    auto entity = registry.entity_from_index(10);
    auto standardEntity = registry.spawn_entity();

    REQUIRE(entity == 10);
    REQUIRE(standardEntity < 10);
}

TEST_CASE("Generate entity from index, already spawned entity", "[Registry Entities]")
{
    ecs::registry registry;
    auto entity = registry.spawn_entity();

    REQUIRE_THROWS_AS(registry.entity_from_index(0), std::runtime_error);
}
