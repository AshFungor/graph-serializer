// standard
#include <cstddef>
#include <unordered_set>
#include <unordered_map>
#include <string_view>
#include <algorithm>
#include <optional>
#include <utility>
#include <format>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <iostream>

// local
#include "common.hpp"

using namespace common;


// common.cpp
Connection::Connection(
    std::string peer, 
    std::optional<int> weight,
    std::optional<std::string> label  
) noexcept 
    : peer(std::move(peer))
    , weight(std::move(weight))
    , label(std::move(label)) {}  


Graph::Graph() noexcept
: connections_(std::make_unique<container_t>())
, labels_(std::make_unique<label_container_t>())
, flags_(0)
{}

void Graph::init(graph_flags_t flags) noexcept {
    if (connections_->empty() && !flags_) {
        flags_ = flags;
    }
}

// void Graph::setFlags(graph_flags_t flags) noexcept { flags_ = flags; }
bool Graph::isDirectional() const noexcept { return flags_ & opt::drc; }
bool Graph::isWeighted() const noexcept { return flags_ & opt::wgh; }

void Graph::pushNode(std::string name) {
    auto pos = connections_->find(name);
    if (pos == connections_->end()) {
        connections_->emplace(std::make_pair(std::move(name), std::vector<Connection>()));
    }
}

void Graph::pushEdge(std::string source, Connection edge) {
    if (isWeighted() && !edge.weight.has_value()) {
        throw std::runtime_error("Weight is required for weighted graph");
    }
    edge.weight = (isWeighted()) 
        ? ((edge.weight == std::nullopt) 
            ? 0 
            : edge.weight)
        : std::nullopt;
    insert(source, edge);
    if (!isDirectional()) {
        std::swap(source, edge.peer);
        insert(source, std::move(edge));
    }
}

void Graph::setLabel(std::string source, std::string label) {
    if (connections_->contains(source)) {
        if (!labels_->contains(source)) {
            labels_->insert(std::make_pair(std::move(source), std::move(label)));
        } else {
            labels_->at(source) = std::move(label);
        }
    }
}

void Graph::removeLabel(const std::string& source) {
    if (auto connections_it = connections_->find(source); 
        connections_it != connections_->end()) {
        
        if (auto labels_it = labels_->find(source); 
            labels_it != labels_->end()) {
            
            labels_->erase(labels_it);
        }
    }
}

std::optional<std::string> Graph::getLabel(std::string source) const {
    if (labels_->contains(source)) {
        return labels_->at(source);
    }
    return std::nullopt;
}

void Graph::insert(std::string_view source, Connection edge) {
    connections_->at(source.data()).emplace_back(std::move(edge));
}

Graph::connections_t::iterator Graph::findConnection(std::string_view source, std::string_view target) const{
    auto& conns = connections_->at(source.data());
    auto want = Connection(target.data());
    return std::find(conns.begin(), conns.end(), want);
}

bool Graph::areConnected(std::string_view source, std::string_view target) const{
    return findConnection(source, target) != connections_->at(source.data()).end();
}

std::optional<int> Graph::getWeight(std::string_view source, std::string_view target) const{
    return findConnection(source, target)->weight;
}

std::vector<std::string> Graph::getNodes() const{
    std::vector<std::string> nodes;
    for (const auto& pair : *connections_){
        nodes.push_back(pair.first);
    }
    return nodes;
}

std::string Graph::dumpGraphState() const {

    auto countEdges = [this]() {
        std::size_t result = 0;
        for (const auto& pair : *connections_) {
            result += pair.second.size();
        }
        return (isDirectional()) ? result : result / 2;
    };

    std::stringstream result;
    result << "Graph object at address " << (void*)this << "\n";
    result << "Flags: " << (isDirectional() ? "directional" : "undirectional") 
           << ", " << (isWeighted() ? "weighted" : "unweighted") 
           << ". Nodes: " << connections_->size() 
           << ", Edges: " << countEdges() << "\n";

    std::set<std::pair<std::string, std::string>> undirected_edges;

    for (const auto& [node, connections] : *connections_) {
        result << "Node [" << node << "], Label: ["
               << getLabel(node).value_or(node) << "]\n";
        
        for (const auto& conn : connections) {

            if (!isDirectional()) {
                auto normalized_edge = (node < conn.peer) 
                    ? std::make_pair(node, conn.peer)
                    : std::make_pair(conn.peer, node);
                
                if (undirected_edges.count(normalized_edge)) continue;
                undirected_edges.insert(normalized_edge);
            }

            const std::string arrow = isDirectional() ? "->" : "--";
            result << "  " << arrow << " " << conn.peer 
                   << " | Weight: " << conn.weight.value_or(0)
                   << " | Label: " 
                   << (conn.label.has_value() 
                        ? "\"" + conn.label.value() + "\"" 
                        : "N/A")
                   << "\n";
        }
    }

    return result.str();
}

std::ostream& common::operator<<(std::ostream& os, const Graph& graph) {
    return os << graph.dumpGraphState();
}