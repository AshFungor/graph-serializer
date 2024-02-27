// gtest
#include <gtest/gtest.h>

// standard
#include <iostream>
#include <optional>

// testing target
#define private public
#include <common/common.hpp>
#undef private

#define GTEST_COUT(chain) \
    std::cout << "[ INFO     ] " << chain << '\n';

namespace {
    void dumpContainer(const common::Graph& target) {
        GTEST_COUT("reading elements of graph container (nullopt is replaced with 0): ")
        for (const auto& pair : *target.connections_) {
            GTEST_COUT("node [" << pair.first << "], connections: ");
            for (const auto& connection : pair.second) {
                GTEST_COUT("- connection: peer = " << connection.peer << ", weight: " << connection.weight.value_or(0));
            }
        }
    }

    void pushSomeNodes(common::Graph& graph) {
        graph.pushNode("first");
        graph.pushNode("second");
        graph.pushNode("third");
    }

    void pushSomeEdges(common::Graph& graph) {
        graph.pushEdge("first", {1, "second"});
        graph.pushEdge("second", {2, "third"});
        graph.pushEdge("third", {3, "first"});
    }
}

TEST(CommonTest, DirectionalWeightedGraphTest) {
    common::Graph graph (nullptr, common::opt::drc | common::opt::wgh);

    pushSomeNodes(graph);
    pushSomeEdges(graph);

    EXPECT_EQ(graph.areConnected("first", "second"), true);
    EXPECT_EQ(graph.areConnected("second", "third"), true);
    EXPECT_EQ(graph.areConnected("third", "first"), true);

    EXPECT_EQ(graph.areConnected("second", "first"), false);
    EXPECT_EQ(graph.areConnected("third", "second"), false);
    EXPECT_EQ(graph.areConnected("first", "third"), false);

    EXPECT_EQ(graph.getWeight("first", "second"), 1);
    EXPECT_EQ(graph.getWeight("second", "third"), 2);
    EXPECT_EQ(graph.getWeight("third", "first"), 3);

    dumpContainer(graph);
}

TEST(CommonTest, ZeroFlagGraphTest) {
    common::Graph graph (nullptr, 0x0);

    pushSomeNodes(graph);
    pushSomeEdges(graph);

    EXPECT_EQ(graph.areConnected("first", "second"), true);
    EXPECT_EQ(graph.areConnected("second", "third"), true);
    EXPECT_EQ(graph.areConnected("third", "first"), true);

    EXPECT_EQ(graph.areConnected("second", "first"), true);
    EXPECT_EQ(graph.areConnected("third", "second"), true);
    EXPECT_EQ(graph.areConnected("first", "third"), true);

    EXPECT_EQ(graph.getWeight("first", "second"), std::nullopt);
    EXPECT_EQ(graph.getWeight("second", "third"), std::nullopt);
    EXPECT_EQ(graph.getWeight("third", "first"), std::nullopt);

    EXPECT_EQ(graph.getWeight("second", "first"), std::nullopt);
    EXPECT_EQ(graph.getWeight("third", "second"), std::nullopt);
    EXPECT_EQ(graph.getWeight("first", "third"), std::nullopt);

    dumpContainer(graph);
}