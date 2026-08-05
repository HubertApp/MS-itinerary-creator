#include "core/exceptions.hpp"
#include "core/priority_queue.hpp"
#include <gtest/gtest.h>

class PriorityQueueBadValueTest : public ::testing::TestWithParam<double> {};

TEST(PriorityQueueTestPushPq, CheckReturnOneValueAfterInsertGreatValue) {
  astar::MinHeap pq;
  astar::PQNode node;
  node.f_score = 42.5;
  node.id = "noeud_A";

  auto g = astar::pq_push(pq, node);

  EXPECT_FALSE(g.empty());
  EXPECT_EQ(g.size(), 1);
  EXPECT_EQ(g.top().id, "noeud_A");
}

TEST(PriorityQueueTestPushPq,
     CheckPreservPriorityOrderAfterInsertionHighValue) {
  astar::MinHeap pq;
  astar::PQNode node;
  node.f_score = 25;
  node.id = "noeud_A";

  auto g = astar::pq_push(pq, node);

  EXPECT_FALSE(g.empty());
  EXPECT_EQ(g.size(), 1);
  EXPECT_EQ(g.top().id, "noeud_A");

  node.f_score = 45;
  node.id = "noeud_B";

  auto g2 = astar::pq_push(g, node);
  EXPECT_FALSE(g2.empty());
  EXPECT_EQ(g2.size(), 2);
  EXPECT_EQ(g2.top().id, "noeud_A");
}

TEST(PriorityQueueTestPushPq, CheckPreservPriorityOrderAfterInsertionLowValue) {
  astar::MinHeap pq;
  astar::PQNode node;
  node.f_score = 25;
  node.id = "noeud_A";

  auto g = astar::pq_push(pq, node);

  EXPECT_FALSE(g.empty());
  EXPECT_EQ(g.size(), 1);
  EXPECT_EQ(g.top().id, "noeud_A");

  node.f_score = 15;
  node.id = "noeud_B";

  auto g2 = astar::pq_push(g, node);
  EXPECT_FALSE(g2.empty());
  EXPECT_EQ(g2.size(), 2);
  EXPECT_EQ(g2.top().id, "noeud_B");
}

TEST(PriorityQueueTestPushPq,
     CheckPreservPriorityOrderAfterInsertionSameValue) {
  astar::MinHeap pq;
  astar::PQNode node;
  node.f_score = 25;
  node.id = "noeud_A";

  auto g = astar::pq_push(pq, node);

  EXPECT_FALSE(g.empty());
  EXPECT_EQ(g.size(), 1);
  EXPECT_EQ(g.top().id, "noeud_A");

  node.f_score = 25;
  node.id = "noeud_B";

  auto g2 = astar::pq_push(g, node);
  EXPECT_FALSE(g2.empty());
  EXPECT_EQ(g2.size(), 2);
  EXPECT_EQ(g2.top().id, "noeud_A");
}

TEST(PriorityQueueTest, CheckThrowExceptionAfterInsertBadValue) {
  astar::MinHeap pq;
  astar::PQNode node;
  node.f_score = -3;
  node.id = "noeud_A";

  EXPECT_THROW(astar::pq_push(pq, node), astar::BadValueException);
}

TEST(PriorityQueueTest, CheckIdValidity) {
  astar::MinHeap pq;
  std::string id_original = "Metz_Square_du_Luxembourg_ligne_A";
  astar::PQNode node{1.5, id_original};

  auto result_pq = astar::pq_push(pq, node);

  astar::PQNode inserted_node = result_pq.top();

  EXPECT_EQ(inserted_node.id, id_original);
  EXPECT_FALSE(inserted_node.id.empty());
}

// Pop function
TEST(PriorityQueueTest, CheckReturnEmptyAfterPopValue) {
  astar::MinHeap pq;
  astar::PQNode node;
  node.f_score = 42.5;
  node.id = "noeud_A";

  auto g = astar::pq_push(pq, node);

  EXPECT_FALSE(g.empty());
  EXPECT_EQ(g.size(), 1);
  EXPECT_EQ(g.top().id, "noeud_A");

  auto result = astar::pq_pop(g);

  EXPECT_TRUE(result.second.empty());
  EXPECT_EQ(result.second.size(), 0);

  EXPECT_EQ(result.first.id, "noeud_A");
  EXPECT_EQ(result.first.f_score, 42.5);
}

TEST(PriorityQueueTest, CheckReturnSmallestValueFirst) {
  astar::MinHeap pq;

  astar::PQNode node;
  node.f_score = 40;
  node.id = "noeud_A";

  auto g = astar::pq_push(pq, node);

  node.f_score = 25;
  node.id = "noeud_B";

  auto g2 = astar::pq_push(g, node);

  node.f_score = 30;
  node.id = "noeud_C";

  auto g3 = astar::pq_push(g2, node);

  auto result = astar::pq_pop(g3);

  EXPECT_FALSE(result.second.empty());
  EXPECT_EQ(result.second.size(), 2);

  EXPECT_EQ(result.first.id, "noeud_B");
  EXPECT_EQ(result.first.f_score, 25);
}

TEST(PriorityQueueTest, CheckThrowExceptionAfterPopEmptyPq) {
  astar::MinHeap pq;
  EXPECT_THROW(astar::pq_pop(pq), astar::EmptyPqException);
}

TEST(PriorityQueueTest, CheckReturnFalseIfNotEmptyPq) {
  astar::MinHeap pq;
  astar::PQNode node;
  node.f_score = 40;
  node.id = "noeud_A";

  auto g = astar::pq_push(pq, node);

  EXPECT_FALSE(astar::pq_empty(g));
}

TEST(PriorityQueueTest, CheckReturnTrueIfEmptyPq) {
  astar::MinHeap pq;
  EXPECT_TRUE(astar::pq_empty(pq));
}