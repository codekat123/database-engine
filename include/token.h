#pragma once

#include <string>

enum class TokenType {
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    STRING_LITERAL,
    OPERATOR,
    PUNCTUATION,
    END_OF_INPUT,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string text;
};
