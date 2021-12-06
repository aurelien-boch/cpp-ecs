cmake_minimum_required(VERSION 3.10)
project(cpp-ecs LANGUAGES CXX)

add_subdirectory(src)
add_subdirectory(include)

if(DEFINED TEST_ENABLE AND "${TEST_ENABLE}" STREQUAL "yes")
    message(STATUS "Unit testing enabled")
    add_subdirectory(tests/)
endif()

set(LIB_NAME ecs)
set(COMPILE_FLAGS)
set(LINK_LIBS)

add_library(
    ${LIB_NAME} SHARED
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

if(WIN32)
    list(APPEND COMPILE_FLAGS /W3)
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
elseif (UNIX)
    list(APPEND COMPILE_FLAGS -Wall -Wextra -Weffc++ -fPIC)
endif()

if(DEFINED COVERAGE_ENABLE AND "${COVERAGE_ENABLE}" STREQUAL "yes")
    message(STATUS "Coverage enabled")
    list(APPEND COMPILE_FLAGS -O0 -fprofile-arcs -ftest-coverage)
    list(APPEND LINK_LIBS gcov)
endif()

message(STATUS "Compiling with: ${COMPILE_FLAGS}")

target_compile_options(
    ${LIB_NAME}
        PRIVATE
        ${COMPILE_FLAGS}
)

target_link_libraries(
    ${LIB_NAME}
        PRIVATE
        ${LINK_LIBS}
)
