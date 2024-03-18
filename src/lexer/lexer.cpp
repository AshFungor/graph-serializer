// standard
#include <vector>

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
    SymbolParser::shared.tokens.push_back({common::LexemeType::GRAPH_START_LABEL,"digraph"});
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
    SymbolParser::shared.tokens.push_back({common::LexemeType::GRAPH_START_LABEL,"graph"});
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
    SymbolParser::shared.tokens.push_back({common::LexemeType::NODE_ID,event});
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
void NodeName::react(InputSpace const &) {
    transit<NodeName>();
}

void OpenSquareBracket::react(InputWeight_W const &) {
    transit<Weight_W>();
}
void OpenSquareBracket::react(InputLabel_L const &) {
    SymbolParser::shared.flag_label_l = 1;
    transit<Label_L>();
}
void OpenSquareBracket::react(InputSpace const &) {
    transit<OpenSquareBracket>();
}
void OpenSquareBracket::react(InputCloseSquareBracket const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::CLOSED_SQUARED_BRACKET});
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
    SymbolParser::shared.tokens.push_back({common::LexemeType::NODE_ID,event});
    SymbolParser::shared.flag_hyphen = 0;
    transit<NodeNameSecond>();
}
void HyphenSecond::react(InputSpace const &) {
    transit<HyphenSecond>();
}

void Arrow::react(InputNodeIdSecond const &event) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::NODE_ID,event});
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
void NodeNameSecond::react(InputNewLine const &) {
    transit<OpenCurlyBracket>();
}
void NodeNameSecond::react(InputSpace const &) {
    transit<NodeNameSecond>();
}

void Weight_W::react(InputWeight_e const &) {
    transit<Weight_e>();
};
void Weight_e::react(InputWeight_i const &) {
    transit<Weight_i>();
};
void Weight_i::react(InputWeight_g const &) {
    transit<Weight_g>();
};
void Weight_g::react(InputWeight_h const &) {
    transit<Weight_h>();
};
void Weight_h::react(InputWeight_t const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::WEIGHT_ATTRIBUTE});
    SymbolParser::shared.flag_weight = 1;
    transit<Weight_t>();
};
void Weight_t::react(InputEqualWeight const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::EQUALS_SIGN});
    transit<EqualWeight>();
};
void Weight_t::react(InputSpace const &) {
    transit<Weight_t>();
};

void EqualWeight::react(InputIntValue const &event) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::ATTRIBUTE_INT_VALUE, event});
    SymbolParser::shared.flag_weight = 0;
    transit<IntValue>();
};
void EqualWeight::react(InputSpace const &) {
    transit<EqualWeight>();
};

void IntValue::react(InputCloseSquareBracket const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::CLOSED_SQUARED_BRACKET});
    SymbolParser::shared.flag_square = 0;
    transit<CloseSquareBracket>();
}
void IntValue::react(InputSpace const &) {
    transit<OpenSquareBracket>();
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
    SymbolParser::shared.flag_label = 1;
    SymbolParser::shared.flag_label_l = 0;
    transit<Label_l>();
};
void Label_l::react(InputEqualLabel const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::EQUALS_SIGN});
    transit<EqualLabel>();
};
void Label_l::react(InputSpace const &) {
    transit<Label_l>();
};

void EqualLabel::react(InputStringValue const &event) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::ATTRIBUTE_STRING_VALUE, event});
    SymbolParser::shared.flag_label = 0;
    transit<StringValue>();
};
void EqualLabel::react(InputSpace const &) {
    transit<EqualLabel>();
};

void StringValue::react(InputCloseSquareBracket const &) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::CLOSED_SQUARED_BRACKET});
    SymbolParser::shared.flag_square = 0;
    transit<CloseSquareBracket>();
};
void StringValue::react(InputSpace const &) {
    transit<OpenSquareBracket>();
};

void CloseSquareBracket::react(InputNodeId const &event) {
    SymbolParser::shared.tokens.push_back({common::LexemeType::NODE_ID,event});
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
    transit<OpenSquareBracket>();
}


FSM_INITIAL_STATE(SymbolParser, Idle)

std::vector<common::Lexeme> lexer::lex(const std::string& input) {
    SymbolParser::start();

    for (char symbol : input) {
        if(symbol == 'd' && SymbolParser::shared.flag_curly == 0){
            SymbolParser::dispatch(InputDigraph_D());
        }
        if(symbol == 'i' && SymbolParser::shared.flag_curly == 0){
            SymbolParser::dispatch(InputDigraph_i());
        }
        if(symbol == 'g' && SymbolParser::shared.flag_curly == 0){
            SymbolParser::dispatch(InputDigraph_g());
        }
        if(symbol == 'r' && SymbolParser::shared.flag_curly == 0){
            SymbolParser::dispatch(InputDigraph_r());
        }
        if(symbol == 'a' && SymbolParser::shared.flag_curly == 0){
            SymbolParser::dispatch(InputDigraph_a());
        }
        if(symbol == 'p' && SymbolParser::shared.flag_curly == 0){
            SymbolParser::dispatch(InputDigraph_p());
        }
        if(symbol == 'h' && SymbolParser::shared.flag_curly == 0){
            SymbolParser::dispatch(InputDigraph_h());
        }
        if(symbol == 'g' && SymbolParser::shared.flag_curly == 0){
            SymbolParser::dispatch(InputGraph_G());
        }
        if(symbol == 'r' && SymbolParser::shared.flag_curly == 0){
            SymbolParser::dispatch(InputGraph_r());
        }
        if(symbol == 'a' && SymbolParser::shared.flag_curly == 0){
            SymbolParser::dispatch(InputGraph_a());
        }
        if(symbol == 'p' && SymbolParser::shared.flag_curly == 0){
            SymbolParser::dispatch(InputGraph_p());
        }
        if(symbol == 'h' && SymbolParser::shared.flag_curly == 0){
            SymbolParser::dispatch(InputGraph_h());
        }
        if(symbol == '['){
            SymbolParser::dispatch(InputOpenSquareBracket());
        }
        if(symbol == '{'){
            SymbolParser::dispatch(InputOpenCurlyBracket());
        }
        if(symbol == ']'){
            SymbolParser::dispatch(InputCloseSquareBracket());
        }
        if(symbol == '}'){
            SymbolParser::dispatch(InputCloseCurlyBracket());
        }
        if(symbol == '-' && SymbolParser::shared.flag_hyphen == 0){
            SymbolParser::dispatch(InputHyphenFirst());
        }
        if(symbol == '-' && SymbolParser::shared.flag_hyphen == 1){
            SymbolParser::dispatch(InputHyphenSecond());
        }
        if(symbol == '>' && SymbolParser::shared.flag_hyphen == 1){
            SymbolParser::dispatch(InputArrow());
        }
        if(symbol == '=' && SymbolParser::shared.flag_label == 1){
            SymbolParser::dispatch(InputEqualLabel());
        }
        if(symbol == '=' && SymbolParser::shared.flag_weight == 1){
            SymbolParser::dispatch(InputEqualWeight());
        }
        if(symbol == 'w' && SymbolParser::shared.flag_square == 1){
            SymbolParser::dispatch(InputWeight_W());
        }
        if(symbol == 'e' && SymbolParser::shared.flag_square == 1){
            SymbolParser::dispatch(InputWeight_e());
        }
        if(symbol == 'i' && SymbolParser::shared.flag_square == 1){
            SymbolParser::dispatch(InputWeight_i());
        }
        if(symbol == 'g' && SymbolParser::shared.flag_square == 1){
            SymbolParser::dispatch(InputWeight_g());
        }
        if(symbol == 'h' && SymbolParser::shared.flag_square == 1){
            SymbolParser::dispatch(InputWeight_h());
        }
        if(symbol == 't' && SymbolParser::shared.flag_square == 1){
            SymbolParser::dispatch(InputWeight_t());
        }
        if(symbol == 'l' && SymbolParser::shared.flag_square == 1 && SymbolParser::shared.flag_label_l == 0){
            SymbolParser::dispatch(InputLabel_L());
        }
        if(symbol == 'a' && SymbolParser::shared.flag_square == 1){
            SymbolParser::dispatch(InputLabel_a());
        }
        if(symbol == 'b' && SymbolParser::shared.flag_square == 1){
            SymbolParser::dispatch(InputLabel_b());
        }
        if(symbol == 'e' && SymbolParser::shared.flag_square == 1){
            SymbolParser::dispatch(InputLabel_e());
        }
        if(symbol == 'l' && SymbolParser::shared.flag_square == 1 && SymbolParser::shared.flag_label_l == 1){
            SymbolParser::dispatch(InputLabel_l());
        }
        if (SymbolParser::shared.token.empty() && symbol == '"' && SymbolParser::shared.flag_label == 1) {
            SymbolParser::shared.token += symbol;
        } else if (!SymbolParser::shared.token.empty() && symbol != '"' && SymbolParser::shared.flag_label == 1) {
            SymbolParser::shared.token += symbol;
        } else if (!SymbolParser::shared.token.empty() && symbol == '"' && SymbolParser::shared.flag_label == 1) {
            SymbolParser::shared.token += symbol;
            SymbolParser::dispatch(InputStringValue{.StringValue = SymbolParser::shared.token});
            SymbolParser::shared.token.clear();
        }
        if (SymbolParser::shared.token.empty() && isdigit(symbol) && SymbolParser::shared.flag_weight == 1) {
            SymbolParser::shared.token += symbol;
        } else if (!SymbolParser::shared.token.empty() && isdigit(symbol) && SymbolParser::shared.flag_weight == 1) {
            SymbolParser::shared.token += symbol;
        } else if (!SymbolParser::shared.token.empty() && symbol == ' ' && SymbolParser::shared.flag_weight == 1) {
            SymbolParser::dispatch(InputIntValue{.IntValue = std::any_cast<int>(SymbolParser::shared.token)});
            SymbolParser::shared.token.clear();
        }
        if (SymbolParser::shared.token.empty() && (isalpha(symbol) || symbol == '_' ||  isdigit(symbol)) && SymbolParser::shared.flag_curly == 1 && SymbolParser::shared.flag_square == 0 && SymbolParser::shared.flag_hyphen == 0){
            SymbolParser::shared.token += symbol;
        } else if(!SymbolParser::shared.token.empty() && (isalpha(symbol) || symbol == '_' ||  isdigit(symbol)) && SymbolParser::shared.flag_curly == 1 && SymbolParser::shared.flag_square == 0 && SymbolParser::shared.flag_hyphen == 0){
            SymbolParser::shared.token += symbol;
        } else if(!SymbolParser::shared.token.empty() && symbol == ' ' && SymbolParser::shared.flag_curly == 1 && SymbolParser::shared.flag_square == 0 && SymbolParser::shared.flag_hyphen == 0){
            SymbolParser::dispatch(InputNodeId{.NodeId = SymbolParser::shared.token});
            SymbolParser::shared.token.clear();
        }
        if (SymbolParser::shared.token.empty() && (isalpha(symbol) || symbol == '_' ||  isdigit(symbol)) && SymbolParser::shared.flag_curly == 1 && SymbolParser::shared.flag_square == 0 && SymbolParser::shared.flag_hyphen == 2){
            SymbolParser::shared.token += symbol;
        } else if(!SymbolParser::shared.token.empty() && (isalpha(symbol) || symbol == '_' ||  isdigit(symbol)) && SymbolParser::shared.flag_curly == 1 && SymbolParser::shared.flag_square == 0 && SymbolParser::shared.flag_hyphen == 2){
            SymbolParser::shared.token += symbol;
        } else if(!SymbolParser::shared.token.empty() && symbol == ' ' && SymbolParser::shared.flag_curly == 1 && SymbolParser::shared.flag_square == 0 && SymbolParser::shared.flag_hyphen == 2){
            SymbolParser::dispatch(InputNodeIdSecond{.NodeIdSecond = SymbolParser::shared.token});
            SymbolParser::shared.token.clear();
        }
         if(symbol == ' '){
            SymbolParser::dispatch(InputSpace());
        }
        if(symbol == '\n'){
            SymbolParser::dispatch(InputNewLine());
        }
    }

    // Возвращаем результат лексического анализа
    return std::move(SymbolParser::shared.tokens);
}