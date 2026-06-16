#pragma once

#include <grpcpp/grpcpp.h>
#include "proto/astar.pb.h"
#include "proto/astar.grpc.pb.h"
#include "core/graph.hpp"
#include <unordered_map>
#include <string>

namespace astar::grpc_service {

class AStarServiceImpl final : public ::astar::proto::AStarService::Service {
public:
    AStarServiceImpl();

    ::grpc::Status StoreGraph(
        ::grpc::ServerContext* context, 
        const ::astar::proto::GraphRequest* req, 
        ::astar::proto::StoreResponse* res) override;

    ::grpc::Status Solve(
        ::grpc::ServerContext* context, 
        const ::astar::proto::SolveRequest* req, 
        ::astar::proto::SolveResponse* res) override;

private:
    std::unordered_map<std::string, astar::Graph> graphs_;
};

}