// gtest
#include <cstddef>
#include <gtest/gtest.h>

// testing target
#include <parser/parser.hpp>


TEST(CommonTest, TestGraphDirection) {
    std::vector<common::Lexeme> input {
    {common::LexemeType::GRAPH_START_LABEL, std::string("graph")}, 
    {common::LexemeType::OPEN_CURLY_BRACKET},
    {common::LexemeType::CLOSED_CURLY_BRACKET}
    };

    auto graph = parser::parse(input);

    EXPECT_EQ(graph->isDirectional(), 0);


    input = {
        {common::LexemeType::GRAPH_START_LABEL, std::string("digraph")}, 
        {common::LexemeType::OPEN_CURLY_BRACKET},
        {common::LexemeType::CLOSED_CURLY_BRACKET}
    };
    graph = parser::parse(input);

    EXPECT_EQ(graph->isDirectional(), 1);
}


TEST(CommonTest, TestConnection) {
    std::vector<common::Lexeme> input {
    {common::LexemeType::GRAPH_START_LABEL, std::string("graph")}, 
    {common::LexemeType::OPEN_CURLY_BRACKET},
    {common::LexemeType::NODE_ID, std::string("a")},
    {common::LexemeType::FLAT_ARROW},
    {common::LexemeType::NODE_ID, std::string("b")},
    {common::LexemeType::CLOSED_CURLY_BRACKET}
    };

    auto graph = parser::parse(input);

    EXPECT_TRUE(graph->areConnected(std::string("a"), std::string("b")));
    EXPECT_TRUE(graph->areConnected(std::string("b"), std::string("a")));
    EXPECT_EQ(graph->getWeight("a", "b"), std::nullopt);
    EXPECT_EQ(graph->getWeight("b", "a"), std::nullopt);

}


TEST(CommonTest, TestDirectionalConnection) {
    std::vector<common::Lexeme> input {
    {common::LexemeType::GRAPH_START_LABEL, std::string("digraph")}, 
    {common::LexemeType::OPEN_CURLY_BRACKET},
    {common::LexemeType::NODE_ID, std::string("a")},
    {common::LexemeType::POINTED_ARROW},
    {common::LexemeType::NODE_ID, std::string("b")},
    {common::LexemeType::CLOSED_CURLY_BRACKET}
    };

    auto graph = parser::parse(input);

    EXPECT_TRUE(graph->areConnected("a", "b"));
    EXPECT_FALSE(graph->areConnected("b", "a"));
    EXPECT_EQ(graph->getWeight("a", "b"), std::nullopt);
}


TEST(CommonTest, TestConnectionWeight) {
    std::vector<common::Lexeme> input {
    {common::LexemeType::GRAPH_START_LABEL, std::string("graph")}, 
    {common::LexemeType::OPEN_CURLY_BRACKET},
    {common::LexemeType::NODE_ID, std::string("a")},
    {common::LexemeType::FLAT_ARROW},
    {common::LexemeType::NODE_ID, std::string("b")},
    {common::LexemeType::OPEN_SQUARE_BRACKET},
    {common::LexemeType::WEIGHT_ATTRIBUTE},
    {common::LexemeType::EQUALS_SIGN},
    {common::LexemeType::ATTRIBUTE_INT_VALUE, 12},
    {common::LexemeType::CLOSED_SQUARE_BRACKET},
    {common::LexemeType::NODE_ID, std::string("b")},
    {common::LexemeType::FLAT_ARROW},
    {common::LexemeType::NODE_ID, std::string("c")},
    {common::LexemeType::OPEN_SQUARE_BRACKET},
    {common::LexemeType::WEIGHT_ATTRIBUTE},
    {common::LexemeType::EQUALS_SIGN},
    {common::LexemeType::ATTRIBUTE_INT_VALUE, 11},
    {common::LexemeType::CLOSED_SQUARE_BRACKET},
    {common::LexemeType::CLOSED_CURLY_BRACKET}
    };

    auto graph = parser::parse(input);

    EXPECT_TRUE(graph->areConnected("a", "b"));
    EXPECT_TRUE(graph->areConnected("b", "a"));
    EXPECT_EQ(graph->getWeight("a", "b"), 12);
    EXPECT_EQ(graph->getWeight("b", "a"), 12);

    EXPECT_TRUE(graph->areConnected("c", "b"));
    EXPECT_TRUE(graph->areConnected("b", "c"));
    EXPECT_EQ(graph->getWeight("c", "b"), 11);
    EXPECT_EQ(graph->getWeight("b", "c"), 11);
}

TEST(CommonTest, TestDirectionalConnectionWeight) {
    std::vector<common::Lexeme> input {
    {common::LexemeType::GRAPH_START_LABEL, std::string("digraph")},
    {common::LexemeType::OPEN_CURLY_BRACKET},
    {common::LexemeType::NODE_ID, std::string("a")},
    {common::LexemeType::POINTED_ARROW},
    {common::LexemeType::NODE_ID, std::string("b")},
    {common::LexemeType::OPEN_SQUARE_BRACKET},
    {common::LexemeType::WEIGHT_ATTRIBUTE},
    {common::LexemeType::EQUALS_SIGN},
    {common::LexemeType::ATTRIBUTE_INT_VALUE, 12},
    {common::LexemeType::CLOSED_SQUARE_BRACKET},

    {common::LexemeType::NODE_ID, std::string("c")},
    {common::LexemeType::POINTED_ARROW},
    {common::LexemeType::NODE_ID, std::string("d")},
    {common::LexemeType::OPEN_SQUARE_BRACKET},
    {common::LexemeType::WEIGHT_ATTRIBUTE},
    {common::LexemeType::EQUALS_SIGN},
    {common::LexemeType::ATTRIBUTE_INT_VALUE, 1},
    {common::LexemeType::CLOSED_SQUARE_BRACKET},
    {common::LexemeType::CLOSED_CURLY_BRACKET}
    };
    auto graph = parser::parse(input);

    EXPECT_TRUE(graph->areConnected("a", "b"));
    EXPECT_FALSE(graph->areConnected("b", "a"));
    EXPECT_EQ(graph->getWeight("a", "b"), 12);

    EXPECT_TRUE(graph->areConnected("c", "d"));
    EXPECT_FALSE(graph->areConnected("d", "c"));
    EXPECT_EQ(graph->getWeight("c", "d"), 1);
}


TEST(CommonTest, TestLabelParsing) {
    std::vector<common::Lexeme> input {
    {common::LexemeType::GRAPH_START_LABEL, std::string("graph")}, 
    {common::LexemeType::OPEN_CURLY_BRACKET},
    {common::LexemeType::NODE_ID, std::string("a")},
    {common::LexemeType::OPEN_SQUARE_BRACKET},
    {common::LexemeType::LABEL_ATTRIBUTE},
    {common::LexemeType::EQUALS_SIGN},
    {common::LexemeType::ATTRIBUTE_STRING_VALUE, std::string("start")},
    {common::LexemeType::CLOSED_SQUARE_BRACKET},

    {common::LexemeType::NODE_ID, std::string(std::string("b"))},
    {common::LexemeType::OPEN_SQUARE_BRACKET},
    {common::LexemeType::LABEL_ATTRIBUTE},
    {common::LexemeType::EQUALS_SIGN},
    {common::LexemeType::ATTRIBUTE_STRING_VALUE, std::string("end")},
    {common::LexemeType::CLOSED_SQUARE_BRACKET},

    {common::LexemeType::NODE_ID, std::string("a")},
    {common::LexemeType::FLAT_ARROW},
    {common::LexemeType::NODE_ID, std::string("b")},

    {common::LexemeType::NODE_ID, std::string("c")},
    
    {common::LexemeType::CLOSED_CURLY_BRACKET}
    };

    auto graph = parser::parse(input);

    EXPECT_TRUE(graph->areConnected("a", "b"));
    EXPECT_TRUE(graph->areConnected("b", "a"));
    EXPECT_EQ(graph->getLabel("a").value_or("failed extract"), "start");
    EXPECT_EQ(graph->getLabel("b").value_or("failed extract"), "end");
    EXPECT_EQ(graph->getLabel("c"), std::nullopt);
}