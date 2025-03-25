// STD
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <memory>
#include <ostream>
#include <string>
#include <algorithm>

// local
#include <common/common.hpp>
#include <common/reverted.hpp>
#include <parser/parser.hpp>
#include <lexer/lexer.hpp>
#include <algorithms/traversal.hpp>

std::string serializeFileIntoString(const std::string& filename) {
    std::string result;
    result.resize(std::filesystem::file_size(filename));

    if (std::ifstream ifs {filename, std::ios::in | std::ios::binary}; ifs.is_open()) {
        ifs.read(result.data(), std::filesystem::file_size(filename));
    } else {
        std::cout << "Error opening file " << filename << "!";
        std::flush(std::cout);
        std::abort();
    }

    return std::move(result);
}

std::shared_ptr<common::Graph> loadGraph() {
    std::string filename;
    std::cout << "Enter filename: ";
    std::cin >> filename;

    if (!std::filesystem::exists(filename)) {
        std::cout << "File " << filename << " not found!";
        std::flush(std::cout);
        std::abort();
    }

    std::cout << "Lexing file...\n";
    auto inString = serializeFileIntoString(filename);
    auto lexemes = lexer::lex(inString);
    
    std::cout << "Parsing file...\n";
    auto graph = parser::parse(lexemes);

    std::cout << "Parse was successful!\n";
    std::cout << "Graph is loaded into memory.\n";

    return std::move(graph);
}

void dumpCurrentGraphState(std::weak_ptr<common::Graph> graph) {
    if (auto locked = graph.lock()) {
        std::cout << "Currently loaded graph:\n";
        std::cout << locked->dumpGraphState() << '\n';
        return;
    }
    std::cout << "There is no currently loaded graph in state.\n";
}
void showCurrentTraversalOrder(std::weak_ptr<common::Graph> graph) {
    auto locked = graph.lock();
    if (!locked) {
        std::cout << "There is no currently loaded graph in state.\n";
        return;
    }
    auto traversal_graph = std::dynamic_pointer_cast<common::TraversalGraph>(locked);
    if (traversal_graph) {
        traversal_graph->showTraversalOrder();
    } else {
        std::cout << "Error: Graph does not support traversal operations!\n";
    }
}

void findGivenNode(std::weak_ptr<common::Graph> graph) {
    auto locked = graph.lock();
    if (!locked) {
        std::cout << "There is no currently loaded graph in state.\n";
        return;
    }
    
    auto traversal_graph = std::dynamic_pointer_cast<common::TraversalGraph>(locked);
    if (!traversal_graph) {
        std::cout << "Error: Graph does not support search operations!\n";
        return;
    }

    std::string desired_node;
    std::cout << "Type a node to be found: \n";
    std::cin >> desired_node;

    const auto& nodes = traversal_graph->getNodes();
    if (std::find(nodes.begin(), nodes.end(), desired_node) == nodes.end()) {
        std::cout << "Node not found.\n";
    } else {
        std::cout << "Searching node...\n";
        traversal_graph->findNode(desired_node);
        std::cout << "Search done successfully.\n";
    }
}

void dumpGraphToFile(std::weak_ptr<common::Graph> graph, common::GraphDumpingFactory& factory) {
    if (auto locked = graph.lock()) {
        std::string filename;
        std::cout << "Enter filename: ";
        std::cin >> filename;

        std::cout << "Dumping graph to file: " << filename << '\n';
        factory.dumpOne(*locked, filename);
        std::cout << "Dump was successful.\n";
        return;
    }
    std::cout << "There is no currently loaded graph in state.\n";
}

int main() {

    std::cout << "This program parses .gv files into"
              << "program-accessible struсture.\n";

    int option = 0;

    common::GraphDumpingFactory factory {{ .verboseWrite = false }};
    std::shared_ptr<common::Graph> state = nullptr;

    while (true) {
        std::cout << "Select action:\n" 
                  << "- load graph (1)\n"
                  << "- dump graph into a file (2)\n"
                  << "- dump currently loaded graph (3)\n"
                  << "- show graph traversal order (4)\n"
                  << "- find node with depth-first search(5)\n"
                  << "- exit (6)\n";
        std::cin >> option;

        switch (option) {
        case 1:
            state = loadGraph();
            break;
        case 2:
            dumpGraphToFile(state, factory);
            break;
        case 3:
            dumpCurrentGraphState(state);
            break;
        case 4:
            showCurrentTraversalOrder(state);
            break;
        case 5:
            findGivenNode(state);
            break;
        case 6:
            return 0;
        default:
            std::cout << "Unknown option, aborting...";
            return 0;
        }

    }

    return 0;
}
