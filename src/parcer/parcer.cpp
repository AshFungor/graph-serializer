#include <any>
#include <ostream>
#include <string>
#include "parcer.hpp"
#include "common/common.hpp"


void parcer::Idle::react(InputGraphType const& event)
{
  if (event.graph_type == "graph")
  {
    event.graph = common::Graph(nullptr);
  }
  else if (event.graph_type == "digraph")
  {
    event.graph = common::Graph(nullptr, common::opt::drc);
  }
  else 
  {
    std::cout << "Invalid graph type" << std::endl;
  }
  transit<parcer::GraphType>();
}


void parcer::GraphType::react(parcer::InputOpenCurlyBracket const &)
{
  transit<parcer::OpenCurlyBracket>();
}


void parcer::OpenCurlyBracket::react(InputNodeId const& event)
{
  parcer::LexemeParcer::shared.from_node_id = event.NodeID;
  transit<parcer::FromNodeID>();
}

void parcer::OpenCurlyBracket::react(InputCloseCurlyBracket const &)
{
  transit<parcer::Idle>();
}


void parcer::FromNodeID::react(parcer::InputCloseCurlyBracket const& event)
{
  event.graph.pushNode(parcer::LexemeParcer::shared.from_node_id);
  transit<parcer::Idle>();
}

void parcer::FromNodeID::react(parcer::InputOpenSquareBracket const&)
{
  parcer::LexemeParcer::shared.what_value_wait = "label";
  transit<parcer::OpenSquareBracket>();
}

void parcer::FromNodeID::react(parcer::InputEdge const& event)
{
  event.graph.pushNode(parcer::LexemeParcer::shared.from_node_id);
  transit<parcer::Edge>();
}


void parcer::OpenSquareBracket::react(parcer::InputLabel const& event)
{
  if (parcer::LexemeParcer::shared.what_value_wait == "label")
  {
    transit<parcer::Label>();
  }
  else {
    THROW_INVALID_INPUT()
  }
}


void parcer::OpenSquareBracket::react(parcer::InputWeight const& event)
{
  if (parcer::LexemeParcer::shared.what_value_wait == "weight")
  {
    transit<parcer::Weight>();
  }
  else {
    THROW_INVALID_INPUT()
  }
}


void parcer::Edge::react(parcer::InputNodeId const& event)
{
  parcer::LexemeParcer::shared.to_node_id = event.NodeID;
  transit<parcer::ToNodeID>();
}


void parcer::ToNodeID::react(parcer::InputCloseCurlyBracket const& event)
{
  if (event.graph.isWeighted() == false)
  {
    event.graph.pushNode(parcer::LexemeParcer::shared.to_node_id);
    event.graph.pushEdge(parcer::LexemeParcer::shared.from_node_id, {1, parcer::LexemeParcer::shared.to_node_id});
    transit<parcer::Idle>();
  }
  else {
    THROW_INVALID_INPUT()
  }
}

void parcer::ToNodeID::react(parcer::InputOpenSquareBracket const&)
{
  parcer::LexemeParcer::shared.what_value_wait = "weight";
  transit<parcer::OpenSquareBracket>();
}

void parcer::ToNodeID::react(parcer::InputNodeId const& event)
{
  if (event.graph.isWeighted() == false)
  {
    event.graph.pushNode(parcer::LexemeParcer::shared.to_node_id);
    event.graph.pushEdge(parcer::LexemeParcer::shared.from_node_id, {1, parcer::LexemeParcer::shared.to_node_id});
    transit<parcer::FromNodeID>();
  }
  else {
    THROW_INVALID_INPUT()
  }
}


void parcer::Label::react(parcer::InputEqual const&)
{
  transit<parcer::Equal>();
}


void parcer::Weight::react(parcer::InputEqual const&)
{
  transit<parcer::Equal>();
}


void parcer::Equal::react(parcer::InputStringValue const& event)
{

    parcer::LexemeParcer::shared.label = event.label;
  if (parcer::LexemeParcer::shared.what_value_wait == "label")
  {
    transit<parcer::Value>();
  }
  else {
    THROW_INVALID_INPUT()
  }
}


void parcer::Equal::react(parcer::InputIntValue const& event)
{
  if (parcer::LexemeParcer::shared.what_value_wait == "weight")
  {
    parcer::LexemeParcer::shared.weight = event.weight;
    transit<parcer::Value>();
  }
  else {
    THROW_INVALID_INPUT()
  }
}


void parcer::Value::react(parcer::InputCloseSquareBracket const& event)
{
  if (parcer::LexemeParcer::shared.what_value_wait == "weight")
  {
    if (event.graph.isWeighted() == false)
    {
    std::uint8_t flags = common::opt::wgh;
    if (event.graph.isDirectional() == true) flags |= common::opt::drc;
    event.graph.setFlags(flags);
    }

    event.graph.pushNode(parcer::LexemeParcer::shared.to_node_id);
    event.graph.pushEdge(parcer::LexemeParcer::shared.from_node_id, 
      {parcer::LexemeParcer::shared.weight, parcer::LexemeParcer::shared.to_node_id});
  }
  transit<parcer::OpenCurlyBracket>();
}


// Set base state
FSM_INITIAL_STATE(parcer::LexemeParcer, parcer::Idle)


common::Graph parcer::parce(std::vector<common::Lexeme>& inpt)
{
  common::Graph graph;
  parcer::LexemeParcer q = parcer::LexemeParcer();
  parcer::LexemeParcer::start();

  for (auto s: inpt)
  {
    switch (s.type)
    {
    case common::LexemeType::GRAPH_START_LABEL:
      parcer::LexemeParcer::dispatch(parcer::InputGraphType{.graph = graph, .graph_type = std::any_cast<std::string>(s.value)});
      break;
    
    case common::LexemeType::OPEN_CURLY_BRACKET:
      parcer::LexemeParcer::dispatch(parcer::InputOpenCurlyBracket{.graph = graph});
      break;

    case common::LexemeType::CLOSED_CURLY_BRACKET:
      parcer::LexemeParcer::dispatch(parcer::InputCloseCurlyBracket{.graph = graph});
      break;

    case common::LexemeType::NODE_ID:
      parcer::LexemeParcer::dispatch(parcer::InputNodeId{.graph = graph, .NodeID = std::any_cast<std::string>(s.value)});
      break;

    case common::LexemeType::POINTED_ARROW:
      if (graph.isDirectional() == true)
      {
        parcer::LexemeParcer::dispatch(parcer::InputEdge{.graph = graph});
      }
      else 
      {
        THROW_INVALID_INPUT()
      }
      break;

    case common::LexemeType::FLAT_ARROW:
      if (graph.isDirectional() == false)
      {
        parcer::LexemeParcer::dispatch(parcer::InputEdge{.graph = graph});
      }
      else 
      {
        THROW_INVALID_INPUT()
      }
      break;

    case common::LexemeType::OPEN_SQUARE_BRACKET:
      parcer::LexemeParcer::dispatch(parcer::InputOpenSquareBracket{.graph = graph});
      break;

    case common::LexemeType::CLOSED_SQUIRED_BRACKET:
      parcer::LexemeParcer::dispatch(parcer::InputCloseSquareBracket{.graph = graph});
      break;

    case common::LexemeType::LABEL_ATTRIBUTE:
      parcer::LexemeParcer::dispatch(parcer::InputLabel{.graph = graph});
      break;

    case common::LexemeType::WEIGHT_ATTRIBUTE:
      parcer::LexemeParcer::dispatch(parcer::InputWeight{.graph = graph});
      break;

    case common::LexemeType::EQUALS_SIGN:
      parcer::LexemeParcer::dispatch(parcer::InputEqual{.graph = graph});
      break;

    case common::LexemeType::ATTRIBUTE_STRING_VALUE:
      parcer::LexemeParcer::dispatch(parcer::InputStringValue{.graph = graph, .label = std::any_cast<std::string>(s.value)});
      break;

    case common::LexemeType::ATTRIBUTE_INT_VALUE:
      parcer::LexemeParcer::dispatch(parcer::InputIntValue{.graph = graph, .weight = std::any_cast<int>(s.value)});
      break;

    default:
      std::cout << "a" << std::endl;
      THROW_INVALID_INPUT()
    };
  }
  return graph;
}