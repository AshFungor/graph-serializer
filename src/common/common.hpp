#pragma once

// standard
#include <unordered_map>
#include <string_view>
#include <optional>
#include <cstdint>
#include <utility>
#include <ostream>
#include <memory>
#include <string>
#include <vector>
#include <any>

namespace common {

    // class to invert graph parsing (write it back)
    class GraphDumpingFactory;

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
        EQUALS_SIGN = 7,
        ATTRIBUTE_STRING_VALUE = 8,
        ATTRIBUTE_INT_VALUE = 9,
        CLOSED_CURLY_BRACKET = 10,
        CLOSED_SQUARE_BRACKET = 11
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

        Connection(std::string peer, std::optional<int> weight = std::nullopt) noexcept;
        bool operator==(const Connection& other) const;
    };

    class Graph {
    public:

        using graph_flags_t = std::uint8_t;
        using connections_t = std::vector<Connection>;
        using label_container_t = std::unordered_map<std::string, std::string>;
        using container_t = std::unordered_map<std::string, connections_t>;
        using container_value_t = std::pair<std::string, connections_t>;

        Graph() noexcept;
        void init(graph_flags_t flags = 0x0) noexcept;
        // void setFlags(graph_flags_t flags) noexcept;
        bool isDirectional() const noexcept;
        bool isWeighted() const noexcept;

        void pushNode(std::string name);
        void pushEdge(std::string source, Connection edge);
        void setLabel(std::string source, std::string label);

        bool areConnected(std::string_view source, std::string_view target);
        std::optional<int> getWeight(std::string_view source, std::string_view target);
        std::optional<std::string> getLabel(std::string source) const;

        std::string dumpGraphState() const;
        friend std::ostream& operator<<(std::ostream& os, const Graph& graph);
        friend GraphDumpingFactory;

    private:
        void insert(std::string_view source, Connection edge);
        connections_t::iterator findConnection(std::string_view source, std::string_view target);

    private:
        std::uint8_t flags_;
        std::unique_ptr<container_t> connections_;
        std::unique_ptr<label_container_t> labels_;
    };

    std::ostream& operator<<(std::ostream& os, const Graph& graph);

} // namespace common