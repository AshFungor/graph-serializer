// gtest
#include <gtest/gtest.h>

// standard
#include <fstream>
#include <filesystem>
#include <ios>
#include <iostream>
#include <memory>
#include <optional>

// testing target
#define private public
#include <common/common.hpp>
#include <common/reverted.hpp>
#undef private

// lexer & parser
#include <lexer/lexer.hpp>
#include <parser/parser.hpp>


#define GTEST_COUT(chain) \
    std::cout << "[ INFO     ] " << chain << '\n';

namespace {
    void dumpContainer(const common::Graph& target) {
        GTEST_COUT("Graph structure:");

        auto nodes = target.getNodes();
        
        for (const auto& node : nodes) {
            GTEST_COUT("Node [" << node << "], connections:");

            auto all_nodes = target.getNodes();
            for (const auto& peer : all_nodes) {
                if (target.areConnected(node, peer)) {
                    auto weight = target.getWeight(node, peer);
                    GTEST_COUT("  -> " << peer << " | Weight: " << weight.value_or(0));
                }
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

    std::shared_ptr<common::Graph> readFromFile(const std::string& filename) {
        std::ifstream ifs {filename, std::ios::in | std::ios::binary};
        std::string contents;
        contents.resize(std::filesystem::file_size(filename));

        ifs.read(contents.data(), contents.size());
        auto lexed = lexer::lex(contents);
        auto parsed = parser::parse(lexed);

        return std::move(parsed);
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

    // dumpContainer(graph);
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

    // dumpContainer(graph);
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

    // we can't parse comments, write non-verbose
    common::GraphDumpingFactory factory ({.verboseWrite = false});
    EXPECT_NO_THROW(factory.dumpOne(graph, "test.gv"));

    std::shared_ptr<common::Graph> parsedBack;
    EXPECT_NO_THROW(parsedBack = readFromFile("test.gv"));
    GTEST_COUT("parsed back:\n" << parsedBack->dumpGraphState());
}