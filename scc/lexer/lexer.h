#pragma once

#include "lexer/token.h"
#include <string_view>
#include <vector>

namespace scc::lexer {

    class Lexer {

        private:
        std::string_view m_sourceCode;
        size_t m_pos {};
        size_t m_tokenStart {};

        int lineNumber {1};
        int columnNumber {1};
        std::vector<Token> tokenVector;

        std::string_view getTokenView();

        [[nodiscard]] bool isWordStart(const char c) const;
        [[nodiscard]] bool isWordChar(const char c) const;
        [[nodiscard]] bool isConstant(const char c) const ;
        [[nodiscard]] bool isDelimiter(const char c) const;
        [[nodiscard]] bool isWordBoundary(const char* c) const;

        void handleLine();
        void handleKeywordOrId();
        void handleIntegerConstant();
        void handleDelimiter();
        
        [[nodiscard]] char peek() const;
        char advance();
        [[nodiscard]] bool isAtEnd() const;

        public:
        Lexer(std::string_view sourceCode)
            : m_sourceCode(sourceCode)
            , tokenVector() { tokenVector.reserve(sourceCode.size() / 4); }

        Lexer(const Lexer&) = delete;
        Lexer& operator=(const Lexer&) = delete;

        Lexer(Lexer&&) = default;
        Lexer& operator=(Lexer&&) = default;

        [[nodiscard]] std::vector<Token> analyze();
        
    };
}