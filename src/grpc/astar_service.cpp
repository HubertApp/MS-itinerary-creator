#include "astar_service.hpp"
#include "core/astar.hpp"
#include "core/graph.hpp"
#include <grpcpp/grpcpp.h>
#include <string>
#include <tuple>
#include <vector>

namespace astar::grpc_service {

::grpc::Status AStarServiceImpl::Solve(::grpc::ServerContext *,
                                       const ::astar::proto::SolveRequest *req,
                                       ::astar::proto::SolveResponse *res) {
  if (req->nodes().empty()) {
    return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                          "aucun noeud fourni");
  }
  if (req->start_id().empty() || req->goal_id().empty()) {
    return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                          "start_id et goal_id sont obligatoires");
  }

  std::vector<std::tuple<std::string, double, double>> nodes;
  nodes.reserve(req->nodes().size());
  for (const auto &n : req->nodes()) {
    nodes.emplace_back(n.id(), n.lat(), n.lon());
  }

  std::vector<std::tuple<std::string, std::string, double>> edges;
  edges.reserve(req->edges().size());
  for (const auto &e : req->edges()) {
    edges.emplace_back(e.from_id(), e.to_id(), e.weight());
  }

  const astar::Graph graph = astar::parse_graph(nodes, edges);

  if (!astar::find_node(graph, req->start_id()) ||
      !astar::find_node(graph, req->goal_id())) {
    return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                          "start_id ou goal_id absent du graphe");
  }

  const auto result = astar::run_astar(graph, req->start_id(), req->goal_id());

  if (!result) {
    res->set_found(false);
    return ::grpc::Status::OK;
  }

  res->set_found(true);
  res->set_total_cost(result->total_cost);
  res->set_nodes_explored(result->nodes_explored);
  for (const auto &node_id : result->path) {
    res->add_path(node_id);
  }

  return ::grpc::Status::OK;
}

} // namespace astar::grpc_service