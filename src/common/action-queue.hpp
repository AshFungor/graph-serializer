#pragma once

// standard
#include <memory>
#include <string>
#include <queue>

// local
#include "common.hpp"


namespace common {

    /**
     * @brief Callback Interface
     */
    class IAction {
    public:
        /**
         * @brief call to action
         */
        virtual void make() = 0;
    };

    /**
     * @brief PushNode action interface implementation
     */
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

    /**
     * @brief SetLabel action interface implementation
     */
    class SetLabelAction : public IAction {
    public:
        using set_node_label = void (common::Graph::*)(std::string, std::string);

        SetLabelAction(set_node_label action, Graph* instance, std::string name, std::string label);
        // IAction
        virtual void make() override final;
    
    private:
        set_node_label action_;
        Graph* this_;
        std::string name_;
        std::string label_;
    };

    /**
     * @brief PushEdge action interface implementation
     */
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

    /**
     * @brief Queue for actions
     * Stores actions and then allows to dump them when needed.
     */
    class ActionQueue {
    public:

        /**
         * @brief Dump all actions till queue is empty
         */
        void dumpAllActions();

        /**
         * @brief Dump single action from queue
         */
        void dumpAction();

        /**
         * @brief Add action to queue
         * @param action action to add
         */
        void query(std::shared_ptr<IAction> action);

    private:
        std::queue<std::shared_ptr<IAction>> actions_;
    };

}