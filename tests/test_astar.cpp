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