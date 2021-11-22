Include(FetchContent)

message(STATUS "Cloning Catch2")
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.0-preview3
)

FetchContent_MakeAvailable(Catch2)

add_executable(
    libecs_tests
        TestRegistryEntities.cpp
        TestRegistryComponents.cpp
        TestRegistrySystems.cpp
        TestSparseArray.cpp
        TestZipper.cpp
        TestIndexedZipper.cpp
)

target_link_libraries(
    libecs_tests
        Catch2::Catch2WithMain
        ecs
)

set_target_properties(
    libecs_tests
        PROPERTIES
        CXX_STANDARD 17
)