#include "core/priority_queue.hpp"
#include "core/exceptions.hpp"
#include "core/math_utils.hpp"
#include "logging/logging.hpp"
#include <cmath>
#include <numbers>
#include <optional>
#include <string>
#include <vector>

namespace astar {

MinHeap pq_push(MinHeap pq, PQNode node) {
  if (node.f_score < 0) {
    LOG_ERROR("pq_push : f_score negatif (" + std::to_string(node.f_score) +
              ") pour le noeud '" + node.id + "'");
    throw BadValueException(
        "ErrorIllegalValue BadValueException : Score inférieur à Zéro");
  }

  if (std::isnan(node.f_score)) {
    LOG_ERROR("pq_push : f_score NaN pour le noeud '" + node.id + "'");
    throw BadValueException(
        "ErrorIllegalValue BadValueException : Score NaN interdit");
  }

  pq.push(node);
  return pq;
}

std::pair<PQNode, MinHeap> pq_pop(MinHeap pq) {
  if (pq.empty()) {
    LOG_ERROR("pq_pop : tentative de depilement sur une file vide");
    throw EmptyPqException(
        "EmptyPriorityQueueException : Aucune valeur dans la priority queue");
  }

  PQNode top = pq.top();
  pq.pop();
  return {top, pq};
}

bool pq_empty(const MinHeap &pq) { return pq.empty(); }
} // namespace astar