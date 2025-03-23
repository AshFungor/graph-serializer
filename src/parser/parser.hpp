#pragma once

// standard
#include <cstdint>
#include <memory>
#include <string>

// internal
#include <common/action-queue.hpp>
#include <common/common.hpp>
#include <algorithms/traversal.hpp>

// contrib
#include <tinyfsm.hpp>


namespace parser {
    inline void throw_invalid_input(std::string message) {
        throw std::runtime_error(message);
    }

    struct SharedState {
        std::shared_ptr<common::Graph> graph;

        common::ActionQueue backoffQueue;
        common::ActionQueue actionQueue;

        std::string fromNodeId;
        std::string toNodeId;
        std::string label;
        std::string expectedValue;

        std::uint8_t flags = 0x0;
        int weight = -1; 
    };

    // ----------------------------------------------------------------------------
    // 1. Event Declarations
    //
    struct GraphEvent               : tinyfsm::Event {};
    struct InputOpenCurlyBracket    : GraphEvent {};
    struct InputCloseCurlyBracket   : GraphEvent {};
    struct InputOpenSquareBracket   : GraphEvent {};
    struct InputCloseSquareBracket  : GraphEvent {};
    struct InputEdge                : GraphEvent {};
    struct InputLabel               : GraphEvent {};
    struct InputEqual               : GraphEvent {};
    struct InputGraphType           : GraphEvent { std::string graphType; };
    struct InputNodeId              : GraphEvent { std::string NodeID; };
    struct InputStringValue         : GraphEvent { std::string label; };
    struct InputIntValue            : GraphEvent { int weight; };


    // ----------------------------------------------------------------------------
    // 2. State Machine Base Class Declaration
    //
    class LexemeParser : public tinyfsm::Fsm<LexemeParser> {
    public:
        virtual void react(InputGraphType const &)          {};
        virtual void react(InputOpenCurlyBracket const &)   { throw_invalid_input(""); };
        virtual void react(InputCloseCurlyBracket const &)  { throw_invalid_input(""); };
        virtual void react(InputNodeId const &)             { throw_invalid_input(""); };
        virtual void react(InputOpenSquareBracket const &)  { throw_invalid_input(""); };
        virtual void react(InputCloseSquareBracket const &) { throw_invalid_input(""); };
        virtual void react(InputEdge const &)               { throw_invalid_input(""); };
        virtual void react(InputLabel const &)              { throw_invalid_input(""); };
        virtual void react(InputEqual const &)              { throw_invalid_input(""); };
        virtual void react(InputStringValue const &)        { throw_invalid_input(""); };
        virtual void react(InputIntValue const &)           { throw_invalid_input(""); };

        static void reset();
        void entry();  /* entry actions in some states */
        void exit();  /* no exit actions */
    
        friend std::shared_ptr<common::TraversalGraph> parse(std::vector<common::Lexeme>& input); // <- Возвращает TraversalGraph
        

    protected:
        inline static SharedState shared {};
    };


    // ----------------------------------------------------------------------------
    // 3. State Declarations
    //
    class Idle : public LexemeParser {
        void react(InputGraphType const &) override;
    };

    class GraphType : public LexemeParser {
        void react(InputOpenCurlyBracket const &) override;
    };

    class OpenCurlyBracket : public LexemeParser {
        void react(InputNodeId const &) override;
        void react(InputCloseCurlyBracket const &) override;
    };

    class FromNodeID : public LexemeParser {
        void react(InputCloseCurlyBracket const&) override;
        void react(InputOpenSquareBracket const&) override;
        void react(InputEdge const&) override;
        void react(InputNodeId const&) override;
    };

    class OpenSquareBracket : public LexemeParser {
        void react(InputLabel const &) override;
    };

    class Edge : public LexemeParser {
        void react(InputNodeId const &) override;
    };

    class ToNodeID : public LexemeParser {
        void react(InputOpenSquareBracket const &) override;
        void react(InputNodeId const& ) override;
        void react(InputCloseCurlyBracket const&) override;
    };

    class Label : public LexemeParser {
        void react(InputEqual const &) override;
    };

    class Equal : public LexemeParser {
        void react(InputStringValue const &) override;
        void react(InputIntValue const &) override;
    };

    class Value : public LexemeParser {
        void react(InputCloseSquareBracket const &) override;
    };

    /**
     * @brief Parse lexemes vector into graph object
     * @param input lexemes
     * @return std::shared_ptr<common::Graph> output object 
     */
    //std::shared_ptr<common::Graph> parse(std::vector<common::Lexeme>& input);
    std::shared_ptr<common::TraversalGraph> parse(std::vector<common::Lexeme>& input); // <- Возвращает TraversalGraph
}