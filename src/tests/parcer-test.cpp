// gtest
#include <gtest/gtest.h>

// testing target
#include <common/common.hpp>
#include <parcer/parcer.hpp>


TEST(CommonTest, TestGraphDirection) {
    std::vector<common::Lexeme> inpt {
    {common::LexemeType::GRAPH_START_LABEL, std::string("graph")}, 
    {common::LexemeType::OPEN_CURLY_BRACKET},
    {common::LexemeType::CLOSED_CURLY_BRACKET}};
    common::Graph graph = parcer::parce(inpt);

    EXPECT_EQ(graph.isDirectional(), 0);


    inpt = {
        {common::LexemeType::GRAPH_START_LABEL, std::string("digraph")}, 
        {common::LexemeType::OPEN_CURLY_BRACKET},
        {common::LexemeType::CLOSED_CURLY_BRACKET}};
    graph = parcer::parce(inpt);

    EXPECT_EQ(graph.isDirectional(), 1);
}


TEST(CommonTest, TestConnection) {
    std::vector<common::Lexeme> inpt {
    {common::LexemeType::GRAPH_START_LABEL, std::string("graph")}, 
    {common::LexemeType::OPEN_CURLY_BRACKET},
    {common::LexemeType::NODE_ID, std::string("a")},
    {common::LexemeType::FLAT_ARROW},
    {common::LexemeType::NODE_ID, std::string("b")},
    {common::LexemeType::CLOSED_CURLY_BRACKET}};
    common::Graph graph = parcer::parce(inpt);

    EXPECT_EQ(graph.areConnected(std::string("a"), std::string("b")), true);
    EXPECT_EQ(graph.areConnected(std::string("b"), std::string("a")), true);
    EXPECT_EQ(graph.getWeight("a", "b"), std::nullopt);
    EXPECT_EQ(graph.getWeight("b", "a"), std::nullopt);

}


TEST(CommonTest, TestDirectionalConnection) {
    std::vector<common::Lexeme> inpt {
    {common::LexemeType::GRAPH_START_LABEL, std::string("digraph")}, 
    {common::LexemeType::OPEN_CURLY_BRACKET},
    {common::LexemeType::NODE_ID, std::string("a")},
    {common::LexemeType::POINTED_ARROW},
    {common::LexemeType::NODE_ID, std::string("b")},
    {common::LexemeType::CLOSED_CURLY_BRACKET}};
    common::Graph graph = parcer::parce(inpt);

    EXPECT_EQ(graph.areConnected("a", "b"), true);
    EXPECT_EQ(graph.areConnected("b", "a"), false);
    EXPECT_EQ(graph.getWeight("a", "b"), std::nullopt);
}


TEST(CommonTest, TestConnectionWeight) {
    std::vector<common::Lexeme> inpt {
    {common::LexemeType::GRAPH_START_LABEL, std::string("graph")}, 
    {common::LexemeType::OPEN_CURLY_BRACKET},
    {common::LexemeType::NODE_ID, std::string("a")},
    {common::LexemeType::FLAT_ARROW},
    {common::LexemeType::NODE_ID, std::string("b")},
    {common::LexemeType::OPEN_SQUARE_BRACKET},
    {common::LexemeType::WEIGHT_ATTRIBUTE},
    {common::LexemeType::EQUALS_SIGN},
    {common::LexemeType::ATTRIBUTE_INT_VALUE, 12},
    {common::LexemeType::CLOSED_SQUIRED_BRACKET},
    {common::LexemeType::NODE_ID, std::string("b")},
    {common::LexemeType::FLAT_ARROW},
    {common::LexemeType::NODE_ID, std::string("c")},
    {common::LexemeType::OPEN_SQUARE_BRACKET},
    {common::LexemeType::WEIGHT_ATTRIBUTE},
    {common::LexemeType::EQUALS_SIGN},
    {common::LexemeType::ATTRIBUTE_INT_VALUE, 11},
    {common::LexemeType::CLOSED_SQUIRED_BRACKET},
    {common::LexemeType::CLOSED_CURLY_BRACKET}};
    common::Graph graph = parcer::parce(inpt);

    EXPECT_EQ(graph.areConnected("a", "b"), true);
    EXPECT_EQ(graph.areConnected("b", "a"), true);
    EXPECT_EQ(graph.getWeight("a", "b"), 12);
    EXPECT_EQ(graph.getWeight("b", "a"), 12);

    EXPECT_EQ(graph.areConnected("c", "b"), true);
    EXPECT_EQ(graph.areConnected("b", "c"), true);
    EXPECT_EQ(graph.getWeight("c", "b"), 11);
    EXPECT_EQ(graph.getWeight("b", "c"), 11);
}

TEST(CommonTest, TestDirectionalConnectionWeight) {
    std::vector<common::Lexeme> inpt {
    {common::LexemeType::GRAPH_START_LABEL, std::string("digraph")},
    {common::LexemeType::OPEN_CURLY_BRACKET},
    {common::LexemeType::NODE_ID, std::string("a")},
    {common::LexemeType::POINTED_ARROW},
    {common::LexemeType::NODE_ID, std::string("b")},
    {common::LexemeType::OPEN_SQUARE_BRACKET},
    {common::LexemeType::WEIGHT_ATTRIBUTE},
    {common::LexemeType::EQUALS_SIGN},
    {common::LexemeType::ATTRIBUTE_INT_VALUE, 12},
    {common::LexemeType::CLOSED_SQUIRED_BRACKET},

    {common::LexemeType::NODE_ID, std::string("c")},
    {common::LexemeType::POINTED_ARROW},
    {common::LexemeType::NODE_ID, std::string("d")},
    {common::LexemeType::OPEN_SQUARE_BRACKET},
    {common::LexemeType::WEIGHT_ATTRIBUTE},
    {common::LexemeType::EQUALS_SIGN},
    {common::LexemeType::ATTRIBUTE_INT_VALUE, 1},
    {common::LexemeType::CLOSED_SQUIRED_BRACKET},
    {common::LexemeType::CLOSED_CURLY_BRACKET}};
    common::Graph graph = parcer::parce(inpt);

    EXPECT_EQ(graph.areConnected("a", "b"), true);
    EXPECT_EQ(graph.areConnected("b", "a"), false);
    EXPECT_EQ(graph.getWeight("a", "b"), 12);

    EXPECT_EQ(graph.areConnected("c", "d"), true);
    EXPECT_EQ(graph.areConnected("d", "c"), false);
    EXPECT_EQ(graph.getWeight("c", "d"), 1);
}