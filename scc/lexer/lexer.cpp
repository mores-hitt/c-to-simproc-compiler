#include "lexer/lexer.h"
#include "lexer/token.h"

#include <vector>
#include <string_view>
#include <iostream>
#include <cctype>
#include <algorithm>

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
        std::cout << "here, a new line: \\n " << "\n";
        lineNumber++;
        columnNumber = 1;
    }

    void Lexer::handleKeywordOrId(){
        
        std::cout << "start of token \n";
        tokenStart = charPointer;
        while (charPointer != sourceCodeEnd && isWordChar(*charPointer)) { // keep looping until break
            charPointer++;
            columnNumber++;
        }
        std::cout << "end of token: ";
        tokenEnd = charPointer;

        auto tokenValue {getTokenView(tokenStart, tokenEnd)};

        std::cout << tokenValue << "\n";

        tokenVector.push_back(scc::isKeyword(tokenValue, lineNumber));

    }

    void Lexer::handleIntegerConstant() {
        std::cout << "start of integer constant?\n";
        tokenStart = charPointer;
        while ( charPointer != sourceCodeEnd && isConstant(*charPointer)) { // keep looping until no more digits
            charPointer++;
            columnNumber++;
        }
        
        if (charPointer != sourceCodeEnd && !isDelimiter(*charPointer)) {
            std::cerr << "Broken integer constant at line:" << lineNumber
                      << "  column:"<< columnNumber << ".\n";
            throw std::runtime_error("\nInvalid integer constant\n");
        }

        std::cout << "end of integer literal: ";
        tokenEnd = charPointer;

        auto tokenValue {getTokenView(tokenStart, tokenEnd)};

        std::cout << tokenValue << "\n";

        Token token {TokenType::integer_constant, tokenValue, lineNumber};

        tokenVector.push_back(token);
    }

    void Lexer::handleWhiteSpace() {
        std::cout << "here, a whitespace " << *charPointer << "\n";
        columnNumber++;
    }

    void Lexer::handleDelimiter() {
        std::cout << "here, a delimiter?: " << *charPointer << "\n";
        tokenVector.push_back(scc::isDelimiter(charPointer, lineNumber));
        columnNumber++;
    }

    std::vector<Token> Lexer::analize() {
        while (charPointer != sourceCodeEnd) {
            std::cout << "line number: " << lineNumber << ". ";
            std::cout << "character number: " << columnNumber << ". ";

            if (*charPointer == '\n') {
                handleLine();
                charPointer++;
                continue;
            }
            else if (std::isalpha(*charPointer)) {
                handleKeyWord();
                // handleKeyword leaves charPointer one character forward
                continue;
            }
            else if (std::isdigit(*charPointer)) {
                handleIntegerConstant();
                // handleKeyword leaves charPointer one character forward
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
                std::cerr << "\n\nCarácter no identificable en linea " << lineNumber << " columna " << columnNumber << ".\n"; 
                throw std::runtime_error("\nUnindentifiable character\n");
            }
        }

        return tokenVector;
    }
}