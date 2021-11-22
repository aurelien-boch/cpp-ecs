#include <catch2/catch_test_macros.hpp>
#include <registry.hpp>

TEST_CASE("Begin", "[sparse_array]")
{
    ecs::containers::sparse_array<int> arr;

    arr.insert_at(0, 15);
    arr.insert_at(1, 20);
    REQUIRE(*arr.begin() == 15);
}

TEST_CASE("End", "[sparse_array]")
{
    ecs::containers::sparse_array<int> arr;

    arr.insert_at(0, 15);
    arr.insert_at(1, 20);
    REQUIRE(arr.begin() + 2 == arr.end());
}

TEST_CASE("Copy insert", "[sparse_array]")
{
    ecs::containers::sparse_array<int> arr;

    arr.insert_at(2, 1);
    REQUIRE(arr[2] == 1);
}

TEST_CASE("Perfect forward insert", "[sparse_array]")
{
    ecs::containers::sparse_array<int> arr;
    int n = 1;

    arr.insert_at(2, std::move(n));
    REQUIRE(arr[2] == 1);
}

TEST_CASE("getIndex finding", "[sparse_array]")
{
    ecs::containers::sparse_array<int> arr;

    for (int i = 0; i < 10; ++i)
        arr.emplace_at(i, i);
    auto &data = arr[5];
    REQUIRE(arr.getIndex(data) == 5);
}

TEST_CASE("getIndex not finding", "[sparse_array]")
{
    ecs::containers::sparse_array<int> arr;

    for (int i = 0; i < 10; ++i)
        arr.emplace_at(i, i);
    std::optional<int> data = 5;
    REQUIRE_THROWS_AS(arr.getIndex(data), std::out_of_range);
}


TEST_CASE("Emplace", "[sparse_array]")
{
    ecs::containers::sparse_array<int> arr;

    arr.emplace_at(2, 1);
    REQUIRE(arr[2] == 1);
}
