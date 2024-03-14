// standard
#include <memory>
#include <stdexcept>

// common
#include <common/action-queue.hpp>
#include <common/common.hpp>

// local
#include "parser.hpp"

using namespace parser;


namespace {

    template<typename TAction, typename... Args>
    std::shared_ptr<common::IAction> makeAction(Args... args) {
        return std::make_shared<TAction>(args...);
    }
} // anon namespace

void LexemeParser::reset() {
    shared = SharedState();
    shared.graph = std::make_shared<common::Graph>();
}

void LexemeParser::entry() {}
void LexemeParser::exit() {}

void Idle::react(InputGraphType const &event) {
    if (event.graphType == "graph") {
        // Do nothing
    } else if (event.graphType == "digraph") {
        shared.flags |= common::opt::drc;
    } else {
        throw std::runtime_error("Invalid graph type, which is " + event.graphType);
    }
    transit<GraphType>();
}
    
void GraphType::react(InputOpenCurlyBracket const &) {
    transit<OpenCurlyBracket>();  
}

void OpenCurlyBracket::react(InputNodeId const &event) {
    LexemeParser::shared.fromNodeId = event.NodeID;
    transit<FromNodeID>();
}

void OpenCurlyBracket::react(InputCloseCurlyBracket const &) {
    transit<Idle>();
}

void FromNodeID::react(InputCloseCurlyBracket const &event) {
    shared.acQueue.query(makeAction<common::PushNodeAction>(
        &common::Graph::pushNode, 
        shared.graph.get(), 
        LexemeParser::shared.fromNodeId));
    transit<Idle>();
}

void FromNodeID::react(InputOpenSquareBracket const &) {
    LexemeParser::shared.expectedValue = "label";
    transit<OpenSquareBracket>();
}

void FromNodeID::react(InputEdge const &event) {
    shared.acQueue.query(makeAction<common::PushNodeAction>(
        &common::Graph::pushNode, 
        shared.graph.get(), 
        LexemeParser::shared.fromNodeId));
    transit<Edge>();
}

void OpenSquareBracket::react(InputLabel const &event) {
    if (LexemeParser::shared.expectedValue == "label") {
        transit<Label>();
    } else {
        throw_invalid_input("Expected label, found: " + LexemeParser::shared.expectedValue);
    }
}

void OpenSquareBracket::react(InputWeight const &event) {
    if (LexemeParser::shared.expectedValue == "weight") {
        transit<Weight>();
    } else {
        throw_invalid_input("Expected weight, found: " + LexemeParser::shared.expectedValue);
    }
}

void Edge::react(InputNodeId const &event) {
    LexemeParser::shared.toNodeId = event.NodeID;
    transit<ToNodeID>();
}

void ToNodeID::react(InputCloseCurlyBracket const &event) {
    if (!(shared.flags & common::opt::wgh)) {
        shared.acQueue.query(makeAction<common::PushNodeAction>(
            &common::Graph::pushNode, 
            shared.graph.get(), 
            LexemeParser::shared.toNodeId));
        shared.acQueue.query(makeAction<common::PushEdgeAction>(
            &common::Graph::pushEdge, 
            shared.graph.get(), 
            LexemeParser::shared.fromNodeId,
            common::Connection(LexemeParser::shared.toNodeId, 1)));
        transit<Idle>();
    } else {
        throw_invalid_input("What?");
    }
}

void ToNodeID::react(InputOpenSquareBracket const &) {
    LexemeParser::shared.expectedValue = "weight";
    transit<OpenSquareBracket>();
}

void ToNodeID::react(InputNodeId const &event) {
    if (!(shared.flags & common::opt::wgh)) {
        shared.acQueue.query(makeAction<common::PushNodeAction>(
            &common::Graph::pushNode, 
            shared.graph.get(), 
            LexemeParser::shared.toNodeId));
        shared.acQueue.query(makeAction<common::PushEdgeAction>(
            &common::Graph::pushEdge, 
            shared.graph.get(), 
            LexemeParser::shared.fromNodeId,
            common::Connection(LexemeParser::shared.toNodeId, LexemeParser::shared.weight)));
        transit<FromNodeID>();
    } else {
        throw_invalid_input("What?");
    }
}

void Label::react(InputEqual const &) {
    transit<Equal>();
}

void Weight::react(InputEqual const &) {
    transit<Equal>();
}

void Equal::react(InputStringValue const &event) {
    LexemeParser::shared.label = event.label;
    if (LexemeParser::shared.expectedValue == "label") {
        transit<Value>();
    } else {
        throw_invalid_input("Expected label, found: " + LexemeParser::shared.expectedValue);
    }
}

void Equal::react(InputIntValue const &event) {
    if (LexemeParser::shared.expectedValue == "weight") {
        LexemeParser::shared.weight = event.weight;
        transit<Value>();
    } else {
        throw_invalid_input("Expected weight, found: " + LexemeParser::shared.expectedValue);
    }
}

void Value::react(InputCloseSquareBracket const &event) {
    if (LexemeParser::shared.expectedValue == "weight") {
        shared.flags |= common::opt::wgh;
        shared.acQueue.query(makeAction<common::PushNodeAction>(
            &common::Graph::pushNode, 
            shared.graph.get(), 
            LexemeParser::shared.toNodeId));
        shared.acQueue.query(makeAction<common::PushEdgeAction>(
            &common::Graph::pushEdge, 
            shared.graph.get(), 
            LexemeParser::shared.fromNodeId,
            common::Connection(LexemeParser::shared.toNodeId, LexemeParser::shared.weight)));
    }
    transit<OpenCurlyBracket>();
}

// Set base state
FSM_INITIAL_STATE(LexemeParser, Idle)

std::shared_ptr<common::Graph> parser::parse(std::vector<common::Lexeme>& input) {
    LexemeParser::reset();
    LexemeParser::start();

    for (const auto& lexeme : input) {
        switch (lexeme.type) {
            case common::LexemeType::GRAPH_START_LABEL:
                LexemeParser::dispatch(InputGraphType{.graphType = std::any_cast<std::string>(lexeme.value)});
                break;

            case common::LexemeType::OPEN_CURLY_BRACKET:
                LexemeParser::dispatch(InputOpenCurlyBracket());
                break;

            case common::LexemeType::CLOSED_CURLY_BRACKET:
                LexemeParser::dispatch(InputCloseCurlyBracket());
                break;

            case common::LexemeType::NODE_ID:
                LexemeParser::dispatch(InputNodeId{.NodeID = std::any_cast<std::string>(lexeme.value)});
                break;

            case common::LexemeType::POINTED_ARROW:
                if (LexemeParser::shared.flags & common::opt::drc) {
                    LexemeParser::dispatch(InputEdge());
                } else {
                    throw_invalid_input("Graph is not directional!");
                }
                break;

            case common::LexemeType::FLAT_ARROW:
                if (!(LexemeParser::shared.flags & common::opt::drc)) {
                    LexemeParser::dispatch(InputEdge());
                } else {
                    throw_invalid_input("Graph is directional");
                }
                break;

            case common::LexemeType::OPEN_SQUARE_BRACKET:
                LexemeParser::dispatch(InputOpenSquareBracket());
                break;

            case common::LexemeType::CLOSED_SQUARE_BRACKET:
                LexemeParser::dispatch(InputCloseSquareBracket());
                break;

            case common::LexemeType::LABEL_ATTRIBUTE:
                LexemeParser::dispatch(InputLabel());
                break;

            case common::LexemeType::WEIGHT_ATTRIBUTE:
                LexemeParser::dispatch(InputWeight());
                break;

            case common::LexemeType::EQUALS_SIGN:
                LexemeParser::dispatch(InputEqual());
                break;

            case common::LexemeType::ATTRIBUTE_STRING_VALUE:
                LexemeParser::dispatch(InputStringValue{.label = std::any_cast<std::string>(lexeme.value)});
                break;

            case common::LexemeType::ATTRIBUTE_INT_VALUE:
                LexemeParser::dispatch(InputIntValue{.weight = std::any_cast<int>(lexeme.value)});
                break;

            default:
                throw_invalid_input("...");
        };
    }

    LexemeParser::shared.graph->init(LexemeParser::shared.flags);
    LexemeParser::shared.acQueue.dumpAllActions();
    return std::move(LexemeParser::shared.graph);
}