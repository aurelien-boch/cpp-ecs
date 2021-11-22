#include <catch2/catch_test_macros.hpp>
#include <registry.hpp>

struct A {};

TEST_CASE("Systems run", "[Systems]")
{
    ecs::registry registry;
    auto entity = registry.spawn_entity();

    registry.register_component<A>();
    registry.emplace_component<A>(entity);
    registry.add_system<A>([](ecs::registry &, double dt, ecs::containers::sparse_array<A> &)
    {
        SUCCEED();
    });
    registry.run_systems(0);
}
