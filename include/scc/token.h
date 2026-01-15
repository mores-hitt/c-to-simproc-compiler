#pragma once

#include <string_view>
#include <array>
#include <unordered_map>
#include <vector>
#include <optional>
#include <iostream>

namespace scc {

    enum class TokenType {
        open_brace,
        close_brace,
        open_parenthesis,
        close_parenthesis,
        semicolon,
        int_keyword,
        return_keyword,
        void_keyword,
        identifier,
        string_literal,
        integral_literal,
        undefined
    };

    inline constexpr std::string_view getTokenType(TokenType type) {
        switch (type)
        {
        case TokenType::open_brace: return "open_brace";
        case TokenType::close_brace: return "close_brace";
        case TokenType::open_parenthesis: return "open_parenthesis";
        case TokenType::close_parenthesis: return "close_parenthesis";
        case TokenType::semicolon: return "semicolon";
        case TokenType::int_keyword: return "int_keyword";
        case TokenType::return_keyword: return "return_keyword";
        case TokenType::void_keyword: return "void_keyword";
        case TokenType::identifier: return "identifier";
        case TokenType::string_literal: return "string_literal";
        case TokenType::integral_literal: return "integral_literal";
        case TokenType::undefined: return "undefined";
        default: return "unknown";
        }
    }

    inline std::ostream& operator<<(std::ostream& out, TokenType type){
        return out << getTokenType(type);
    }

    inline const std::unordered_map<std::string_view, TokenType> keywordMap = {
        {"int", TokenType::int_keyword},
        {"return", TokenType::return_keyword},
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