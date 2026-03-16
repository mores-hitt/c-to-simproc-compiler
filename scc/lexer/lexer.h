#pragma once

#include "lexer/token.h"
#include "errors/diagnostic.h"

#include <string_view>
#include <vector>

namespace scc::lexer {

    class Lexer {

        private:
        std::string_view m_sourceCode;
        size_t m_pos {};
        size_t m_tokenStart {};
        int m_lineNumber {1};
        int m_columnNumber {1};
        std::vector<Token> m_tokenVector;
        Diagnostics& m_diagnostics;
        

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
        Lexer(std::string_view sourceCode, Diagnostics& diagnostics)
            : m_sourceCode(sourceCode)
            , m_tokenVector()
            , m_diagnostics(diagnostics) { m_tokenVector.reserve(sourceCode.size() / 4); }

        Lexer(const Lexer&) = delete;
        Lexer& operator=(const Lexer&) = delete;

        Lexer(Lexer&&) = default;
        // No move assignment because Lexer takes diagnostic as a reference

        [[nodiscard]] std::vector<Token> analyze();
        
    };
}