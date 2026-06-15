#include <cmath>
#include <vector>
#include <numbers>
#include <optional>
#include <string>
#include "core/priority_queue.hpp"
#include "core/math_utils.hpp"
#include "core/exceptions.hpp"


namespace astar {

    MinHeap pq_push(MinHeap pq, PQNode node){
        if(node.f_score <= 0 ){
            throw BadValueException("ErrorIllegalValue BadValueException : Score inférieur ou égal à Zéro");
        }

        pq.push(node);
        return pq;
    }

    std::pair<PQNode, MinHeap> pq_pop(MinHeap pq)
    {
        if(pq.empty()){
            throw EmptyPqException("EmptyPriorityQueueException : Aucune valeur dans la priority queue");
        }

        PQNode top = pq.top();
        pq.pop();
        return {top, pq};
    }

    
}