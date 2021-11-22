#include <catch2/catch_test_macros.hpp>
#include <registry.hpp>
#include <indexed_zipper.hpp>

TEST_CASE("indexed_zipper Iterate over single sparse_array", "[indexed_zipper]")
{
    ecs::containers::sparse_array<int> arr;

    for (int i = 0; i < 20; ++i) {
        if (!(i & 1))
            arr.emplace_at(i, 10);
    }
    ecs::containers::indexed_zipper zipper(arr);

    for (const auto &&[entity, component] : zipper) {
        if (entity & 1)
            FAIL();
        else
            SUCCEED();
    }
}

TEST_CASE("indexed_zipper Iterate over multiple sparse_array (no entity matching)", "[indexed_zipper]")
{
    ecs::containers::sparse_array<int> arr;
    ecs::containers::sparse_array<long> arr2;

    for (int i = 0; i < 20; ++i) {
        if (i & 1)
            arr.emplace_at(i, 10);
        else
            arr2.emplace_at(i, 10);
    }
    ecs::containers::indexed_zipper zipper(arr, arr2);

    for (const auto &&[entity, component1, component2] : zipper)
        FAIL();
}

TEST_CASE("indexed_zipper Iterate over multiple sparse_array (entity matching)", "[indexed_zipper]")
{
    ecs::containers::sparse_array<int> arr;
    ecs::containers::sparse_array<long> arr2;
    int n = 0;

    for (int i = 0; i < 20; ++i) {
        if (i & 1) {
            arr.emplace_at(i, 10);
            arr2.emplace_at(i, 10);
        }
    }
    ecs::containers::indexed_zipper zipper(arr, arr2);

    for (const auto &&[entity, component1, component2] : zipper)
        n++;
    if (n == 0)
        FAIL();
    else
        SUCCEED();
}
