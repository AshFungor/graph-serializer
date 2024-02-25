#pragma once

// standard
#include <cstdint>
#include <any>

namespace common {

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

}