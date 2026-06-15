#include "core/priority_queue.hpp"
#include "core/exceptions.hpp"
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

TEST(PriorityQueueTestPushPq, CheckPreservPriorityOrderAfterInsertionHighValue) {
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

TEST(PriorityQueueTestPushPq, CheckPreservPriorityOrderAfterInsertionSameValue) {
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

TEST_P(PriorityQueueBadValueTest, CheckThrowExceptionAfterInsertBadValue) {
    astar::MinHeap pq;
    astar::PQNode node;
    node.f_score = 0;
    node.id = "noeud_A";

    EXPECT_THROW(astar::pq_push(pq, node), astar::BadValueException);
}

//Pour "mocker" une liste de valeur, a vérifier si c'est la bonne manière
INSTANTIATE_TEST_SUITE_P(
    InvalidScores,
    PriorityQueueBadValueTest,
    ::testing::Values(0.0, -1.0, -42.5, -0.0001)
);


TEST(PriorityQueueTest, CheckIdValidity) {
    astar::MinHeap pq;
    std::string id_original = "Metz_Square_du_Luxembourg_ligne_A";
    astar::PQNode node{1.5, id_original};

    auto result_pq = astar::pq_push(pq, node);

    astar::PQNode inserted_node = result_pq.top();

    EXPECT_EQ(inserted_node.id, id_original);
    EXPECT_FALSE(inserted_node.id.empty());
}