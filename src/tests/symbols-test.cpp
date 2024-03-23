// gtest
#include <gtest/gtest.h>

// standard
#include <iostream>
#include <ostream>
#include <vector>

// testing target
#include <lexer/lexer.hpp>
#include <common/common.hpp>

namespace {

    bool matchLex(const std::vector<common::Lexeme>& first, const std::vector<common::Lexeme>& second) {
        if (first.size() != second.size()) {
            return false;
        }

        for (auto iter1 = first.begin(), iter2 = second.begin(); iter1 != first.end(); ++iter1, ++iter2) {
            if ((*iter1).type != (*iter2).type) {
                return false;
            }
        }
        return true;
    }

} // namespace


TEST(LexerTest, ParseSingleEdge) {
    std::string in = "graph {\na -- b\n}";
    auto lexed = lexer::lex(in);

    EXPECT_TRUE(matchLex(lexed, {
        {common::LexemeType::GRAPH_START_LABEL, std::string("graph")}, 
        {common::LexemeType::OPEN_CURLY_BRACKET},
        {common::LexemeType::NODE_ID, std::string("a")},
        {common::LexemeType::FLAT_ARROW},
        {common::LexemeType::NODE_ID, std::string("b")},
        {common::LexemeType::CLOSED_CURLY_BRACKET}
    }));
}

TEST(LexerTest, ParseSingleDirectionalEdge) {
    std::string in = "digraph {\na -> b \n}";
    auto lexed = lexer::lex(in);

    EXPECT_TRUE(matchLex(lexed, {
        {common::LexemeType::GRAPH_START_LABEL, std::string("digraph")}, 
        {common::LexemeType::OPEN_CURLY_BRACKET},
        {common::LexemeType::NODE_ID, std::string("a")},
        {common::LexemeType::POINTED_ARROW},
        {common::LexemeType::NODE_ID, std::string("b")},
        {common::LexemeType::CLOSED_CURLY_BRACKET}
    }));
}

TEST(LexerTest, ParseMultipleWithAttributes) {
    std::string in = "graph {\na[label = \"qwed\"]\n a -- b \n b -- c\n}\n";
    auto lexed = lexer::lex(in);

    EXPECT_TRUE(matchLex(lexed, {
        {common::LexemeType::GRAPH_START_LABEL, std::string("graph")}, 
        {common::LexemeType::OPEN_CURLY_BRACKET},
        {common::LexemeType::NODE_ID, std::string("a")},
        {common::LexemeType::OPEN_SQUARE_BRACKET},
        {common::LexemeType::LABEL_ATTRIBUTE},
        {common::LexemeType::EQUALS_SIGN},
        {common::LexemeType::ATTRIBUTE_STRING_VALUE, std::string("qwed")},
        {common::LexemeType::CLOSED_SQUARE_BRACKET},
        {common::LexemeType::NODE_ID, std::string("a")},
        {common::LexemeType::FLAT_ARROW},
        {common::LexemeType::NODE_ID, std::string("b")},
        {common::LexemeType::NODE_ID, std::string("b")},
        {common::LexemeType::FLAT_ARROW},
        {common::LexemeType::NODE_ID, std::string("c")},
        {common::LexemeType::CLOSED_CURLY_BRACKET}
    }));
}

TEST(LexerTest, ParseNodeAttribute) {
    std::string input = "graph {\nA [label=\"Node A\"] \n}";
    auto lexed = lexer::lex(input);

    EXPECT_TRUE(matchLex(lexed, {
        {common::LexemeType::GRAPH_START_LABEL, std::string("graph")}, 
        {common::LexemeType::OPEN_CURLY_BRACKET},
        {common::LexemeType::NODE_ID, std::string("A")},
        {common::LexemeType::OPEN_SQUARE_BRACKET},
        {common::LexemeType::LABEL_ATTRIBUTE},
        {common::LexemeType::EQUALS_SIGN},
        {common::LexemeType::ATTRIBUTE_STRING_VALUE, std::string("Node A")},
        {common::LexemeType::CLOSED_SQUARE_BRACKET},
        {common::LexemeType::CLOSED_CURLY_BRACKET}
    }));
}


TEST(LexerTest, ParseGraphWithMultipleEdgesAndNodeAttributes) {
    std::string input = "graph {\nA [label=\"Node A\"] \nB [label=\"Node B\"] \nA -- B \nB -- C \n}";
    auto lexed = lexer::lex(input);

    EXPECT_TRUE(matchLex(lexed, {
        {common::LexemeType::GRAPH_START_LABEL, std::string("graph")}, 
        {common::LexemeType::OPEN_CURLY_BRACKET},
        {common::LexemeType::NODE_ID, std::string("A")},
        {common::LexemeType::OPEN_SQUARE_BRACKET},
        {common::LexemeType::LABEL_ATTRIBUTE},
        {common::LexemeType::EQUALS_SIGN},
        {common::LexemeType::ATTRIBUTE_STRING_VALUE, std::string("Node A")},
        {common::LexemeType::CLOSED_SQUARE_BRACKET},
        {common::LexemeType::NODE_ID, std::string("B")},
        {common::LexemeType::OPEN_SQUARE_BRACKET},
        {common::LexemeType::LABEL_ATTRIBUTE},
        {common::LexemeType::EQUALS_SIGN},
        {common::LexemeType::ATTRIBUTE_STRING_VALUE, std::string("Node B")},
        {common::LexemeType::CLOSED_SQUARE_BRACKET},
        {common::LexemeType::NODE_ID, std::string("A")},
        {common::LexemeType::FLAT_ARROW},
        {common::LexemeType::NODE_ID, std::string("B")},
        {common::LexemeType::NODE_ID, std::string("B")},
        {common::LexemeType::FLAT_ARROW},
        {common::LexemeType::NODE_ID, std::string("C")},
        {common::LexemeType::CLOSED_CURLY_BRACKET}
    }));
}


TEST(LexerTest, ParseDirectedGraphWithCycle) {
    std::string input = "digraph {\nA -> B \nB -> C \nC -> A \n}";
    auto lexed = lexer::lex(input);

    EXPECT_TRUE(matchLex(lexed, {
        {common::LexemeType::GRAPH_START_LABEL, std::string("digraph")}, 
        {common::LexemeType::OPEN_CURLY_BRACKET},
        {common::LexemeType::NODE_ID, std::string("A")},
        {common::LexemeType::POINTED_ARROW},
        {common::LexemeType::NODE_ID, std::string("B")},
        {common::LexemeType::NODE_ID, std::string("B")},
        {common::LexemeType::POINTED_ARROW},
        {common::LexemeType::NODE_ID, std::string("C")},
        {common::LexemeType::NODE_ID, std::string("C")},
        {common::LexemeType::POINTED_ARROW},
        {common::LexemeType::NODE_ID, std::string("A")},
        {common::LexemeType::CLOSED_CURLY_BRACKET}
    }));
}

TEST(LexerTest, ParseGraphWithoutNodes) {
    std::string input = "graph {}";
    auto lexed = lexer::lex(input);

    EXPECT_TRUE(matchLex(lexed, {
        {common::LexemeType::GRAPH_START_LABEL, std::string("graph")}, 
        {common::LexemeType::OPEN_CURLY_BRACKET},
        {common::LexemeType::CLOSED_CURLY_BRACKET}
    }));
}
