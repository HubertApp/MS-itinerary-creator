#include "core/graph.hpp"
#include <cmath>
#include <vector>
#include <numbers>
#include <optional>

namespace astar {

    Graph parse_graph(
        const std::vector<std::tuple<std::string, double, double>>& nodes,
        const std::vector<std::tuple<std::string, std::string, double>>& edges) {
        Graph g;
        for (const auto [id, lat, lon] : nodes){
            g.nodes[id] = Node{id, lat, lon};
        }
        for (const auto [from_id ,to_id, weight]: edges){
            g.adjacency[from_id].push_back(Edge{to_id, weight});
        }
        return g;
    }

    std::optional<Node> find_node(const Graph graph, const std::string node_id){
        for (const auto [id, node] : graph.nodes){
            if (id == node_id){
                return node;
            }
        }
        return std::nullopt;
    }

}