// STD
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>

// graph
#include <common/common.hpp>
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

int main() {

    std::cout << "This program parses .gv files into"
              << "program-accessible struture.\n";

    std::string filename;
    std::cout << "Enter filename: ";
    std::cin >> filename;

    if (!std::filesystem::exists(filename)) {
        std::cout << "File " << filename << " not found!";
        std::flush(std::cout);
        return 1;
    }

    std::cout << "Lexing file...\n";
    auto inString = serializeFileIntoString(filename);
    auto lexemes = lexer::lex(inString);
    
    std::cout << "Parsing file...\n";
    auto graph = parser::parse(lexemes);

    std::cout << "Parse was successful! Result:\n";
    graph->dumpGraphState();

    return 0;
}
