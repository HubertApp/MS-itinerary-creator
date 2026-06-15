#pragma once
#include <queue>
#include <string>
#include <stdexcept>

namespace astar {

    struct PQNode {
        double f_score; 
        std::string id;
        bool operator>(const PQNode& o) const { return f_score > o.f_score; }
    };

    using MinHeap = std::priority_queue<PQNode, std::vector<PQNode>, std::greater<PQNode>>;

    MinHeap pq_push(MinHeap pq, PQNode node);
    std::pair<PQNode, MinHeap> pq_pop(MinHeap pq);
    bool pq_empty(const MinHeap pq);

}