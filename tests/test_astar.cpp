#include <gtest/gtest.h>
#include "core/astar.hpp"

class AStarTest : public ::testing::Test {
protected:
    astar::Graph g;
    void SetUp() override {
        g = astar::parse_graph(
            {{"A",0.0,0.0},{"B",0.0,1.0},{"C",0.0,2.0},{"D",0.0,3.0}},
            {{"A","B",1.0},{"A","C",4.0},{"B","C",2.0},
             {"B","D",5.0},{"C","D",1.0}});
    }
};

TEST_F(AStarTest, CheckReconstructPathIsPureReturnGreatValue) {
    std::unordered_map<std::string,std::string> cf{{"B","A"},{"C","B"},{"D","C"}};
    EXPECT_EQ(astar::reconstruct_path(cf,"D"),
              astar::reconstruct_path(cf,"D"));
}

TEST_F(AStarTest, CheckReconstructPathSingleNodeSingleNodeReturnOriginNode) {
    std::unordered_map<std::string, std::string> came_from = {};
    std::string current = "A";
    std::vector<std::string> acc = {};

    auto result = astar::reconstruct_path(came_from, current, acc);
    
    EXPECT_EQ(result, std::vector<std::string>({"A"}));
}

TEST_F(AStarTest,  CheckReconstructPathIncompletePathReturnAtFirstOrigin){
   std::unordered_map<std::string, std::string> came_from = {
        {"C", "B"},
        {"D", "C"}
    };
    std::string current = "D";
    std::vector<std::string> acc = {};

    auto result = astar::reconstruct_path(came_from, current, acc);

    EXPECT_EQ(result, std::vector<std::string>({"B", "C", "D"}));
}


TEST_F(AStarTest,  CheckHeuristicHaversineWithSamePointFromTo){
    auto result = astar::heuristic_haversine("A", "A", g);
    EXPECT_EQ(result, 0.0);
}

TEST_F(AStarTest,  CheckHeuristicHaversineReturnGreatValue){
    astar::Graph geo_g;

    geo_g.nodes["A"] = astar::Node{"A", 49.10600415081308, 6.182594836092145};
    geo_g.nodes["B"] = astar::Node{"B", 49.10995796261632, 6.177141075227262};
    
    auto result = astar::heuristic_haversine("A", "B", geo_g);
    EXPECT_NEAR(result, 0.59235843356499385, 1e-9)
      << "La fonction devrait retourner un double";
}

TEST_F(AStarTest, CheckRunAstarPathFound) {
    auto r = astar::run_astar(g,"A","D",astar::heuristic_zero);
    ASSERT_TRUE(r.has_value());
    EXPECT_EQ(r->path.front(),"A");
    EXPECT_EQ(r->path.back(), "D");
}

TEST_F(AStarTest, CheckRunAstarOptimalCost) {
    // A→B(1)+B→C(2)+C→D(1) = 4.0  ← g(D), PAS f(D)
    auto r = astar::run_astar(g,"A","D",astar::heuristic_zero);
    ASSERT_TRUE(r.has_value());
    EXPECT_DOUBLE_EQ(r->total_cost, 4.0);
}

TEST_F(AStarTest, CheckRunAstarStartEqualsGoal) {
    auto r = astar::run_astar(g,"A","A",astar::heuristic_zero);
    ASSERT_TRUE(r.has_value());
    EXPECT_EQ(r->path.size(), 1);
    EXPECT_DOUBLE_EQ(r->total_cost, 0.0);
}

TEST_F(AStarTest, CheckRunAstarUnreachableGoal) {
    EXPECT_FALSE(astar::run_astar(g,"A","Z",astar::heuristic_zero).has_value());
}

TEST_F(AStarTest, CheckRunAstarHeuristicAdmissibleDoesNotInflateCost) {
    astar::Graph g_admissible;
    
    g_admissible.nodes["A"] = astar::Node{"A", 48.856600, 2.352200};
    g_admissible.nodes["B"] = astar::Node{"B", 48.856601, 2.352201};
    g_admissible.nodes["C"] = astar::Node{"C", 48.856602, 2.352202};
    g_admissible.nodes["D"] = astar::Node{"D", 48.856603, 2.352203};

    g_admissible.adjacency["A"].push_back(astar::Edge{"B", 100.0});
    g_admissible.adjacency["B"].push_back(astar::Edge{"C", 200.0});
    g_admissible.adjacency["C"].push_back(astar::Edge{"D", 100.0});
    
    g_admissible.adjacency["A"].push_back(astar::Edge{"C", 400.0}); 

    auto r0 = astar::run_astar(g_admissible, "A", "D", astar::heuristic_zero);
    auto rh = astar::run_astar(g_admissible, "A", "D", astar::heuristic_haversine);
    
    ASSERT_TRUE(r0 && rh);
    EXPECT_DOUBLE_EQ(r0->total_cost, rh->total_cost);
}

TEST_F(AStarTest, CheckHeuristicCostIsGScoreNotFScoreWithHaversine) {
    astar::Graph geo_g;

    geo_g.nodes["A"] = astar::Node{"A", 0.0, 0.0};
    geo_g.nodes["B"] = astar::Node{"B", 0.0, 1.0};
    geo_g.nodes["C"] = astar::Node{"C", 0.0, 2.0};
    geo_g.nodes["D"] = astar::Node{"D", 0.0, 3.0};

    double w_ab = astar::heuristic_haversine("A", "B", geo_g);
    double w_bc = astar::heuristic_haversine("B", "C", geo_g);
    double w_cd = astar::heuristic_haversine("C", "D", geo_g);

    geo_g.adjacency["A"].push_back(astar::Edge{"B", w_ab});
    geo_g.adjacency["B"].push_back(astar::Edge{"C", w_bc});
    geo_g.adjacency["C"].push_back(astar::Edge{"D", w_cd});

    auto r = astar::run_astar(geo_g, "A", "D", astar::heuristic_haversine);
    ASSERT_TRUE(r.has_value());

    double real = 0.0;
    for (size_t i = 0; i + 1 < r->path.size(); ++i) {
        auto nbrs = astar::find_neighbors(geo_g, r->path[i]);
        ASSERT_TRUE(nbrs.has_value());
        
        for (const auto& e : *nbrs) {
            if (e.to_id == r->path[i+1]) { 
                real += e.weight; 
                break; 
            }
        }
    }

    EXPECT_DOUBLE_EQ(r->total_cost, real);
}