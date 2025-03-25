// gtest
#include <gtest/gtest.h>

// standard
#include <filesystem>
#include <string>
#include <unordered_set>
#include <vector>

// testing target
#include <algorithms/traversal.hpp>
#include <common/common.hpp>

#define GTEST_COUT(chain) \
    std::cout << "[ INFO     ] " << chain << '\n';

namespace {
    void pushSomeNodes(common::TraversalGraph& graph) {
        graph.pushNode("A");
        graph.pushNode("B");
        graph.pushNode("C");
        graph.pushNode("D");
        graph.pushNode("E");
    }

    void buildSampleGraph(common::TraversalGraph& graph) {
        graph.pushEdge("A", common::Connection("B", 1));
        graph.pushEdge("B", common::Connection("C", 2));
        graph.pushEdge("C", common::Connection("D", 3));
        graph.pushEdge("D", common::Connection("E", 4));
    }
}

class TraversalTest : public ::testing::Test {
protected:
    void SetUp() override {
        graph_.init(common::opt::drc);
        pushSomeNodes(graph_);
        buildSampleGraph(graph_);
    }

    common::TraversalGraph graph_;
    std::unordered_set<std::string> visited_;
    std::vector<std::string> traversal_order_;
    std::unordered_map<std::string, std::pair<int, int>> timestamps_;
    int discovery_time_ = 0;
};

TEST_F(TraversalTest, FullTraversalTimestamps) {
    int flag = 0;
    graph_.tofWithTimestamps("A", visited_, traversal_order_, timestamps_, discovery_time_, flag);

    EXPECT_EQ(visited_.size(), 5);
    
    EXPECT_LT(timestamps_["A"].first, timestamps_["B"].first);
    EXPECT_LT(timestamps_["B"].first, timestamps_["C"].first);
    EXPECT_GT(timestamps_["E"].second, timestamps_["D"].second);
}

TEST_F(TraversalTest, TargetedSearchSuccess) {
    int flag_to_quit = 0;
    std::string target = "C";
    
    graph_.dfsWithTimestamps("A", visited_, traversal_order_, timestamps_, 
                            discovery_time_, flag_to_quit, target);

    EXPECT_EQ(flag_to_quit, 1);
    EXPECT_TRUE(visited_.contains("C"));
    EXPECT_FALSE(visited_.contains("E"));

    std::vector<std::string> expected_path{"A", "B", "C"};
    EXPECT_EQ(traversal_order_, expected_path);
}

TEST_F(TraversalTest, TargetedSearchFailure) {
    int flag_to_quit = 0;
    std::string target = "F"; 
    
    graph_.dfsWithTimestamps("A", visited_, traversal_order_, timestamps_,
                            discovery_time_, flag_to_quit, target);

    EXPECT_EQ(flag_to_quit, 0);
    EXPECT_TRUE(traversal_order_.empty());
}

TEST_F(TraversalTest, ComponentTraversal) {
    graph_.pushNode("F");
    graph_.pushNode("G");
    graph_.pushEdge("F", common::Connection("G", 5));

    int flag = 0;
    graph_.tofWithTimestamps("A", visited_, traversal_order_, timestamps_, discovery_time_, flag);
    graph_.tofWithTimestamps("F", visited_, traversal_order_, timestamps_, discovery_time_, flag);

    EXPECT_EQ(visited_.size(), 7);
    EXPECT_TRUE(timestamps_["F"].first > timestamps_["E"].second);
}