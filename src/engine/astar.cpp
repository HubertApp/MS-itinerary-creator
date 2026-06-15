#include "core/astar.hpp"
#include "core/priority_queue.hpp"
// #include "core/graph.hpp"
#include <cmath>
#include <vector>
#include <numbers>
#include <optional>
#include <iostream>

namespace astar {
   
    std::vector<std::string> reconstruct_path(
        const std::unordered_map<std::string, std::string> came_from,
        const std::string current, std::vector<std::string> acc)
        {
            acc.push_back(current);
            auto it = came_from.find(current);
            if (it == came_from.end()) {
                std::reverse(acc.begin(), acc.end());
                return acc;
            }
            return reconstruct_path(came_from, it->second, std::move(acc));
        }

    double heuristic_haversine(const std::string from_id,
                           const std::string to_id,
                           const Graph graph)
        {
            auto from = find_node(graph, from_id);
            auto to   = find_node(graph, to_id);
            if (!from || !to) return 0.0;
            return geocalcul::haversine(from->lat, from->lon, to->lat, to->lon);
        }


    std::optional<AStarResult> run_astar(
        const Graph graph, const std::string start,
        const std::string goal, Heuristic h)
        {
            if (start == goal) return AStarResult{{start}, 0.0, 0};

            std::unordered_map<std::string, double> g_score;
            std::unordered_map<std::string, std::string> came_from;
            g_score[start] = 0.0;

            // f(start) = g(start) + h(start, goal) = 0 + h(start, goal)
            MinHeap open_set = pq_push({}, {h(start, goal, graph), start});
            size_t  explored = 0;

            while (!pq_empty(open_set)) {
                auto [cur, rest] = pq_pop(open_set);
                open_set = std::move(rest);

                if (cur.id == goal)
                    return AStarResult{
                        reconstruct_path(came_from, cur.id),
                        g_score[cur.id], explored};

                auto nbrs = find_neighbors(graph, cur.id);
                if (!nbrs) { ++explored; continue; }

                for (const auto& edge : *nbrs) {
                    // g(neighbor) = g(current) + edge.weight  = cout reel
                    double tentative_g = g_score[cur.id] + edge.weight;
                    double known_g     = g_score.count(edge.to_id)
                                    ? g_score.at(edge.to_id)
                                    : std::numeric_limits<double>::infinity();

                    if (tentative_g < known_g) {
                        came_from[edge.to_id] = cur.id;
                        g_score[edge.to_id]   = tentative_g;
                        // f(neighbor) = g(neighbor) + h(neighbor, goal)
                        double f = tentative_g + h(edge.to_id, goal, graph);
                        open_set = pq_push(open_set, {f, edge.to_id});
                    }
                }
                ++explored;
            }
            return std::nullopt;
        }
    
}