#include "core/graph.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include <numbers>
#include <type_traits>

static auto two_nodes() {
    return std::vector<std::tuple<std::string,double,double>>{
        {"A",48.85,2.35}, {"B",51.50,-0.12}};
}

TEST(GraphTest, CheckReturnEmptyGraphWhenNoNodesAndEdges) {
    auto g = astar::parse_graph({}, {});
    EXPECT_TRUE(g.nodes.empty());
    EXPECT_TRUE(g.adjacency.empty());
}

TEST(GraphTest, CheckReturnGraphContainsSameNodesAndEdges) {
    auto nodes = two_nodes();
    auto edges = std::vector<std::tuple<std::string, std::string, double>>{
        {"A", "B", 100.0}
    };
    auto g = astar::parse_graph(nodes, edges);

    auto gtest = astar::Graph{
        .nodes = {
            {"A", {"A", 48.85, 2.35}},
            {"B", {"B", 51.50, -0.12}}
        },
        .adjacency = {
            {"A", {{"B", 100.0}}}
        }
    };

    EXPECT_EQ(g, gtest);  
}

TEST(GraphTest, CheckReturnNothingWhenSearchNonExistingNode) {
    auto g = astar::parse_graph(two_nodes(),{});
    auto n = astar::find_node(g,"C");
    
    ASSERT_FALSE(n.has_value());
}

TEST(GraphTest, CheckReturnNothingWhenEmptyGraph) {
    auto g = astar::parse_graph({},{});
    auto n = astar::find_node(g,"A");
    
    ASSERT_FALSE(n.has_value());
}

TEST(GraphTest, CheckReturnGreatValuesWhenSearchExistingNode) {
    auto g = astar::parse_graph(two_nodes(),{});
    auto n = astar::find_node(g,"A");
    
    ASSERT_TRUE(n.has_value());
    EXPECT_DOUBLE_EQ(n->lat, 48.85);
    EXPECT_DOUBLE_EQ(n->lon,  2.35);
}