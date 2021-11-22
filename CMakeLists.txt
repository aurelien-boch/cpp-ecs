cmake_minimum_required(VERSION 3.10)
project(cpp-ecs LANGUAGES CXX)

add_subdirectory(src)
add_subdirectory(include)

if(DEFINED TEST_ENABLE AND ${TEST_ENABLE} STREQUAL "yes")
    message(STATUS "Unit testing enabled")
    add_subdirectory(tests/)
endif()

set(LIB_NAME ecs)

add_library(
    ${LIB_NAME}
        ${SRCS}
        ${INCLUDES}
)

set_target_properties(
    ${LIB_NAME}
        PROPERTIES
        CXX_STANDARD 17
)

target_include_directories(
    ${LIB_NAME}
        PUBLIC
        include/
)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(COMPILE_FLAGS  "/W3")
elseif (
    (CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR
    (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
)
    set(COMPILE_FLAGS -Wall -Wextra -Weffc++ -fPIC)
endif()

target_compile_options(
    ${LIB_NAME}
        PRIVATE
        ${COMPILE_FLAGS}
)
