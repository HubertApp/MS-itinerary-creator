#include "astar_service.hpp"
#include "proto/astar.grpc.pb.h"
#include "proto/astar.pb.h"

#include <grpcpp/grpcpp.h>
#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <string>

namespace {

std::string grid_id(int r, int c) {
  return "n" + std::to_string(r) + "_" + std::to_string(c);
}

void fill_grid(astar::proto::SolveRequest &req, int n) {
  for (int r = 0; r < n; ++r)
    for (int c = 0; c < n; ++c) {
      auto *node = req.add_nodes();
      node->set_id(grid_id(r, c));
      node->set_lat(48.0 + r * 1e-3);
      node->set_lon(2.0 + c * 1e-3);
    }

  auto add_edge = [&](const std::string &from, const std::string &to,
                      double w) {
    auto *e = req.add_edges();
    e->set_from_id(from);
    e->set_to_id(to);
    e->set_weight(w);
  };

  for (int r = 0; r < n; ++r)
    for (int c = 0; c < n; ++c) {
      if (r + 1 < n) {
        add_edge(grid_id(r, c), grid_id(r + 1, c), 0.111);
        add_edge(grid_id(r + 1, c), grid_id(r, c), 0.111);
      }
      if (c + 1 < n) {
        add_edge(grid_id(r, c), grid_id(r, c + 1), 0.073);
        add_edge(grid_id(r, c + 1), grid_id(r, c), 0.073);
      }
    }

  req.set_start_id(grid_id(0, 0));
  req.set_goal_id(grid_id(n - 1, n - 1));
}

void fill_disconnected(astar::proto::SolveRequest &req) {
  const std::tuple<const char *, double, double> pts[] = {
      {"A", 48.8566, 2.3522},
      {"B", 48.8570, 2.3530},
      {"X", 43.2965, 5.3698},
      {"Y", 43.2970, 5.3700}};
  for (const auto &[id, lat, lon] : pts) {
    auto *n = req.add_nodes();
    n->set_id(id);
    n->set_lat(lat);
    n->set_lon(lon);
  }
  auto *e1 = req.add_edges();
  e1->set_from_id("A");
  e1->set_to_id("B");
  e1->set_weight(120.0);

  auto *e2 = req.add_edges();
  e2->set_from_id("X");
  e2->set_to_id("Y");
  e2->set_weight(90.0);
}

} // namespace

class AStarGrpcTest : public ::testing::Test {
protected:
  static std::unique_ptr<astar::grpc_service::AStarServiceImpl> service_;
  static std::unique_ptr<grpc::Server> server_;
  static std::unique_ptr<astar::proto::AStarService::Stub> stub_;

  static void SetUpTestSuite() {
    service_ = std::make_unique<astar::grpc_service::AStarServiceImpl>();

    grpc::ServerBuilder builder;
    int port = 0;
    builder.AddListeningPort("127.0.0.1:0", grpc::InsecureServerCredentials(),
                             &port);
    builder.RegisterService(service_.get());
    server_ = builder.BuildAndStart();

    ASSERT_NE(server_, nullptr) << "le serveur gRPC n'a pas demarre";
    ASSERT_GT(port, 0);

    auto channel = grpc::CreateChannel("127.0.0.1:" + std::to_string(port),
                                       grpc::InsecureChannelCredentials());
    stub_ = astar::proto::AStarService::NewStub(channel);

    ASSERT_TRUE(channel->WaitForConnected(std::chrono::system_clock::now() +
                                          std::chrono::seconds(5)));
  }

  static void TearDownTestSuite() {
    if (server_) {
      server_->Shutdown();
      server_->Wait();
    }
    stub_.reset();
    server_.reset();
    service_.reset();
  }

  static std::unique_ptr<grpc::ClientContext> context() {
    auto ctx = std::make_unique<grpc::ClientContext>();
    ctx->set_deadline(std::chrono::system_clock::now() +
                      std::chrono::seconds(10));
    return ctx;
  }
};

std::unique_ptr<astar::grpc_service::AStarServiceImpl> AStarGrpcTest::service_;
std::unique_ptr<grpc::Server> AStarGrpcTest::server_;
std::unique_ptr<astar::proto::AStarService::Stub> AStarGrpcTest::stub_;

TEST_F(AStarGrpcTest, SolveReturnsOptimalPath) {
  astar::proto::SolveRequest req;
  fill_grid(req, 5);

  astar::proto::SolveResponse res;
  auto ctx = context();
  const grpc::Status status = stub_->Solve(ctx.get(), req, &res);

  ASSERT_TRUE(status.ok()) << status.error_message();
  EXPECT_TRUE(res.found());

  ASSERT_EQ(res.path_size(), 9);
  EXPECT_EQ(res.path(0), "n0_0");
  EXPECT_EQ(res.path(8), "n4_4");
  EXPECT_NEAR(res.total_cost(), 4 * 0.111 + 4 * 0.073, 1e-9);
  EXPECT_GT(res.nodes_explored(), 0u);
}

TEST_F(AStarGrpcTest, SolveStartEqualsGoalReturnsSingleNode) {
  astar::proto::SolveRequest req;
  fill_disconnected(req);
  req.set_start_id("A");
  req.set_goal_id("A");

  astar::proto::SolveResponse res;
  auto ctx = context();
  ASSERT_TRUE(stub_->Solve(ctx.get(), req, &res).ok());

  EXPECT_TRUE(res.found());
  ASSERT_EQ(res.path_size(), 1);
  EXPECT_EQ(res.path(0), "A");
  EXPECT_DOUBLE_EQ(res.total_cost(), 0.0);
}

TEST_F(AStarGrpcTest, SolveRejectsEmptyNodeList) {
  astar::proto::SolveRequest req;
  req.set_start_id("A");
  req.set_goal_id("B");

  astar::proto::SolveResponse res;
  auto ctx = context();
  const grpc::Status status = stub_->Solve(ctx.get(), req, &res);

  EXPECT_EQ(status.error_code(), grpc::StatusCode::INVALID_ARGUMENT);
  EXPECT_FALSE(status.error_message().empty())
      << "un message d'erreur exploitable est attendu";
}

TEST_F(AStarGrpcTest, SolveRejectsEmptyStartId) {
  astar::proto::SolveRequest req;
  fill_disconnected(req);
  req.set_start_id("");
  req.set_goal_id("B");

  astar::proto::SolveResponse res;
  auto ctx = context();
  EXPECT_EQ(stub_->Solve(ctx.get(), req, &res).error_code(),
            grpc::StatusCode::INVALID_ARGUMENT);
}

TEST_F(AStarGrpcTest, SolveRejectsUnknownStartId) {
  astar::proto::SolveRequest req;
  fill_disconnected(req);
  req.set_start_id("NOEUD_INEXISTANT");
  req.set_goal_id("B");

  astar::proto::SolveResponse res;
  auto ctx = context();
  EXPECT_EQ(stub_->Solve(ctx.get(), req, &res).error_code(),
            grpc::StatusCode::INVALID_ARGUMENT);
}

TEST_F(AStarGrpcTest, SolveOnDisconnectedGraphReturnsFoundFalse) {
  astar::proto::SolveRequest req;
  fill_disconnected(req);
  req.set_start_id("A");
  req.set_goal_id("Y");

  astar::proto::SolveResponse res;
  auto ctx = context();
  const grpc::Status status = stub_->Solve(ctx.get(), req, &res);

  ASSERT_TRUE(status.ok()) << status.error_message();
  EXPECT_FALSE(res.found());
  EXPECT_EQ(res.path_size(), 0);
}

TEST_F(AStarGrpcTest, SolveWithNegativeWeightReturnsInvalidArgumentNotCrash) {
  astar::proto::SolveRequest req;

  auto *a = req.add_nodes();
  a->set_id("A");
  a->set_lat(48.8566);
  a->set_lon(2.3522);

  auto *b = req.add_nodes();
  b->set_id("B");
  b->set_lat(48.8566);
  b->set_lon(2.3523);

  auto *e = req.add_edges();
  e->set_from_id("A");
  e->set_to_id("B");
  e->set_weight(-100.0);

  req.set_start_id("A");
  req.set_goal_id("B");

  astar::proto::SolveResponse res;
  auto ctx = context();
  const grpc::Status status = stub_->Solve(ctx.get(), req, &res);

  EXPECT_EQ(status.error_code(), grpc::StatusCode::INVALID_ARGUMENT)
      << "l'exception du moteur doit devenir un statut gRPC, pas un crash";

  astar::proto::SolveRequest ok_req;
  fill_grid(ok_req, 4);
  astar::proto::SolveResponse ok_res;
  auto ctx2 = context();
  EXPECT_TRUE(stub_->Solve(ctx2.get(), ok_req, &ok_res).ok())
      << "le serveur doit survivre a une requete fautive";
}

TEST_F(AStarGrpcTest, SolveOnLargeGraphStaysWithinBudget) {
  astar::proto::SolveRequest req;
  fill_grid(req, 40);

  astar::proto::SolveResponse res;
  auto ctx = context();

  const auto t0 = std::chrono::steady_clock::now();
  const grpc::Status status = stub_->Solve(ctx.get(), req, &res);
  const double ms = std::chrono::duration<double, std::milli>(
                        std::chrono::steady_clock::now() - t0)
                        .count();

  ASSERT_TRUE(status.ok()) << status.error_message();
  EXPECT_TRUE(res.found());
  EXPECT_EQ(res.path_size(), 79);
  EXPECT_LT(ms, 2000.0) << "requete trop lente : " << ms << " ms";
}