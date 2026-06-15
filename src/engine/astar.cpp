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

    
}