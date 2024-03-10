#pragma once

// standard
#include <unordered_map>
#include <string_view>
#include <algorithm>
#include <stdexcept>
#include <optional>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <any>

namespace common {

    namespace opt {
        // Directional graph
        inline constexpr std::uint8_t drc = 0x01;
        // Weighted graph
        inline constexpr std::uint8_t wgh = 0x02;
    } // namespace opt

    // Lexemes types
    enum class LexemeType : std::uint8_t {
        GRAPH_START_LABEL = 0,
        OPEN_CURLY_BRACKET = 1,
        NODE_ID = 2,
        POINTED_ARROW = 3,
        FLAT_ARROW = 4,
        OPEN_SQUARE_BRACKET = 5,
        LABEL_ATTRIBUTE = 6,
        WEIGHT_ATTRIBUTE = 7,
        EQUALS_SIGN = 8,
        ATTRIBUTE_STRING_VALUE = 9,
        ATTRIBUTE_INT_VALUE = 10,
        CLOSED_CURLY_BRACKET = 11,
        CLOSED_SQUIRED_BRACKET = 12
    };

    struct Lexeme {
        LexemeType type;
        std::any value;
    };

    struct Connection {
        // std::nullopt for non-weighted graphs
        std::optional<int> weight;
        // Peer
        std::string peer;

        inline bool operator==(const Connection& other) const {
            return other.peer == peer;
        }
    };

    class Graph {
    public:
        using container_t = std::unordered_map<std::string, std::vector<Connection>>;

        inline Graph(std::unique_ptr<container_t> base = nullptr, std::uint8_t flags = 0x0) 
        : flags_(flags) 
        {
            if (base) {
                // need to validate this first...
                connections_ = std::move(base);    
            } else {
                connections_ = std::make_unique<container_t>();
            }
        }
        inline void setFlags(std::uint8_t flags) { flags_ = flags; }
        inline bool isDirectional() { return flags_ & opt::drc; }
        inline bool isWeighted() { return flags_ & opt::wgh; }

        inline void pushNode(std::string name) {
            connections_->insert({name, {}});
        }

        inline void pushEdge(std::string source, Connection edge) {
            if (isWeighted()) {
                edge.weight = (edge.weight == std::nullopt) ? 0 : edge.weight;
            } else {
                edge.weight = std::nullopt;
            }
            insert(source, edge);
            if (!isDirectional()) {
                std::swap(source, edge.peer);
                insert(source, edge);
            }
        }

        inline bool areConnected(std::string_view source, std::string_view target) {
            return findConnection(source, target) != connections_->at(source.data()).end();
        }

        inline std::optional<int> getWeight(std::string_view source, std::string_view target) {
            return findConnection(source, target)->weight;
        }

    private:
        inline void insert(std::string_view source, const Connection& edge) {
            connections_->at(source.data()).push_back(edge);
        }

        inline std::vector<Connection>::iterator findConnection(std::string_view source, std::string_view target) {
            auto iter = std::find_if(
                connections_->at(source.data()).begin(),
                connections_->at(source.data()).end(),
                [&target] (const auto& connection) { return connection.peer == target; }
            );
            return iter;
        }

    private:
        std::uint8_t flags_;
        std::unique_ptr<container_t> connections_;
    };

} // namespace common