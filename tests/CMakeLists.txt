Include(FetchContent)

message(STATUS "Cloning Catch2")
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.0-preview3
)

FetchContent_MakeAvailable(Catch2)

set(LINK_LIBS Catch2::Catch2WithMain ecs)
set(COMPILE_OPTIONS)

if(DEFINED COVERAGE_ENABLE AND "${COVERAGE_ENABLE}" STREQUAL "yes")
    list(APPEND LINK_LIBS gcov)
    list(APPEND COMPILE_OPTIONS --coverage)
endif()

add_executable(
    libecs_tests
        TestRegistryEntities.cpp
        TestRegistryComponents.cpp
        TestComponentAlreadyRegisteredException.cpp
        TestComponentNotRegisteredException.cpp
)

target_link_libraries(
    libecs_tests
        ${LINK_LIBS}
)

target_compile_options(
    libecs_tests
        PRIVATE
        ${COMPILE_OPTIONS}
)

set_target_properties(
    libecs_tests
        PROPERTIES
        CXX_STANDARD 17
)

add_custom_command(TARGET libecs_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ecs> $<TARGET_FILE_DIR:libecs_tests>
    COMMENT "Copying libecs to test directory"
)
