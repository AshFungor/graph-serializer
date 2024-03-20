#pragma once

// standard
#include <vector>
#include <string>

// internal
#include <common/common.hpp>

// contrib
#include <tinyfsm.hpp>

namespace lexer {
    inline void throw_invalid_input(std::string message) {
        throw std::runtime_error(message);
    }

    struct SharedState {
        std::vector<common::Lexeme> tokens;
        std::string token;

        int flag_label = 0;
        int flag_weight = 0;
        int flag_curly = 0;
        int flag_square = 0;
        int flag_hyphen = 0;
        int flag_label_l = 0;
    };

    // ----------------------------------------------------------------------------
    // 1. Event Declarations
    //
    struct lexemeEvent               : tinyfsm::Event {};
    struct InputDigraph_D            : lexemeEvent {};
    struct InputDigraph_i            : lexemeEvent {};
    struct InputDigraph_g            : lexemeEvent {};
    struct InputDigraph_r            : lexemeEvent {};
    struct InputDigraph_a            : lexemeEvent {};
    struct InputDigraph_p            : lexemeEvent {};
    struct InputDigraph_h            : lexemeEvent {};
    struct InputGraph_G              : lexemeEvent {};
    struct InputGraph_r              : lexemeEvent {};
    struct InputGraph_a              : lexemeEvent {};
    struct InputGraph_p              : lexemeEvent {};
    struct InputGraph_h              : lexemeEvent {};
    struct InputOpenCurlyBracket     : lexemeEvent {};
    struct InputCloseCurlyBracket    : lexemeEvent {};
    struct InputOpenSquareBracket    : lexemeEvent {};
    struct InputCloseSquareBracket   : lexemeEvent {};
    struct InputHyphenFirst          : lexemeEvent {};
    struct InputHyphenSecond         : lexemeEvent {};
    struct InputArrow                : lexemeEvent {};
    struct InputWeight_W             : lexemeEvent {};
    struct InputWeight_e             : lexemeEvent {};
    struct InputWeight_i             : lexemeEvent {};
    struct InputWeight_g             : lexemeEvent {};
    struct InputWeight_h             : lexemeEvent {};
    struct InputWeight_t             : lexemeEvent {};
    struct InputLabel_L              : lexemeEvent {};
    struct InputLabel_a              : lexemeEvent {};
    struct InputLabel_b              : lexemeEvent {};
    struct InputLabel_e              : lexemeEvent {};
    struct InputLabel_l              : lexemeEvent {};
    struct InputEqualWeight          : lexemeEvent {};
    struct InputEqualLabel           : lexemeEvent {};
    struct InputSpace                : lexemeEvent {};
    struct InputNewLine              : lexemeEvent {};
    struct InputNodeId               : lexemeEvent {std::string NodeId; };
    struct InputNodeIdSecond         : lexemeEvent {std::string NodeIdSecond; };
    struct InputStringValue          : lexemeEvent {std::string StringValue; };
    struct InputIntValue             : lexemeEvent {int IntValue; };

    // ----------------------------------------------------------------------------
    // 2. State Machine Base Class Declaration
    //
    class SymbolParser : public tinyfsm::Fsm<SymbolParser> {
    public:
        virtual void react(InputDigraph_D const &)          { throw_invalid_input(""); };
        virtual void react(InputDigraph_i const &)          { throw_invalid_input(""); };
        virtual void react(InputDigraph_g const &)          { throw_invalid_input(""); };
        virtual void react(InputDigraph_r const &)          { throw_invalid_input(""); };
        virtual void react(InputDigraph_a const &)          { throw_invalid_input(""); };
        virtual void react(InputDigraph_p const &)          { throw_invalid_input(""); };
        virtual void react(InputDigraph_h const &)          { throw_invalid_input(""); };
        virtual void react(InputGraph_G const &)            { throw_invalid_input(""); };
        virtual void react(InputGraph_r const &)            { throw_invalid_input(""); };
        virtual void react(InputGraph_a const &)            { throw_invalid_input(""); };
        virtual void react(InputGraph_p const &)            { throw_invalid_input(""); };
        virtual void react(InputGraph_h const &)            { throw_invalid_input(""); };
        virtual void react(InputOpenCurlyBracket const &)   { throw_invalid_input(""); };
        virtual void react(InputCloseCurlyBracket const &)  { throw_invalid_input(""); };
        virtual void react(InputNodeId const &)             { throw_invalid_input(""); };
        virtual void react(InputNodeIdSecond const &)       { throw_invalid_input(""); };
        virtual void react(InputSpace const &)              { throw_invalid_input(""); };
        virtual void react(InputNewLine const &)            { throw_invalid_input(""); };
        virtual void react(InputOpenSquareBracket const &)  { throw_invalid_input(""); };
        virtual void react(InputCloseSquareBracket const &) { throw_invalid_input(""); };
        virtual void react(InputHyphenFirst  const &)       { throw_invalid_input(""); };
        virtual void react(InputHyphenSecond const &)       { throw_invalid_input(""); };
        virtual void react(InputArrow const &)              { throw_invalid_input(""); };
        virtual void react(InputWeight_W const &)           { throw_invalid_input(""); };
        virtual void react(InputWeight_e const &)           { throw_invalid_input(""); };
        virtual void react(InputWeight_i const &)           { throw_invalid_input(""); };
        virtual void react(InputWeight_g const &)           { throw_invalid_input(""); };
        virtual void react(InputWeight_h const &)           { throw_invalid_input(""); };
        virtual void react(InputWeight_t const &)           { throw_invalid_input(""); };
        virtual void react(InputLabel_L  const &)           { throw_invalid_input(""); };
        virtual void react(InputLabel_a  const &)           { throw_invalid_input(""); };
        virtual void react(InputLabel_b  const &)           { throw_invalid_input(""); };
        virtual void react(InputLabel_e  const &)           { throw_invalid_input(""); };
        virtual void react(InputLabel_l  const &)           { throw_invalid_input(""); };
        virtual void react(InputEqualWeight const &)        { throw_invalid_input(""); };
        virtual void react(InputEqualLabel const &)         { throw_invalid_input(""); };
        virtual void react(InputStringValue const &)        { throw_invalid_input(""); };
        virtual void react(InputIntValue const &)           { throw_invalid_input(""); };

        static void reset();
        void entry();  /* entry actions in some states */
        void exit();  /* no exit actions */

        friend std::vector<common::Lexeme> lex(const std::string& input);
    
        inline static SharedState shared {};
    };

    // ----------------------------------------------------------------------------
    // 3. State Declarations
    //
    class Idle : public SymbolParser {
        void react(InputDigraph_D const &) override;
        void react(InputGraph_G const &) override;
        void react(InputSpace const &) override;
        void react(InputNewLine const &) override;
    };

    class Digraph_D : public SymbolParser {
        void react(InputDigraph_i const &) override;
    };
    class Digraph_i : public SymbolParser {
        void react(InputDigraph_g const &) override;
    };
    class Digraph_g : public SymbolParser {
        void react(InputDigraph_r const &) override;
    };
    class Digraph_r : public SymbolParser {
        void react(InputDigraph_a const &) override;
    };
    class Digraph_a : public SymbolParser {
        void react(InputDigraph_p const &) override;
    };
    class Digraph_p : public SymbolParser {
        void react(InputDigraph_h const &) override;
    };
    class Digraph_h : public SymbolParser {
        void react(InputOpenCurlyBracket const &) override;
        void react(InputSpace const &) override;
    };

    class Graph_G : public SymbolParser {
        void react(InputGraph_r const &) override;
    };
    class Graph_r : public SymbolParser {
        void react(InputGraph_a const &) override;
    };
    class Graph_a : public SymbolParser {
        void react(InputGraph_p const &) override;
    };
    class Graph_p : public SymbolParser {
        void react(InputGraph_h const &) override;
    };
    class Graph_h : public SymbolParser {
        void react(InputOpenCurlyBracket const &) override;
        void react(InputSpace const &) override;
    };

    class OpenCurlyBracket : public SymbolParser {
        void react(InputNodeId const &) override;
        void react(InputCloseCurlyBracket const &) override;
        void react(InputSpace const &) override;
        void react(InputNewLine const &) override;
    };

    class NodeName : public SymbolParser {
        void react(InputOpenSquareBracket const &) override;
        void react(InputCloseCurlyBracket const &) override;
        void react(InputHyphenFirst const &) override;
        void react(InputSpace const &) override;
        void react(InputNewLine const &) override;
    };

    class OpenSquareBracket : public SymbolParser {
        void react(InputWeight_W const &) override;
        void react(InputLabel_L const &) override;
        void react(InputSpace const &) override;
        void react(InputCloseSquareBracket const &) override;
    };

    class HyphenFirst : public SymbolParser {
        void react(InputHyphenSecond const &) override;
        void react(InputArrow const &) override;
    };

    class HyphenSecond : public SymbolParser {
        void react(InputNodeIdSecond const &) override;
        void react(InputSpace const &) override;
    };

    class Arrow : public SymbolParser {
        void react(InputNodeIdSecond const &) override;
        void react(InputSpace const &) override;
    };

    class NodeNameSecond : public SymbolParser {
        void react(InputOpenSquareBracket const &) override;
        void react(InputCloseCurlyBracket const &) override;
        void react(InputNewLine const &) override;
        void react(InputSpace const &) override;
    };

    class Weight_W : public SymbolParser {
        void react(InputWeight_e const &) override;
    };
    class Weight_e : public SymbolParser {
        void react(InputWeight_i const &) override;
    };
    class Weight_i : public SymbolParser {
        void react(InputWeight_g const &) override;
    };
    class Weight_g : public SymbolParser {
        void react(InputWeight_h const &) override;
    };
    class Weight_h : public SymbolParser {
        void react(InputWeight_t const &) override;
    };
    class Weight_t : public SymbolParser {
        void react(InputEqualWeight const &) override;
        void react(InputSpace const &) override;
    };
    class EqualWeight : public SymbolParser {
        void react(InputIntValue const &) override;
        void react(InputSpace const &) override;
    };
    class IntValue : public SymbolParser {
        void react(InputCloseSquareBracket const &) override;
        void react(InputSpace const &) override;
    };

    class Label_L : public SymbolParser {
        void react(InputLabel_a const &) override;
    };
    class Label_a : public SymbolParser {
        void react(InputLabel_b const &) override;
    };
    class Label_b : public SymbolParser {
        void react(InputLabel_e const &) override;
    };
    class Label_e : public SymbolParser {
        void react(InputLabel_l const &) override;
    };
    class Label_l : public SymbolParser {
        void react(InputEqualLabel const &) override;
        void react(InputSpace const &) override;
    };
    class EqualLabel : public SymbolParser {
        void react(InputStringValue const &) override;
        void react(InputSpace const &) override;
    };
    class StringValue : public SymbolParser {
        void react(InputCloseSquareBracket const &) override;
        void react(InputSpace const &) override;
    };

    class CloseSquareBracket : public SymbolParser {
        void react(InputNodeId const &) override;
        void react(InputCloseCurlyBracket const &) override;
        void react(InputSpace const &) override;
        void react(InputNewLine const &) override;
    };

    std::vector<common::Lexeme> lex(const std::string& input);
}