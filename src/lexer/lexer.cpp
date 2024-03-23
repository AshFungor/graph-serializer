// standard
#include <any>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>
#include <iostream>
// common
#include <common/common.hpp>

// local
#include "lexer.hpp"

using namespace lexer;


void SymbolParser::entry() {}
void SymbolParser::exit() {}

void Idle::react(InputDigraph_D const &) {
    transit<Digraph_D>();
}
void Idle::react(InputGraph_G const & ){
    transit<Graph_G>();
}
void Idle::react(InputSpace const & ){
    transit<Idle>();
}
void Idle::react(InputNewLine const & ){
    transit<Idle>();
}

void Digraph_D::react(InputDigraph_i const &) {
    transit<Digraph_i>();
}
void Digraph_i::react(InputDigraph_g const &) {
    transit<Digraph_g>();
}
void Digraph_g::react(InputDigraph_r const &) {
    transit<Digraph_r>();
}
void Digraph_r::react(InputDigraph_a const &) {
    transit<Digraph_a>();
}
void Digraph_a::react(InputDigraph_p const &) {
    transit<Digraph_p>();
}
void Digraph_p::react(InputDigraph_h const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::GRAPH_START_LABEL,std::make_any<std::string>("digraph")});
    transit<Digraph_h>();
}
void Digraph_h::react(InputOpenCurlyBracket const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::OPEN_CURLY_BRACKET});
    SymbolParser::shared.flag_curly = 1;
    transit<OpenCurlyBracket>();
}
void Digraph_h::react(InputSpace const &) {
    transit<Digraph_h>();
}

void Graph_G::react(InputGraph_r const &) {
    transit<Graph_r>();
}
void Graph_r::react(InputGraph_a const &) {
    transit<Graph_a>();
}
void Graph_a::react(InputGraph_p const &) {
    transit<Graph_p>();
}
void Graph_p::react(InputGraph_h const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::GRAPH_START_LABEL,std::make_any<std::string>("graph")});
    transit<Graph_h>();
}
void Graph_h::react(InputOpenCurlyBracket const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::OPEN_CURLY_BRACKET});
    SymbolParser::shared.flag_curly = 1;
    transit<OpenCurlyBracket>();
}
void Graph_h::react(InputSpace const &) {
    transit<Graph_h>();
}

void OpenCurlyBracket::react(InputNodeId const &event) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::NODE_ID,std::make_any<std::string>(event.NodeId)});
    transit<NodeName>();
}
void OpenCurlyBracket::react(InputCloseCurlyBracket const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::CLOSED_CURLY_BRACKET});
    SymbolParser::shared.flag_curly = 0;
    transit<Idle>();
}
void OpenCurlyBracket::react(InputSpace const &) {
    transit<OpenCurlyBracket>();
}
void OpenCurlyBracket::react(InputNewLine const &) {
    transit<OpenCurlyBracket>();
}

void NodeName::react(InputOpenSquareBracket const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::OPEN_SQUARE_BRACKET});
    SymbolParser::shared.flag_square = 1;
    transit<OpenSquareBracket>();
}
void NodeName::react(InputHyphenFirst const &) {
    SymbolParser::shared.flag_hyphen = 1;
    transit<HyphenFirst>();
}
void NodeName::react(InputCloseCurlyBracket const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::CLOSED_CURLY_BRACKET});
    SymbolParser::shared.flag_curly = 0;
    transit<Idle>();
}
void NodeName::react(InputSpace const &) {
    transit<NodeName>();
}
void NodeName::react(InputNewLine const &) {
    transit<OpenCurlyBracket>();
}


void OpenSquareBracket::react(InputLabel_L const &) {
    SymbolParser::shared.flag_label_l = 1;
    transit<Label_L>();
}
void OpenSquareBracket::react(InputSpace const &) {
    transit<OpenSquareBracket>();
}
void OpenSquareBracket::react(InputCloseSquareBracket const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::CLOSED_SQUARE_BRACKET});
    SymbolParser::shared.flag_square = 0;
    transit<CloseSquareBracket>();
}

void HyphenFirst::react(InputHyphenSecond const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::FLAT_ARROW});
    SymbolParser::shared.flag_hyphen = 2;
    transit<HyphenSecond>();
}
void HyphenFirst::react(InputArrow const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::POINTED_ARROW});
    SymbolParser::shared.flag_hyphen = 2;
    transit<Arrow>();
}

void HyphenSecond::react(InputNodeIdSecond const &event) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::NODE_ID,std::make_any<std::string>(event.NodeIdSecond)});
    SymbolParser::shared.flag_hyphen = 0;
    transit<NodeNameSecond>();
}
void HyphenSecond::react(InputSpace const &) {
    transit<HyphenSecond>();
}

void Arrow::react(InputNodeIdSecond const &event) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::NODE_ID,std::make_any<std::string>(event.NodeIdSecond)});
    SymbolParser::shared.flag_hyphen = 0;
    transit<NodeNameSecond>();
}
void Arrow::react(InputSpace const &) {
    transit<Arrow>();
}

void NodeNameSecond::react(InputOpenSquareBracket const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::OPEN_SQUARE_BRACKET});
    SymbolParser::shared.flag_square = 1;
    transit<OpenSquareBracket>();
}
void NodeNameSecond::react(InputCloseCurlyBracket const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::CLOSED_CURLY_BRACKET});
    SymbolParser::shared.flag_curly = 0;
    transit<Idle>();
}
void NodeNameSecond::react(InputNewLine const &) {
    transit<OpenCurlyBracket>();
}
void NodeNameSecond::react(InputSpace const &) {
    transit<NodeNameSecond>();
}

void Label_L::react(InputLabel_a const &) {
    transit<Label_a>();
};
void Label_a::react(InputLabel_b const &) {
    transit<Label_b>();
};
void Label_b::react(InputLabel_e const &) {
    transit<Label_e>();
};
void Label_e::react(InputLabel_l const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::LABEL_ATTRIBUTE});
    SymbolParser::shared.flag_label_l = 0;
    transit<Label_l>();
};
void Label_l::react(InputEqualLabel const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::EQUALS_SIGN});
    SymbolParser::shared.flag_label = 1;
    transit<EqualLabel>();
};
void Label_l::react(InputSpace const &) {
    transit<Label_l>();
};

void EqualLabel::react(InputStringValue const &event) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::ATTRIBUTE_STRING_VALUE, std::make_any<std::string>(event.StringValue)});
    SymbolParser::shared.flag_label = 0;
    transit<StringValue>();
};
void EqualLabel::react(InputIntValue const &event) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::ATTRIBUTE_INT_VALUE, std::make_any<int>(event.IntValue)});
    SymbolParser::shared.flag_label = 0;
    SymbolParser::shared.token.clear();
    transit<IntValue>();
};
void EqualLabel::react(InputSpace const &) {
    transit<EqualLabel>();
};

void IntValue::react(InputCloseSquareBracket const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::CLOSED_SQUARE_BRACKET});
    SymbolParser::shared.flag_square = 0;
    transit<CloseSquareBracket>();
}
void IntValue::react(InputSpace const &) {
    transit<OpenSquareBracket>();
}

void StringValue::react(InputCloseSquareBracket const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::CLOSED_SQUARE_BRACKET});
    SymbolParser::shared.flag_square = 0;
    transit<CloseSquareBracket>();
};
void StringValue::react(InputSpace const &) {
    transit<OpenSquareBracket>();
};

void CloseSquareBracket::react(InputNodeId const &event) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::NODE_ID,std::make_any<std::string>(event.NodeId)});
    transit<NodeName>();
}
void CloseSquareBracket::react(InputCloseCurlyBracket const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::CLOSED_CURLY_BRACKET});
    SymbolParser::shared.flag_curly = 0;
    transit<Idle>();
}
void CloseSquareBracket::react(InputSpace const &) {
    transit<OpenCurlyBracket>();
}
void CloseSquareBracket::react(InputNewLine const &) {
    transit<OpenCurlyBracket>();
}


FSM_INITIAL_STATE(SymbolParser, Idle)

std::vector<common::Lexeme> lexer::lex(const std::string& input) {
    SymbolParser::start();

    for (const char& symbol : input) {
        if(symbol == 'd' && SymbolParser::shared.flag_curly == 0 && SymbolParser::is_in_state<Idle>()){
            SymbolParser::dispatch(InputDigraph_D());
            continue;
        }
        if(symbol == 'i' && SymbolParser::shared.flag_curly == 0 && SymbolParser::is_in_state<Digraph_D>()){
            SymbolParser::dispatch(InputDigraph_i());
            continue;
        }
        if(symbol == 'g' && SymbolParser::shared.flag_curly == 0 && SymbolParser::is_in_state<Digraph_i>()){
            SymbolParser::dispatch(InputDigraph_g());
            continue;
        }
        if(symbol == 'r' && SymbolParser::shared.flag_curly == 0 && SymbolParser::is_in_state<Digraph_g>()){
            SymbolParser::dispatch(InputDigraph_r());
            continue;
        }
        if(symbol == 'a' && SymbolParser::shared.flag_curly == 0 && SymbolParser::is_in_state<Digraph_r>()){
            SymbolParser::dispatch(InputDigraph_a());
            continue;
        }
        if(symbol == 'p' && SymbolParser::shared.flag_curly == 0 && SymbolParser::is_in_state<Digraph_a>()){
            SymbolParser::dispatch(InputDigraph_p());
            continue;
        }
        if(symbol == 'h' && SymbolParser::shared.flag_curly == 0 && SymbolParser::is_in_state<Digraph_p>()){
            SymbolParser::dispatch(InputDigraph_h());
            continue;
        }
        if(symbol == 'g' && SymbolParser::shared.flag_curly == 0 && SymbolParser::is_in_state<Idle>()){
            SymbolParser::dispatch(InputGraph_G());
            continue;
        }
        if(symbol == 'r' && SymbolParser::shared.flag_curly == 0 && SymbolParser::is_in_state<Graph_G>()){
            SymbolParser::dispatch(InputGraph_r());
            continue;
        }
        if(symbol == 'a' && SymbolParser::shared.flag_curly == 0 && SymbolParser::is_in_state<Graph_r>()){
            SymbolParser::dispatch(InputGraph_a());
            continue;
        }
        if(symbol == 'p' && SymbolParser::shared.flag_curly == 0 && SymbolParser::is_in_state<Graph_a>()){
            SymbolParser::dispatch(InputGraph_p());
            continue;
        }
        if(symbol == 'h' && SymbolParser::shared.flag_curly == 0 && SymbolParser::is_in_state<Graph_p>()){
            SymbolParser::dispatch(InputGraph_h());
            continue;
        }
        if(symbol == 'l' && SymbolParser::shared.flag_square == 1 && SymbolParser::shared.flag_label_l == 0 && SymbolParser::is_in_state<OpenSquareBracket>()){
            SymbolParser::dispatch(InputLabel_L());
            continue;
        }
        if(symbol == 'a' && SymbolParser::shared.flag_square == 1 && SymbolParser::is_in_state<Label_L>()){
            SymbolParser::dispatch(InputLabel_a());
            continue;
        }
        if(symbol == 'b' && SymbolParser::shared.flag_square == 1 && SymbolParser::is_in_state<Label_a>()){
            SymbolParser::dispatch(InputLabel_b());
            continue;
        }
        if(symbol == 'e' && SymbolParser::shared.flag_square == 1 && SymbolParser::is_in_state<Label_b>()){
            SymbolParser::dispatch(InputLabel_e());
            continue;
        }
        if(symbol == 'l' && SymbolParser::shared.flag_square == 1 && SymbolParser::shared.flag_label_l == 1 && SymbolParser::is_in_state<Label_e>()){
            SymbolParser::dispatch(InputLabel_l());
            continue;
        }
        if (symbol != '"' && SymbolParser::shared.flag_label == 1 && SymbolParser::shared.quotes_count == 1) {
            SymbolParser::shared.token += symbol;
        } else if (symbol == '"' && SymbolParser::shared.flag_label == 1) {
            SymbolParser::shared.token += symbol;
            SymbolParser::shared.quotes_count++;
            if (SymbolParser::shared.quotes_count == 2){
                SymbolParser::dispatch(InputStringValue{.StringValue = std::string(SymbolParser::shared.token.substr(1, SymbolParser::shared.token.size()-2))});
                SymbolParser::shared.token.clear();
                SymbolParser::shared.quotes_count = 0;
            }
        }
        if (isdigit(symbol) && SymbolParser::shared.flag_label == 1) {
            SymbolParser::shared.token += symbol;
            continue;
        } else if (!SymbolParser::shared.token.empty() && symbol == ' ' && SymbolParser::shared.flag_label == 1 && SymbolParser::shared.quotes_count == 0) {
            SymbolParser::dispatch(InputIntValue{.IntValue = std::stoi(SymbolParser::shared.token)});
            continue;
        } else if (!SymbolParser::shared.token.empty() && symbol == ']' && SymbolParser::shared.flag_label == 1 && SymbolParser::shared.quotes_count == 0) {
            SymbolParser::dispatch(InputIntValue{.IntValue = std::stoi(SymbolParser::shared.token)});
            SymbolParser::dispatch(InputCloseSquareBracket());
            continue;
        }

        if ((isalpha(symbol) || symbol == '_' ||  isdigit(symbol)) && SymbolParser::shared.flag_curly == 1 && SymbolParser::shared.flag_square == 0 && SymbolParser::shared.flag_hyphen == 0){
            SymbolParser::shared.token += symbol;
            continue;
        } else if(!SymbolParser::shared.token.empty() && SymbolParser::shared.flag_curly == 1 && SymbolParser::shared.flag_square == 0 && SymbolParser::shared.flag_hyphen == 0){
            if (symbol == ' ' ) {
                SymbolParser::dispatch(InputNodeId{.NodeId = SymbolParser::shared.token});
                SymbolParser::shared.token.clear();
                continue;
            } else if (symbol == '\n' ) {
                SymbolParser::dispatch(InputNodeId{.NodeId = SymbolParser::shared.token});
                SymbolParser::shared.token.clear();
                SymbolParser::dispatch(InputNewLine());
                continue;
            } else if (symbol == '[' ) {
                SymbolParser::dispatch(InputNodeId{.NodeId = SymbolParser::shared.token});
                SymbolParser::shared.token.clear();
                SymbolParser::dispatch(InputOpenSquareBracket());
                continue;
            } else if (symbol == '}' ) {
                SymbolParser::dispatch(InputNodeId{.NodeId = SymbolParser::shared.token});
                SymbolParser::shared.token.clear();
                SymbolParser::dispatch(InputCloseCurlyBracket());
                continue;
            } else if (symbol == '-' ) {
                SymbolParser::dispatch(InputNodeId{.NodeId = SymbolParser::shared.token});
                SymbolParser::shared.token.clear();
                SymbolParser::dispatch(InputHyphenFirst());
                continue;
            }
        }

        if ((isalpha(symbol) || symbol == '_' ||  isdigit(symbol)) && SymbolParser::shared.flag_curly == 1 && SymbolParser::shared.flag_square == 0 && SymbolParser::shared.flag_hyphen == 2){
            SymbolParser::shared.token += symbol;
            continue;
        } else if(!SymbolParser::shared.token.empty() && SymbolParser::shared.flag_curly == 1 && SymbolParser::shared.flag_square == 0 && SymbolParser::shared.flag_hyphen == 2){
            if (symbol == ' ' ) {
                SymbolParser::dispatch(InputNodeIdSecond{.NodeIdSecond = SymbolParser::shared.token});
                SymbolParser::shared.token.clear();
                continue;
            } else if (symbol == '\n' ) {
                SymbolParser::dispatch(InputNodeIdSecond{.NodeIdSecond = SymbolParser::shared.token});
                SymbolParser::shared.token.clear();
                SymbolParser::dispatch(InputNewLine());
                continue;
            } else if (symbol == '[' ) {
                SymbolParser::dispatch(InputNodeIdSecond{.NodeIdSecond = SymbolParser::shared.token});
                SymbolParser::shared.token.clear();
                SymbolParser::dispatch(InputOpenSquareBracket());
                continue;
            } else if (symbol == '}' ) {
                SymbolParser::dispatch(InputNodeIdSecond{.NodeIdSecond = SymbolParser::shared.token});
                SymbolParser::shared.token.clear();
                SymbolParser::dispatch(InputCloseCurlyBracket());
                continue;
            }
        }
        if(symbol == '{'){
            SymbolParser::dispatch(InputOpenCurlyBracket());
            continue;
        }
        if(symbol == '>' && SymbolParser::shared.flag_hyphen == 1){
            SymbolParser::dispatch(InputArrow());
            continue;
        }
        if(symbol == '=' && SymbolParser::is_in_state<Label_l>()){
            SymbolParser::dispatch(InputEqualLabel());
            continue;
        }
        if(symbol == '-' && SymbolParser::shared.flag_hyphen == 0){
            SymbolParser::dispatch(InputHyphenFirst());
            continue;
        }
        if(symbol == '-' && SymbolParser::shared.flag_hyphen == 1){
            SymbolParser::dispatch(InputHyphenSecond());
            continue;
        }
        if(symbol == '['){
            SymbolParser::dispatch(InputOpenSquareBracket());
            continue;
        }
        if(symbol == ']'){
            SymbolParser::dispatch(InputCloseSquareBracket());
            continue;
        }
        if(symbol == '}'){
            SymbolParser::dispatch(InputCloseCurlyBracket());
            continue;
        }
        if(symbol == ' '){
            SymbolParser::dispatch(InputSpace());
            continue;
        }
        if(symbol == '\n'){
            SymbolParser::dispatch(InputNewLine());
            continue;
        }
    }

    // Возвращаем результат лексического анализа
    return std::move(SymbolParser::shared.tokens);
}