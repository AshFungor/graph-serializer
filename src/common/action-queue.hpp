#pragma once

// standard
#include <memory>
#include <string>
#include <queue>

// local
#include "common.hpp"


namespace common {

    class IAction {
    public:
        virtual void make() = 0;
    };

    class PushNodeAction : public IAction {
    public:
        using push_node_proto = void (common::Graph::*)(std::string);

        PushNodeAction(push_node_proto action, Graph* instance, std::string name);
        // IAction
        virtual void make() override final;
    
    private:
        push_node_proto action_;
        Graph* this_;
        std::string name_;
    };

    class PushEdgeAction : public IAction {
    public:
        using push_edge_action = void (common::Graph::*)(std::string, Connection);

        PushEdgeAction(push_edge_action action, Graph* instance, std::string name, Connection edge);
        // IAction
        virtual void make() override final;
    
    private:
        push_edge_action action_;
        Graph* this_;
        std::string name_;
        Connection edge_;
    };

    class ActionQueue {
    public:
        void dumpAllActions();
        void dumpAction();
        void query(std::shared_ptr<IAction> action);

    private:
        std::queue<std::shared_ptr<IAction>> actions_;
    };

}