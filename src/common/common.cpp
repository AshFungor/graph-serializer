// standard
#include <unordered_map>
#include <string_view>
#include <algorithm>
#include <optional>
#include <cstdint>
#include <utility>
#include <memory>
#include <string>
#include <vector>
#include <any>

// local
#include "common.hpp"

using namespace common;


Connection::Connection(std::string peer, std::optional<int> weight) noexcept
    : peer(std::move(peer))
    , weight(std::move(weight))
{}

bool Connection::operator==(const Connection& other) const {
    return peer == other.peer;
}

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

std::optional<std::string> Graph::getLabel(std::string source) {
    if (labels_->contains(source)) {
        return labels_->at(source);
    }
    return std::nullopt;
}

void Graph::insert(std::string_view source, Connection edge) {
    connections_->at(source.data()).emplace_back(std::move(edge));
}

Graph::connections_t::iterator Graph::findConnection(std::string_view source, std::string_view target) {
    auto& conns = connections_->at(source.data());
    auto want = Connection(target.data());
    return std::find(conns.begin(), conns.end(), want);
}

bool Graph::areConnected(std::string_view source, std::string_view target) {
    return findConnection(source, target) != connections_->at(source.data()).end();
}

std::optional<int> Graph::getWeight(std::string_view source, std::string_view target) {
    return findConnection(source, target)->weight;
}