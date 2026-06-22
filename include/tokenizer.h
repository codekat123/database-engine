#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include "token.h"

class Tokenizer {
public:
    explicit Tokenizer(const std::string& input);

    std::vector<Token> tokenize();

private:
    std::string input_;
    size_t pos_;

    static const std::unordered_set<std::string> keywords_;
};
