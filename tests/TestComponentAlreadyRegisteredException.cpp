#include <catch2/catch_test_macros.hpp>
#include <exceptions/component_already_registered_exception.hpp>

struct velocityComponent
{
    int x;
    int y;
};

struct positionComponent
{
    int x;
    int y;
};

TEST_CASE("No components registered, component already registered exception", "[ComponentAlreadyRegisteredException]")
{
    std::vector<std::type_index> vector;
    ecs::exceptions::component_already_registered_exception exception(
        typeid(velocityComponent),
        vector);


#if (WIN32)
    REQUIRE(std::string(exception.what()) == "Component struct velocityComponent already registered. Registered components:");
#elif (__GNUC__)
    REQUIRE(std::string(exception.what()) == "Component velocityComponent already registered. Registered components:");
#endif
}

TEST_CASE("One component registered, component already registered exception", "[ComponentAlreadyRegisteredException]")
{
    std::vector<std::type_index> vector{std::type_index(typeid(positionComponent))};
    ecs::exceptions::component_already_registered_exception exception(
        typeid(velocityComponent),
        vector);

#if (WIN32)
    REQUIRE(
        std::string(exception.what()) == "Component struct velocityComponent already registered. Registered components:\n\t- struct positionComponent"
    );
#elif (__GNUC__)
    REQUIRE(
        std::string(exception.what()) == "Component velocityComponent already registered. Registered components:\n\t- positionComponent"
    );
#endif
}
