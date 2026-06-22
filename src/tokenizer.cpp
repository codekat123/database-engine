#include "../include/tokenizer.h"

const std::unordered_set<std::string> Tokenizer::keywords_ = {
    "SELECT", "FROM", "WHERE", "INSERT", "INTO", "VALUES"
};

Tokenizer::Tokenizer(const std::string& input)
    : input_(input), pos_(0) {}



std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens;

    while (pos_ < input_.size()) {
        char c = input_[pos_];

        if (std::isspace(static_cast<unsigned char>(c))) {
            pos_++;
            continue;
        }

        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
            size_t start = pos_;
            while (pos_ < input_.size() &&
                   (std::isalnum(static_cast<unsigned char>(input_[pos_])) || input_[pos_] == '_')) {
                pos_++;
            }
            std::string word = input_.substr(start, pos_ - start);
            std::string upper = word;
            for (char& ch : upper) ch = std::toupper(static_cast<unsigned char>(ch));
            TokenType type = keywords_.count(upper) ? TokenType::KEYWORD : TokenType::IDENTIFIER;
            tokens.push_back({type, word});
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(c))) {
            size_t start = pos_;
            while (pos_ < input_.size() &&
                   std::isdigit(static_cast<unsigned char>(input_[pos_]))) {
                pos_++;
            }
            tokens.push_back({TokenType::NUMBER, input_.substr(start, pos_ - start)});
            continue;
        }

        if (c == '\'') {
            pos_++;
            size_t start = pos_;
            while (pos_ < input_.size() && input_[pos_] != '\'') {
                pos_++;
            }
            std::string value = input_.substr(start, pos_ - start);
            if (pos_ < input_.size()) pos_++;
            tokens.push_back({TokenType::STRING_LITERAL, value});
            continue;
        }

        if (c == '=' || c == '>' || c == '<') {
            tokens.push_back({TokenType::OPERATOR, std::string(1, c)});
            pos_++;
            continue;
        }

        if (c == '(' || c == ')' || c == ',' || c == ';') {
            tokens.push_back({TokenType::PUNCTUATION, std::string(1, c)});
            pos_++;
            continue;
        }


        pos_++;
    }

    tokens.push_back({TokenType::END_OF_INPUT, ""});
    return tokens;
}
