#include "core/graph.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include <numbers>
#include <type_traits>

static auto two_nodes() {
  return std::vector<std::tuple<std::string, double, double>>{
      {"A", 48.85, 2.35}, {"B", 51.50, -0.12}};
}

TEST(GraphTestParseGraph, CheckReturnEmptyGraphWhenNoNodesAndEdges) {
  auto g = astar::parse_graph({}, {});
  EXPECT_TRUE(g.nodes.empty());
  EXPECT_TRUE(g.adjacency.empty());
}

TEST(GraphTestParseGraph, CheckReturnGraphContainsSameNodesAndEdges) {
  auto nodes = two_nodes();
  auto edges = std::vector<std::tuple<std::string, std::string, double>>{
      {"A", "B", 100.0}};
  auto g = astar::parse_graph(nodes, edges);

  auto gtest = astar::Graph{
      .nodes = {{"A", {"A", 48.85, 2.35}}, {"B", {"B", 51.50, -0.12}}},
      .adjacency = {{"A", {{"B", 100.0}}}}};

  EXPECT_EQ(g, gtest);
}

TEST(GraphTestFindNode, CheckReturnNothingWhenSearchNonExistingNode) {
  auto g = astar::parse_graph(two_nodes(), {});
  auto n = astar::find_node(g, "C");

  ASSERT_FALSE(n.has_value());
}

TEST(GraphTestFindNode, CheckReturnNothingWhenEmptyGraph) {
  auto g = astar::parse_graph({}, {});
  auto n = astar::find_node(g, "A");

  ASSERT_FALSE(n.has_value());
}

TEST(GraphTestFindNode, CheckReturnGreatValuesWhenSearchExistingNode) {
  auto g = astar::parse_graph(two_nodes(), {});
  auto n = astar::find_node(g, "A");

  ASSERT_TRUE(n.has_value());
  EXPECT_DOUBLE_EQ(n->lat, 48.85);
  EXPECT_DOUBLE_EQ(n->lon, 2.35);
}

TEST(GraphTestFindNeighbors, CheckReturnNothingWhenSearchNonExistingNode) {
  auto g = astar::parse_graph(two_nodes(), {});
  auto n = astar::find_neighbors(g, "C");

  ASSERT_FALSE(n.has_value());
}

TEST(GraphTestFindNeighbors, CheckReturnNothingWhenEmptyGraph) {
  auto g = astar::parse_graph({}, {});
  auto n = astar::find_neighbors(g, "A");

  ASSERT_FALSE(n.has_value());
}

TEST(GraphTestFindNeighbors, CheckReturnGreatValuesWhenSearchExistingNode) {
  auto g = astar::parse_graph(two_nodes(), {{"A", "B", 100.0}});
  auto n = astar::find_neighbors(g, "A");

  ASSERT_TRUE(n.has_value());
  EXPECT_EQ(n->size(), 1);
  EXPECT_EQ(n->at(0).to_id, "B");
}

TEST(GraphTestFindNeighbors,
     CheckReturnGreatValuesWhenSearchExistingNodeCompareWithBadValue) {
  auto g = astar::parse_graph(two_nodes(), {{"A", "B", 100.0}});
  auto n = astar::find_neighbors(g, "A");

  ASSERT_TRUE(n.has_value());
  EXPECT_EQ(n->size(), 1);
  EXPECT_FALSE(n->at(0).to_id == "C");
}

TEST(GraphTest, NeighborsExist) {
  auto g = astar::parse_graph(two_nodes(), {{"A", "B", 100.0}});
  auto nb = astar::find_neighbors(g, "A");
  ASSERT_TRUE(nb.has_value());
  EXPECT_EQ(nb->at(0).to_id, "B");
  EXPECT_DOUBLE_EQ(nb->at(0).weight, 100.0);
}

TEST(GraphTest, NeighborsMissing) {
  auto g = astar::parse_graph(two_nodes(), {{"A", "B", 100.0}});
  EXPECT_FALSE(astar::find_neighbors(g, "Z").has_value());
}