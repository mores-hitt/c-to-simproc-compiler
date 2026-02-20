#pragma once

#include "lexer/token.h"
#include <string_view>
#include <string>
#include <vector>

namespace scc::lexer {

    class Lexer {

        private:
        const char* charPointer;
        const char* sourceCodeEnd;
        const char* tokenStart {nullptr};
        const char* tokenEnd {nullptr};
        int lineNumber {1};
        int columnNumber {0};
        std::vector<Token> tokenVector;

        std::string_view getTokenView(const char* tokenStart, const char* tokenEnd);

        [[nodiscard]] bool isWordStart(const char c) const;
        [[nodiscard]] bool isWordChar(const char c) const;
        [[nodiscard]] bool isConstant(const char c) const ;
        [[nodiscard]] bool isDelimiter(const char c) const;
        [[nodiscard]] bool isWordBoundary(const char* c) const;

        void handleLine();
        void handleKeywordOrId();
        void handleIntegerConstant();
        void handleWhiteSpace();
        void handleDelimiter();

        public:
        Lexer(std::string_view sourceCode):
        charPointer(sourceCode.data()),
        sourceCodeEnd(sourceCode.data() + sourceCode.size()),
        tokenVector() {
            tokenVector.reserve(sourceCode.size() / 4);
        }

        Lexer(const Lexer&) = delete;
        Lexer& operator=(const Lexer&) = delete;

        [[nodiscard]] std::vector<Token> analize();
        
    };

}