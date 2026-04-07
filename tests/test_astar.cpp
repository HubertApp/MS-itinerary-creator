#include <catch2/catch_test_macros.hpp>
#include "../src/engine/astar.h"

TEST_CASE("A* Logic - Functional", "[astar]") {
    auto result = astar::compute_path({0,0}, {0,2});
    REQUIRE(result.size() == 3);
}
