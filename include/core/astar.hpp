#pragma once
#include "graph.hpp"
#include "math_utils.hpp"
#include <functional>
#include <optional>
#include <unordered_map>

namespace astar {

using Heuristic = std::function<double(
    const std::string &from_id, const std::string &to_id, const Graph &graph)>;

struct AStarResult {
  std::vector<std::string> path;
  double total_cost;
  size_t nodes_explored;
};

double heuristic_haversine(const std::string &from_id, const std::string &to_id,
                           const Graph &graph);

inline double heuristic_zero(const std::string &, const std::string &,
                             const Graph &) {
  return 0.0;
}

std::vector<std::string>
reconstruct_path(const std::unordered_map<std::string, std::string> &came_from,
                 const std::string &current, std::vector<std::string> acc = {});

std::optional<AStarResult> run_astar(const Graph &graph,
                                     const std::string &start,
                                     const std::string &goal,
                                     Heuristic h = heuristic_haversine);
} // namespace astar