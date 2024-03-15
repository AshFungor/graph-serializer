// gtest
#include <gtest/gtest.h>

// standard
#include <filesystem>
#include <iostream>
#include <optional>

// testing target
#define private public
#include <common/common.hpp>
#include <common/reverted.hpp>
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
        graph.pushEdge("first", common::Connection("second", 1));
        graph.pushEdge("second", common::Connection("third", 2));
        graph.pushEdge("third", common::Connection("first", 3));
    }

    void pushMoreNodesWithLabels(common::Graph& graph) {
        graph.pushNode("fourth");
        graph.pushNode("fifth");
        graph.pushNode("sixth");

        graph.setLabel("fourth", "fourth");
        graph.setLabel("fifth", "fifth");
        graph.setLabel("sixth", "sixth");
    }

    void pushMoreEdges(common::Graph& graph) {
        graph.pushEdge("second", common::Connection("first", 4));
        graph.pushEdge("third", common::Connection("second", 5));
        graph.pushEdge("first", common::Connection("third", 6));
        graph.pushEdge("fourth", common::Connection("sixth", 7));
        graph.pushEdge("fifth", common::Connection("sixth", 8));
        graph.pushEdge("sixth", common::Connection("fifth", 9));
    }
}

TEST(CommonTest, DirectionalWeightedGraphTest) {
    common::Graph graph;
    graph.init(common::opt::drc | common::opt::wgh);

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
    common::Graph graph;
    graph.init(0x0);

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

TEST(CommonTest, VerboseGraph) {
    common::Graph graph;
    graph.init(common::opt::drc | common::opt::wgh);

    pushSomeNodes(graph);
    pushSomeEdges(graph);

    GTEST_COUT(graph.dumpGraphState());
}

TEST(CommonTest, DumpingTest) {
    common::Graph graph;
    graph.init(common::opt::drc | common::opt::wgh);

    pushSomeNodes(graph);
    pushSomeEdges(graph);

    pushMoreNodesWithLabels(graph);
    pushMoreEdges(graph);

    if (std::filesystem::exists("test.gv")) {
        std::filesystem::remove("test.gv");
    }

    common::GraphDumpingFactory factory ({.verboseWrite = true});
    EXPECT_NO_THROW(factory.dumpOne(graph, "test.gv"));

    // add further validation later
}