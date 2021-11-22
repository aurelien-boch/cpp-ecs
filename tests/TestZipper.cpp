#include <catch2/catch_test_macros.hpp>
#include <registry.hpp>
#include <zipper.hpp>

TEST_CASE("zipper Iterate over single sparse_array", "[zipper]")
{
    ecs::containers::sparse_array<int> arr;

    for (int i = 0; i < 20; ++i) {
        if (!(i & 1))
            arr.emplace_at(i, i);
    }
    ecs::containers::zipper zipper(arr);

    for (const auto &&[e] : zipper) {
        if (e & 1)
            SUCCEED();
    }
}

TEST_CASE("zipper Iterate over multiple sparse_array (no entity matching)", "[zipper]")
{
    ecs::containers::sparse_array<int> arr;
    ecs::containers::sparse_array<long> arr2;

    for (int i = 0; i < 20; ++i) {
        if (i & 1)
            arr.emplace_at(i, 10);
        else
            arr2.emplace_at(i, 10);
    }
    ecs::containers::zipper zipper(arr, arr2);

    for (const auto &&[component1, component2] : zipper)
        FAIL();
}


TEST_CASE("zipper Iterate over multiple sparse_array (entity matching)", "[zipper]")
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
    ecs::containers::zipper zipper(arr, arr2);

    for (const auto &&[component1, component2] : zipper)
        n++;
    if (n == 0)
        FAIL();
    else
        SUCCEED();
}
