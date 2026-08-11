#pragma once

#include "proto/astar.grpc.pb.h"
#include "proto/astar.pb.h"
#include <grpcpp/grpcpp.h>

namespace astar::grpc_service {

class AStarServiceImpl final : public ::astar::proto::AStarService::Service {
public:
  AStarServiceImpl() = default;

  ::grpc::Status Solve(::grpc::ServerContext *context,
                       const ::astar::proto::SolveRequest *req,
                       ::astar::proto::SolveResponse *res) override;
};

} // namespace astar::grpc_service