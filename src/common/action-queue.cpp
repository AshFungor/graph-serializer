// standard
#include <string>
#include <queue>

// local
#include "common.hpp"
#include "action-queue.hpp"

using namespace common;


PushNodeAction::PushNodeAction(push_node_proto action, Graph* instance, std::string name) 
    : action_(std::move(action))
    , this_(std::move(instance))
    , name_(std::move(name))
{}

void PushNodeAction::make() {
    (this_->*action_)(std::move(name_));
}

PushEdgeAction::PushEdgeAction(push_edge_action action, Graph* instance, std::string name, Connection edge)
    : action_(std::move(action))
    , this_(std::move(instance))
    , name_(std::move(name))
    , edge_(std::move(edge))
{}

void PushEdgeAction::make() {
    (this_->*action_)(std::move(name_), std::move(edge_));
}

void ActionQueue::query(std::shared_ptr<IAction> action) {
    actions_.push(std::move(action));
}

void ActionQueue::dumpAllActions() {
    while (!actions_.empty()) {
        dumpAction();
    }
}

void ActionQueue::dumpAction() {
    actions_.front()->make();
    actions_.pop();
}