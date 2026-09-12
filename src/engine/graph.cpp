#include "core/graph.hpp"
#include "logging/logging.hpp"

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
  size_t unresolved_edges = 0;
  for (const auto &[from_id, to_id, weight] : edges) {

    if (!g.nodes.contains(from_id) || !g.nodes.contains(to_id))
      ++unresolved_edges;
    g.adjacency[from_id].push_back(Edge{to_id, weight});
  }

  if (unresolved_edges > 0)
    LOG_WARN("parse_graph : " + std::to_string(unresolved_edges) +
             " arete(s) referencant un noeud inconnu");

  LOG_DEBUG("parse_graph : " + std::to_string(g.nodes.size()) + " noeuds, " +
            std::to_string(edges.size()) + " aretes");
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