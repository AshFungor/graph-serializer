#include "traversal.hpp"
#include <iostream>

using namespace common;

void TraversalGraph::tofWithTimestamps(const std::string& node, 
    std::unordered_set<std::string>& visited,
    std::vector<std::string>& result,
    std::unordered_map<std::string, std::pair<int, int>>& timestamps,
    int& discovery_time,
    int& flag) 
{
    visited.insert(node);
    timestamps[node].first = ++discovery_time;
    if (connections_->find(node) == connections_->end()) {
        return;
    }
    const auto& edges = connections_->at(node);

    for (const auto& edge : edges) {
        const std::string& neighbor = edge.peer;
        if (!visited.contains(neighbor)) {
            result.push_back(node);
            tofWithTimestamps(neighbor, visited, result, timestamps, discovery_time, flag);
        }
        
    }

    result.push_back(node);
    ++discovery_time;
    timestamps[node].second = discovery_time;
    if (!flag){
        this->setLabel(node, node + " : " + std::to_string(timestamps[node].first) + '/' + std::to_string(timestamps[node].second));
    }
}

void TraversalGraph::dfsWithTimestamps(const std::string& node,
    std::unordered_set<std::string>& visited,
    std::vector<std::string>& result,
    std::unordered_map<std::string, std::pair<int, int>>& timestamps,
    int& discovery_time,
    int& flag_to_quit,
    std::string& searched_node) 
{
    if (flag_to_quit){
        return;
    }
    visited.insert(node);
    timestamps[node].first = ++discovery_time;
    if (connections_->find(node) == connections_->end()) {
        return;
    }
    const auto& edges = connections_->at(node);
    if (node == searched_node){
        flag_to_quit = 1;
        result.push_back(node);
        ++discovery_time;
        timestamps[node].second = discovery_time;
        this->setLabel(node, node + " : " + std::to_string(timestamps[node].first) + '/' + std::to_string(timestamps[node].second));
        return;
    }
    for (const auto& edge : edges) {
            if (!(flag_to_quit)){
            const std::string& neighbor = edge.peer;
            if (!visited.contains(neighbor)) {
                result.push_back(node);
                dfsWithTimestamps(neighbor, visited, result, timestamps, discovery_time, flag_to_quit, searched_node);
            }
            } else{
                break;
            }
    }
    result.push_back(node);
    ++discovery_time;
    timestamps[node].second = discovery_time;
    this->setLabel(node, node + " : " + std::to_string(timestamps[node].first) + '/' + std::to_string(timestamps[node].second));
}

void TraversalGraph::showTraversalOrder() {
    std::unordered_set<std::string> visited;
    std::vector<std::string> traversalOrder;
    std::unordered_map<std::string, std::pair<int, int>> timestamps;
    std::vector<std::string> graph_nodes;
    std::string maximum_node;
    int discovery_time = 0;
    int flag = 1;
    int maximum = 0;

    graph_nodes = this->getNodes();

    for (int i = 0; i < graph_nodes.size(); i++){
        this->tofWithTimestamps(graph_nodes[i], visited, traversalOrder, timestamps, discovery_time, flag);
        if (visited.size() > maximum){
            maximum = visited.size();
            maximum_node = graph_nodes[i];
        } else if(visited.size() == maximum){
            maximum_node = std::min(maximum_node, graph_nodes[i]);
        }
    }
    visited.clear();
    traversalOrder.clear();
    timestamps.clear();
    discovery_time = 0;
    flag = 0;
    this->tofWithTimestamps(maximum_node, visited, traversalOrder, timestamps, discovery_time, flag);
    std::cout << "Graph traversal order: \n";
    for (const auto& node : traversalOrder) {
        if(this->getLabel(node).has_value()){
            std::cout << "Node : Timestamp - " + this->getLabel(node).value() << "\n";
        }
    }
}

void TraversalGraph::findNode(std::string& desired_node) {
    std::unordered_set<std::string> visited;
    std::vector<std::string> traversalOrder;
    std::unordered_map<std::string, std::pair<int, int>> timestamps;
    std::vector<std::string> graph_nodes;
    std::string maximum_node;
    int discovery_time = 0;
    int maximum = 0;
    int flag = 1;
    int flag_to_quit = 0;

    graph_nodes = this->getNodes();

    for (int i = 0; i < graph_nodes.size(); i++){
        if (this->getLabel(graph_nodes[i])){
            this->removeLabel(graph_nodes[i]);
        }
    }

    for (int i = 0; i < graph_nodes.size(); i++){
        this->tofWithTimestamps(graph_nodes[i], visited, traversalOrder, timestamps, discovery_time, flag);
        if (visited.size() > maximum){
            maximum = visited.size();
            maximum_node = graph_nodes[i];
        } else if(visited.size() == maximum){
            maximum_node = std::min(maximum_node, graph_nodes[i]);
        }
    }
    visited.clear();
    traversalOrder.clear();
    timestamps.clear();
    discovery_time = 0;
    this->dfsWithTimestamps(maximum_node, visited, traversalOrder, timestamps, discovery_time, flag_to_quit, desired_node);
}