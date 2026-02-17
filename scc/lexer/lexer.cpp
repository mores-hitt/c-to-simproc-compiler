#include "lexer/lexer.h"
#include "lexer/token.h"

#include <vector>
#include <string_view>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <stdexcept>

namespace scc {

    std::string_view Lexer::getTokenView(const char* tokenStart, const char* tokenEnd){
        return std::string_view(tokenStart, static_cast<size_t>(tokenEnd - tokenStart)); // get size of token by pointer substraction
    }

    bool Lexer::isWordStart(const char c) const {
        return (c >= 'A' && c <= 'Z' ) || (c >= 'a' && c <= 'z' ) || c == '_';
    }

    bool Lexer::isWordChar(const char c) const {
        return (c >= 'A' && c <= 'Z' ) || (c >= 'a' && c <= 'z' ) || (c >= '0' && c <= '9') || c == '_';
    }

    bool Lexer::isConstant(const char c) const {
        return c >= '0' && c <= '9';
    }

    bool Lexer::isDelimiter(const char c) const {
        return delimiterMap.count(c) > 0;
    }

    void Lexer::handleLine(){
        std::cerr << "here, a new line: ... " << "\n";
        lineNumber++;
        columnNumber = 0;
    }

    void Lexer::handleKeywordOrId(){
        
        std::cerr << "start of token \n";
        tokenStart = charPointer;
        while (charPointer != sourceCodeEnd && isWordChar(*charPointer)) { // keep looping until break
            charPointer++;
            columnNumber++;
        }
        std::cerr << "end of token: ";
        tokenEnd = charPointer;

        auto tokenValue {getTokenView(tokenStart, tokenEnd)};

        std::cerr << tokenValue << "\n";

        tokenVector.push_back(scc::makeKeywordToken(tokenValue, lineNumber, columnNumber));

    }

    void Lexer::handleIntegerConstant() {
        std::cerr << "start of integer constant\n";
        tokenStart = charPointer;
        while ( charPointer != sourceCodeEnd && isConstant(*charPointer)) { // keep looping until no more digits
            charPointer++;
            columnNumber++;
        }
        
        if (charPointer != sourceCodeEnd && !isDelimiter(*charPointer) && *charPointer != ' ' && *charPointer != '\n') {
            std::cerr << "Broken integer constant at line:" << lineNumber
                      << "  column:"<< columnNumber << ".\n";
            throw std::runtime_error("\nInvalid integer constant\n");
        }

        // TODO: ver temas de arroba y caso especiales (123;bar y 123bar)

        std::cerr << "end of integer literal: ";
        tokenEnd = charPointer;

        auto tokenValue {getTokenView(tokenStart, tokenEnd)};

        std::cerr << tokenValue << "\n";

        Token token {TokenType::integer_constant, tokenValue, lineNumber, columnNumber};

        tokenVector.push_back(token);
    }

    void Lexer::handleWhiteSpace() {
        std::cerr << "here, a whitespace " << *charPointer << "\n";
    }

    void Lexer::handleDelimiter() {
        std::cerr << "here, a delimiter: " << *charPointer << "\n";
        tokenVector.push_back(scc::makeDelimiterToken(charPointer, lineNumber, columnNumber));
    }

    std::vector<Token> Lexer::analize() {
        while (charPointer != sourceCodeEnd) {
            ++columnNumber;
            std::cerr << "line number: " << lineNumber << ". ";
            std::cerr << "character number: " << columnNumber << ". ";

            if (*charPointer == '\n') {
                handleLine();
                charPointer++;
                continue;
            }
            else if ( isWordStart(*charPointer) ) {
                handleKeywordOrId();
                // handleKeywordOrId leaves charPointer one character forward
                continue;
            }
            else if (std::isdigit(*charPointer)) {
                handleIntegerConstant();
                // handleIntegerConstant leaves charPointer one character forward
                continue;
            }
            else if (std::isspace(*charPointer)) {
                handleWhiteSpace();
                charPointer++;
                continue;
            }
            else if (isDelimiter(*charPointer)) {
                handleDelimiter();
                charPointer++;
                continue;
            } else {
                std::cerr << "\n\nChár no identificable en linea " << lineNumber << " columna " << columnNumber << ".\n"; 
                throw std::runtime_error("\nUnindentifiable character\n");
            }
        }

        return tokenVector;
    }
}