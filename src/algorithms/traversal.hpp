#pragma once

#include <common/common.hpp>

namespace common {
    class TraversalGraph : public Graph {
    public:

        /**
         * @brief Recursive DFS with timestampes for traversal order
         * @param node Current processing node
         * @param visited Set of visited nodes
         * @param result Vector to preserve traversal order
         * @param timestamps Timestamps of current node
         * @param discovery_time Start time of current node
         */ 
        void tofWithTimestamps(
            const std::string& node,
            std::unordered_set<std::string>& visited,
            std::vector<std::string>& result,
            std::unordered_map<std::string, std::pair<int, int>>& timestamps,
            int& discovery_time,
            int& flag
        );

        /**
         * @brief Recursive DFS with timestampes for traversal order
         * @param node Current processing node
         * @param visited Set of visited nodes
         * @param result Vector to preserve traversal order
         * @param timestamps Timestamps of current node
         * @param discovery_time Start time of current node
         * @param flag_to_quit Flag to quit from recursion
         * @param searched_node Desired node
         */ 
        void dfsWithTimestamps(
            const std::string& node,
            std::unordered_set<std::string>& visited,
            std::vector<std::string>& result,
            std::unordered_map<std::string, std::pair<int, int>>& timestamps,
            int& discovery_time,
            int& flag_to_quit,
            std::string& searched_node
        );
        
        /**
         * @brief Find node with depth-first search of graph
         * @param desired_node Desired node of graph
        */
        void findNode(std::string& desired_node);

        /**
         * @brief Show depth-first search traversal order of graph
         */
        void showTraversalOrder();
    };
}