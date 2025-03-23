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

SetLabelAction::SetLabelAction(set_node_label action, Graph* instance, std::string name, std::string label) 
    : action_(std::move(action))
    , this_(std::move(instance))
    , name_(std::move(name))
    , label_(std::move(label))
{}

void SetLabelAction::make() {
    (this_->*action_)(std::move(name_), std::move(label_));
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

SetWeightAction::SetWeightAction(set_weight_proto action, Graph* instance, std::string source, std::string target, int weight)
    : action_(std::move(action))
    , this_(std::move(instance))
    , source_(std::move(source))
    , target_(std::move(target))
    , weight_(std::move(weight))
{}

void SetWeightAction::make() {
    (this_->*action_)(std::move(source_), std::move(target_), weight_);
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