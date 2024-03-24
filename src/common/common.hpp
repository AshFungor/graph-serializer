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

    /**
     * @brief Options for graph
     * This enumeration represents options that
     * graph supports and that alter its API
     * calls behaviour.
     */
    namespace opt {
        inline constexpr std::uint8_t drc = 0x01; ///< is graph directional?
        inline constexpr std::uint8_t wgh = 0x02; ///< is graph weighted?
    } // namespace opt

    /**
     * @brief Supported lexemes
     * Lexemes that are accepted by parser.
     */
    enum class LexemeType : std::uint8_t {
        GRAPH_START_LABEL = 0,      ///< graph begging keyword
        OPEN_CURLY_BRACKET = 1,     ///< graph body start symbol
        NODE_ID = 2,                ///< string node ID
        POINTED_ARROW = 3,          ///< edge symbol, drc set
        FLAT_ARROW = 4,             ///< edge symbol, no drc
        OPEN_SQUARE_BRACKET = 5,    ///< attributes start symbol
        LABEL_ATTRIBUTE = 6,        ///< label attribute keyword
        EQUALS_SIGN = 7,            ///< attribute value symbol
        ATTRIBUTE_STRING_VALUE = 8, ///< string attribute value
        ATTRIBUTE_INT_VALUE = 9,    ///< integer attribute value
        CLOSED_CURLY_BRACKET = 10,  ///< graph body end symbol
        CLOSED_SQUARE_BRACKET = 11  ///< attributes end symbol
    };

    /**
     * @brief Lexeme
     * Represents single unit of lexer output.
     */
    struct Lexeme {
        LexemeType type;
        std::any value;
    };

    /**
     * @brief Edge
     * Represents edge without source node.
     */
    struct Connection {
        // std::nullopt for non-weighted graphs
        std::optional<int> weight;
        // Peer
        std::string peer;

        Connection(std::string peer, std::optional<int> weight = std::nullopt) noexcept;
        bool operator==(const Connection& other) const;
    };

    /**
     * @brief Graph
     * Represents graph object as lists of connections. Supports
     * some API calls to make life easier.
     */
    class Graph {
    public:

        using graph_flags_t = std::uint8_t;
        using connections_t = std::vector<Connection>;
        using label_container_t = std::unordered_map<std::string, std::string>;
        using container_t = std::unordered_map<std::string, connections_t>;
        using container_value_t = std::pair<std::string, connections_t>;

        /**
         * @brief Construct a new Graph object
         * Constructs object and puts it into non-initialized state. 
         */
        Graph() noexcept;

        /**
         * @brief Inits graph object
         * Sets flags and puts object into active mode.
         * @param flags flags to set
         */
        void init(graph_flags_t flags = 0x0) noexcept;

        /**
         * @brief Checks for drc flag
         * @return true if graph is directional
         * @return false otherwise
         */
        bool isDirectional() const noexcept;

        /**
         * @brief Checks for wgh flag
         * @return true if graph is weighted
         * @return false otherwise
         */
        bool isWeighted() const noexcept;

        /**
         * @brief Adds new node to graph
         * @param name node ID 
         */
        void pushNode(std::string name);

        /**
         * @brief Adds new edge to graph
         * @param source source node ID
         * @param edge edge to add
         */
        void pushEdge(std::string source, Connection edge);

        /**
         * @brief Sets label for node
         * @param source node ID
         * @param label label string
         */
        void setLabel(std::string source, std::string label);

        /**
         * @brief Checks for connection between nodes
         * @param source first node ID
         * @param target second node ID
         * @return true if connection exists
         * @return false otherwise
         */
        bool areConnected(std::string_view source, std::string_view target);

        /**
         * @brief Gets weight of edge
         * @param source first node ID
         * @param target second node ID
         * @return std::optional<int> weight if exists
         */
        std::optional<int> getWeight(std::string_view source, std::string_view target);

        /**
         * @brief Gets label of node
         * @param source node ID
         * @return std::optional<std::string> label if exists
         */
        std::optional<std::string> getLabel(std::string source) const;

        /**
         * @brief Dump graph state to a string
         * @return std::string graph state
         */
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