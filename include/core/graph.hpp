#include <unordered_map>
#include <string>
#include <vector>


namespace astar {

    struct Node {
        std::string id;
        double lat;
        double lon;
        bool operator==(const Node&) const = default;
    };

    struct Edge {
        std::string to_id;
        double weight;
        bool operator==(const Edge&) const = default;
    };

    struct Graph {
        std::unordered_map<std::string, Node> nodes;
        std::unordered_map<std::string, std::vector<Edge>> adjacency;
        bool operator==(const Graph&) const = default;
    };


    Graph parse_graph(
        const std::vector<std::tuple<std::string, double, double>>& nodes,
        const std::vector<std::tuple<std::string, std::string, double>>& edges);

}