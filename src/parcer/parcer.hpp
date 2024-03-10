#pragma once

// standard
#include <string>

// internal
#include <common/common.hpp>

// contrib
#include <tinyfsm.hpp>


namespace parcer {
    inline void throw_invalid_input() {
        throw std::runtime_error("Invalid graph type");
    }

    struct SharedMemory {
        std::string from_node_id;
        std::string to_node_id;
        std::string label;
        int weight = -1;
        std::string what_value_wait;
    };

    // ----------------------------------------------------------------------------
    // 1. Event Declarations
    //
    struct InputGraphType : tinyfsm::Event {
        common::Graph& graph; 
        std::string graph_type;
    };
    struct InputOpenCurlyBracket : tinyfsm::Event {
        common::Graph& graph;
    };
    struct InputCloseCurlyBracket : tinyfsm::Event {
    common::Graph& graph;
    };
    struct InputNodeId : tinyfsm::Event { common::Graph& graph; 
        std::string NodeID;
    };
    struct InputOpenSquareBracket : tinyfsm::Event {
        common::Graph& graph;
    };
    struct InputCloseSquareBracket : tinyfsm::Event {
        common::Graph& graph;
    };
    struct InputEdge : tinyfsm::Event {
        common::Graph& graph;
    };
    struct InputLabel : tinyfsm::Event {
        common::Graph& graph;
    };
    struct InputWeight : tinyfsm::Event {
        common::Graph& graph;
    };
    struct InputEqual : tinyfsm::Event {
        common::Graph& graph;
    };
    struct InputStringValue : tinyfsm::Event {
        common::Graph& graph;
        std::string label;
    };
    struct InputIntValue : tinyfsm::Event {
        common::Graph& graph;
        int weight;
    };


    // ----------------------------------------------------------------------------
    // 2. State Machine Base Class Declaration
    //
    class LexemeParcer : public tinyfsm::Fsm<LexemeParcer> {
        public:
        inline static SharedMemory shared;
        virtual void react(InputGraphType const &) {};
        virtual void react(InputOpenCurlyBracket const &) {throw_invalid_input();};
        virtual void react(InputCloseCurlyBracket const &) {throw_invalid_input();};
        virtual void react(InputNodeId const &) {throw_invalid_input();};
        virtual void react(InputOpenSquareBracket const &) {throw_invalid_input();};
        virtual void react(InputCloseSquareBracket const &) {throw_invalid_input();};
        virtual void react(InputEdge const &) {throw_invalid_input();};
        virtual void react(InputLabel const &) {throw_invalid_input();};
        virtual void react(InputWeight const &) {throw_invalid_input();};
        virtual void react(InputEqual const &) {throw_invalid_input();};
        virtual void react(InputStringValue const &) {throw_invalid_input();};
        virtual void react(InputIntValue const &) {throw_invalid_input();};


        void entry(void) { };  /* entry actions in some states */
        void exit(void)  { };  /* no exit actions */
    };


    // ----------------------------------------------------------------------------
    // 3. State Declarations
    //
    class Idle : public LexemeParcer {
        void react(InputGraphType const &) override;
    };

    class GraphType : public LexemeParcer {
        void react(InputOpenCurlyBracket const &) override;
    };

    class OpenCurlyBracket : public LexemeParcer {
        void react(InputNodeId const &) override;
        void react(InputCloseCurlyBracket const &) override;
    };

    class FromNodeID : public LexemeParcer {
        void react(InputCloseCurlyBracket const&) override;
        void react(InputOpenSquareBracket const&) override;
        void react(InputEdge const&) override;
    };

    class OpenSquareBracket : public LexemeParcer {
        void react(InputLabel const &) override;
        void react(InputWeight const &) override;
    };

    class Edge : public LexemeParcer {
        void react(InputNodeId const &) override;
    };

    class ToNodeID : public LexemeParcer {
        void react(InputOpenSquareBracket const &) override;
        void react(InputNodeId const& ) override;
        void react(InputCloseCurlyBracket const&) override;
    };

    class Label : public LexemeParcer {
        void react(InputEqual const &) override;
    };

    class Weight : public LexemeParcer {
        void react(InputEqual const &) override;
    };

    class Equal : public LexemeParcer {
        void react(InputStringValue const &) override;
        void react(InputIntValue const &) override;
    };

    class Value : public LexemeParcer {
        void react(InputCloseSquareBracket const &) override;
    };


    common::Graph parce(std::vector<common::Lexeme>& inpt);
}