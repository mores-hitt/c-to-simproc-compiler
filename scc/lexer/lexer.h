#pragma once

#include "lexer/token.h"
#include <string_view>
#include <string>
#include <vector>

namespace scc{

    class Lexer {

        private:
        const char* charPointer;
        const char* sourceCodeEnd;
        const char* tokenStart {nullptr};
        const char* tokenEnd {nullptr};
        int lineNumber {1};
        int columnNumber {1};
        std::vector<Token> tokenVector;

        std::string_view getTokenView(const char* tokenStart, const char* tokenEnd);

        void handleLine();
        void handleKeyWord();
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