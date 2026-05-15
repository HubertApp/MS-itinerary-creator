cmake_minimum_required(VERSION 3.14)
project(AStarCore)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_language(ASM)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(astar_engine 
    src/engine/math_utils.cpp
    include/core/math_utils.hpp
    src/engine/graph.cpp
)

target_include_directories(astar_engine PUBLIC src/engine include)

add_executable(unit_tests 
    tests/test_math_logique.cpp
    tests/test_graph.cpp
)

target_link_libraries(unit_tests PRIVATE astar_engine gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)