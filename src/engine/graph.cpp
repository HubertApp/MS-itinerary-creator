#include "core/graph.hpp"

namespace astar {

Graph parse_graph(
    const std::vector<std::tuple<std::string, double, double>> &nodes,
    const std::vector<std::tuple<std::string, std::string, double>> &edges) {
  Graph g;
  g.nodes.reserve(nodes.size());
  g.adjacency.reserve(nodes.size());
  for (const auto &[id, lat, lon] : nodes) {
    g.nodes[id] = Node{id, lat, lon};
  }
  for (const auto &[from_id, to_id, weight] : edges) {
    g.adjacency[from_id].push_back(Edge{to_id, weight});
  }
  return g;
}

std::optional<Node> find_node(const Graph &graph, const std::string &node_id) {
  auto it = graph.nodes.find(node_id);
  if (it == graph.nodes.end())
    return std::nullopt;
  return it->second;
}

std::optional<std::vector<Edge>> find_neighbors(const Graph &graph,
                                                const std::string &node_id) {
  auto it = graph.adjacency.find(node_id);
  if (it == graph.adjacency.end())
    return std::nullopt;
  return it->second;
}

} // namespace astar