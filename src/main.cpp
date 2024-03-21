// STD
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <memory>
#include <ostream>
#include <string>

// graph
#include <common/common.hpp>
#include <common/reverted.hpp>
#include <parser/parser.hpp>
#include <lexer/lexer.hpp>

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
              << "program-accessible struture.\n";

    int option = 0;

    common::GraphDumpingFactory factory {{ .verboseWrite = false }};
    std::shared_ptr<common::Graph> state = nullptr;

    while (true) {
        std::cout << "Select action:\n" 
                  << "- load graph (1)\n"
                  << "- dump graph into a file (2)\n"
                  << "- dump currently loaded graph (3)\n"
                  << "- exit (4)\n";
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
            return 0;
        default:
            std::cout << "Unknown option, aborting...";
            return 0;
        }

    }

    return 0;
}
