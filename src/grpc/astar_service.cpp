#include "astar_service.hpp"
#include "core/astar.hpp"
#include "core/exceptions.hpp"
#include "core/graph.hpp"
#include "logging/logging.hpp"
#include <chrono>
#include <exception>
#include <grpcpp/grpcpp.h>
#include <string>
#include <tuple>
#include <vector>

namespace astar::grpc_service {

::grpc::Status AStarServiceImpl::Solve(::grpc::ServerContext *context,
                                       const ::astar::proto::SolveRequest *req,
                                       ::astar::proto::SolveResponse *res) {

  const auto t0 = std::chrono::steady_clock::now();
  const std::string peer = context ? context->peer() : "inconnu";

  LOG_INFO("Solve recu de " + peer + " : " + std::to_string(req->nodes().size()) +
           " noeuds, " + std::to_string(req->edges().size()) + " aretes, '" +
           req->start_id() + "' -> '" + req->goal_id() + "'");


  if (req->nodes().empty()) {
    LOG_WARN("Solve rejete (INVALID_ARGUMENT) : aucun noeud fourni");
    return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                          "aucun noeud fourni");
  }
  if (req->start_id().empty() || req->goal_id().empty()) {
    LOG_WARN("Solve rejete (INVALID_ARGUMENT) : start_id ou goal_id vide");
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
    LOG_WARN("Solve rejete (INVALID_ARGUMENT) : '" + req->start_id() +
             "' ou '" + req->goal_id() + "' absent du graphe");
    return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                          "start_id ou goal_id absent du graphe");
  }

  std::optional<astar::AStarResult> result;
  try {
    result = astar::run_astar(graph, req->start_id(), req->goal_id());

  } catch (const astar::BadValueException &e) {

    LOG_ERROR(std::string("Solve : BadValueException -> INVALID_ARGUMENT : ") +
              e.what());
    return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, e.what());

  } catch (const astar::EmptyPqException &e) {

    LOG_ERROR(std::string("Solve : EmptyPqException -> INTERNAL (bug moteur) : ") +
              e.what());
    return ::grpc::Status(::grpc::StatusCode::INTERNAL,
                          "erreur interne du moteur A*");

  } catch (const std::exception &e) {
    LOG_ERROR(std::string("Solve : exception inattendue -> INTERNAL : ") +
              e.what());
    return ::grpc::Status(::grpc::StatusCode::INTERNAL,
                          "erreur interne du moteur A*");

  } catch (...) {
    LOG_ERROR("Solve : exception de type inconnu -> UNKNOWN");
    return ::grpc::Status(::grpc::StatusCode::UNKNOWN, "erreur inconnue");
  }

  const double ms = std::chrono::duration<double, std::milli>(
                        std::chrono::steady_clock::now() - t0)
                        .count();


  if (!result) {
    LOG_INFO("Solve termine : aucun chemin, " + std::to_string(ms) + " ms");
    res->set_found(false);
    return ::grpc::Status::OK;
  }

  res->set_found(true);
  res->set_total_cost(result->total_cost);
  res->set_nodes_explored(result->nodes_explored);
  for (const auto &node_id : result->path) {
    res->add_path(node_id);
  }

  LOG_INFO("Solve termine : chemin de " + std::to_string(result->path.size()) +
           " noeuds, cout=" + std::to_string(result->total_cost) +
           ", explores=" + std::to_string(result->nodes_explored) + ", " +
           std::to_string(ms) + " ms");

  return ::grpc::Status::OK;
}

} // namespace astar::grpc_service