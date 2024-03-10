#pragma once

#include <tinyfsm.hpp>
#include <iostream>
#include <string>
#include <common/common.hpp>


#define THROW_INVALID_INPUT() \
            std::cout<<"Invalid input"<<std::endl;


namespace parcer {
  struct SharedMemory {
    std::string from_node_id = "";
    std::string to_node_id = "";
    std::string label = "";
    int weight = -1;
    std::string what_value_wait = "";
  };

  // ----------------------------------------------------------------------------
  // 1. Event Declarations
  //
  struct InputGraphType : tinyfsm::Event { common::Graph& graph; 
                                            std::string graph_type; };
  struct InputOpenCurlyBracket : tinyfsm::Event { common::Graph& graph; };
  struct InputCloseCurlyBracket : tinyfsm::Event { common::Graph& graph; };
  struct InputNodeId : tinyfsm::Event { common::Graph& graph; 
                                        std::string NodeID; };
  struct InputOpenSquareBracket : tinyfsm::Event { common::Graph& graph; };
  struct InputCloseSquareBracket : tinyfsm::Event { common::Graph& graph; };
  struct InputEdge : tinyfsm::Event { common::Graph& graph; };
  struct InputLabel : tinyfsm::Event { common::Graph& graph; };
  struct InputWeight : tinyfsm::Event { common::Graph& graph; };
  struct InputEqual : tinyfsm::Event { common::Graph& graph; };
  struct InputStringValue : tinyfsm::Event { common::Graph& graph;
                                              std::string label; };
  struct InputIntValue : tinyfsm::Event { common::Graph& graph;
                                          int weight; };


  // ----------------------------------------------------------------------------
  // 2. State Machine Base Class Declaration
  //
  struct LexemeParcer : tinyfsm::Fsm<LexemeParcer>
  {
    inline static SharedMemory shared;
    virtual void react(InputGraphType const &) {};
    virtual void react(InputOpenCurlyBracket const &) {THROW_INVALID_INPUT()};
    virtual void react(InputCloseCurlyBracket const &) {THROW_INVALID_INPUT()};
    virtual void react(InputNodeId const &) {THROW_INVALID_INPUT()};
    virtual void react(InputOpenSquareBracket const &) {THROW_INVALID_INPUT()};
    virtual void react(InputCloseSquareBracket const &) {THROW_INVALID_INPUT()};
    virtual void react(InputEdge const &) {THROW_INVALID_INPUT()};
    virtual void react(InputLabel const &) {THROW_INVALID_INPUT()};
    virtual void react(InputWeight const &) {THROW_INVALID_INPUT()};
    virtual void react(InputEqual const &) {THROW_INVALID_INPUT()};
    virtual void react(InputStringValue const &) {THROW_INVALID_INPUT()};
    virtual void react(InputIntValue const &) {THROW_INVALID_INPUT()};


    void entry(void) { };  /* entry actions in some states */
    void exit(void)  { };  /* no exit actions */
  };


  // ----------------------------------------------------------------------------
  // 3. State Declarations
  //
  struct Idle : LexemeParcer
  {
    void react(InputGraphType const &) override;
  };

  struct GraphType : LexemeParcer
  {
    void react(InputOpenCurlyBracket const &) override;
  };

  struct OpenCurlyBracket : LexemeParcer
  {
    void react(InputNodeId const &) override;
    void react(InputCloseCurlyBracket const &) override;
  };

  struct FromNodeID : LexemeParcer
  {
    void react(InputCloseCurlyBracket const&) override;
    void react(InputOpenSquareBracket const&) override;
    void react(InputEdge const&) override;
  };

  struct OpenSquareBracket : LexemeParcer
  {
    void react(InputLabel const &) override;
    void react(InputWeight const &) override;
  };

  struct Edge : LexemeParcer
  {
    void react(InputNodeId const &) override;
  };

  struct ToNodeID : LexemeParcer
  {
    void react(InputOpenSquareBracket const &) override;
    void react(InputNodeId const& ) override;
    void react(InputCloseCurlyBracket const&) override;
  };

  struct Label : LexemeParcer
  {
    void react(InputEqual const &) override;
  };

  struct Weight : LexemeParcer
  {
    void react(InputEqual const &) override;
  };

  struct Equal : LexemeParcer
  {
    void react(InputStringValue const &) override;
    void react(InputIntValue const &) override;
  };

  struct Value : LexemeParcer
  {
    void react(InputCloseSquareBracket const &) override;
  };


  common::Graph parce(std::vector<common::Lexeme>& inpt);
}