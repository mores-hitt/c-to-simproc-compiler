#pragma once

#include <string_view>
#include <array>
#include <unordered_map>
#include <vector>
#include <optional>

namespace scc {

    enum class TokenType {
        open_brace,
        close_brace,
        open_parenthesis,
        close_parenthesis,
        semicolon,
        int_keyword,
        return_keyword,
        main_keyword,
        void_keyword,
        identifier,
        string_literal,
        integral_literal,
        undefined
    };

    inline const std::unordered_map<std::string_view, TokenType> keywordMap = {
        {"int", TokenType::int_keyword},
        {"return", TokenType::return_keyword},
        {"main", TokenType::main_keyword},
        {"void", TokenType::void_keyword},
    };

    inline const std::unordered_map<char, TokenType> delimiterMap = {
        {'{', TokenType::open_brace},
        {'}', TokenType::close_brace},
        {'(', TokenType::open_parenthesis},
        {')', TokenType::close_parenthesis},
        {';', TokenType::semicolon},
    };

    struct Token {
        TokenType type {};
        std::string_view value {};
        int lineNumber {};
    };

    scc::Token isDelimiter(const char* c, int lineNumber);
    bool isDelimiter(char c);
    scc::Token isKeyword(std::string_view word, int lineNumber);

}