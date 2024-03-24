#pragma once

// standard
#include <unordered_map>
#include <string_view>
#include <optional>
#include <fstream>
#include <utility>
#include <ostream>
#include <memory>
#include <string>
#include <vector>
#include <any>

// local
#include <common/common.hpp>


namespace common {

    class GraphDumpingFactory {
    public:
        struct Settings {
            bool verboseWrite = false;
        };
        // constructors
        /**
         * @brief Construct a new graph dumping factory
         * @param settings settings for this factory
         */
        GraphDumpingFactory(Settings settings) noexcept;
        GraphDumpingFactory(GraphDumpingFactory&&) noexcept = default;
        GraphDumpingFactory(const GraphDumpingFactory&) = delete;

        /**
         * @brief Dumps single graph to a file
         * @param one target graph object
         * @param filename file to dump to
         */
        void dumpOne(const Graph& one, std::string_view filename);

    private:
        void dumpGraphMetadata(const Graph& unit);
        void dumpGraphNodes(const Graph& unit);
        void dumpGraphEdges(const Graph& unit);
        void trail(const Graph& unit);

        void error(const std::string& message);

    private:
        Settings settings_;
        std::ofstream ofs_;
    };

}