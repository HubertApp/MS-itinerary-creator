#include "astar_service.hpp"
#include "core/astar.hpp"
#include <grpcpp/grpcpp.h>
#include <string>
#include <tuple>
#include <vector>

namespace astar::grpc_service {

AStarServiceImpl::AStarServiceImpl() {}

// A voir si ça sert vraiment de stocker le graph en mémoire séparer de la
// résolution Initialement c'était pour save le graph dans redis, mais en vrai
// on peut tous faire d'un coup ça évite de faire un double appel Sachant que on
// va sur du snapshot ça rentre ça calcul et ça sort

::grpc::Status
AStarServiceImpl::StoreGraph(::grpc::ServerContext *,
                             const ::astar::proto::GraphRequest *req,
                             ::astar::proto::StoreResponse *res) {
  std::vector<std::tuple<std::string, double, double>> nodes;
  for (const auto &n : req->nodes()) {
    nodes.emplace_back(n.id(), n.lat(), n.lon());
  }

  std::vector<std::tuple<std::string, std::string, double>> edges;
  for (const auto &e : req->edges()) {
    edges.emplace_back(e.from_id(), e.to_id(), e.weight());
  }

  graphs_[req->graph_id()] = astar::parse_graph(nodes, edges);

  res->set_success(true);
  return ::grpc::Status::OK;
}

::grpc::Status AStarServiceImpl::Solve(::grpc::ServerContext *,
                                       const ::astar::proto::SolveRequest *req,
                                       ::astar::proto::SolveResponse *res) {
  auto it = graphs_.find(req->graph_id());
  if (it == graphs_.end()) {
    return ::grpc::Status(::grpc::NOT_FOUND, "graph not found");
  }

  const auto &g = it->second;

  auto result = astar::run_astar(g, req->start_id(), req->goal_id());

  if (!result) {
    res->set_found(false);
    return ::grpc::Status::OK;
  }

  res->set_found(true);
  res->set_total_cost(result->total_cost);
  res->set_nodes_explored(result->nodes_explored);

  for (const auto &n : result->path) {
    res->add_path(n);
  }

  return ::grpc::Status::OK;
}

} // namespace astar::grpc_service