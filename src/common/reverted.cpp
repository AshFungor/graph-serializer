// standard
#include <string_view>
#include <filesystem>
#include <stdexcept>
#include <optional>
#include <fstream>
#include <utility>
#include <ostream>
#include <memory>
#include <string>
#include <set>
#include <ios>
#include <iostream>
// common
#include <common/common.hpp>

// local
#include "reverted.hpp"

using namespace common;


GraphDumpingFactory::GraphDumpingFactory(Settings settings) noexcept 
    : settings_(settings)
{}

void GraphDumpingFactory::dumpOne(const Graph& one, std::string_view filename) {
    if (std::filesystem::exists(filename)) {
        error("Cannot write to existing file: " + std::string(filename.data()));
    }

    ofs_.open(filename.data(), std::ios::out | std::ios::binary);

    dumpGraphMetadata(one);
    dumpGraphNodes(one);
    dumpGraphEdges(one);
    trail(one);

    ofs_.close();
}

void GraphDumpingFactory::dumpGraphMetadata(const Graph& unit) {
    if (settings_.verboseWrite) {
        ofs_ << "# Written by GraphWriter.\n"
             << "# This software is part of our university project.\n";
        ofs_ << '\n';
    }

    ofs_ << ((unit.isDirectional()) ? "digraph" : "graph") << " {\n";
}

void GraphDumpingFactory::dumpGraphNodes(const Graph& unit) {
    ofs_ << '\n';
    if (settings_.verboseWrite) {
        ofs_ << "  # Graph nodes.\n";
    }
    
    ofs_ << '\n';
    for (const auto& pair : *unit.connections_) {
        ofs_ << "  " << pair.first << ' ';
        if (auto label = unit.getLabel(pair.first); label.has_value()) {
            ofs_ << "[label = \"" << label.value() << "\"]";
        }
        ofs_ << '\n';
    }
    ofs_ << '\n';
}

void GraphDumpingFactory::dumpGraphEdges(const Graph& unit) {
    ofs_ << '\n';
    if (settings_.verboseWrite) {
        ofs_ << "  # Graph edges.\n";
    }

    ofs_ << '\n';
    std::unordered_map<std::string, std::set<std::string>> met;
    for (const auto& pair : *unit.connections_) {
        for (const auto& connection : pair.second) {
            if (!unit.isDirectional() && met[pair.first].contains(connection.peer)) continue;

            // Формируем строку с атрибутами ребра
            std::vector<std::string> attrs;

            // Добавляем метку, если она есть
            if (connection.label.has_value()) {
                attrs.push_back("label=\"" + connection.label.value() + "\"");
            }

            // Добавляем вес, если граф взвешенный и вес задан
            if (unit.isWeighted() && connection.weight.has_value()) {
                attrs.push_back("weight=" + std::to_string(*connection.weight));
            }

            // Формируем вывод атрибутов
            std::string attr_str;
            if (!attrs.empty()) {
                attr_str = " [";
                for (size_t i = 0; i < attrs.size(); ++i) {
                    if (i > 0) attr_str += ", ";
                    attr_str += attrs[i];
                }
                attr_str += "]";
            }

            // Записываем ребро с атрибутами
            ofs_ << "  " << pair.first 
                 << ((unit.isDirectional()) ? " -> " : " -- ") 
                 << connection.peer 
                 << attr_str 
                 << '\n';

            met[pair.first].insert(connection.peer);
            if (!unit.isDirectional()) {
                met[connection.peer].insert(pair.first);
            }
        }
    }
    ofs_ << '\n';
}

void GraphDumpingFactory::trail(const Graph& unit) {
    ofs_ << "}\n";
    if (!ofs_) error("Stream error, failbit is set to 1");
}

void GraphDumpingFactory::error(const std::string& message) {
    throw std::runtime_error(message);
}