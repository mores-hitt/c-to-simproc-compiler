#include <vector>
#include <string_view>
#include <iostream>
#include <cctype>
#include <stdexcept>

#include "lexer/lexer.h"
#include "lexer/token.h"

namespace scc::lexer {
    
    char Lexer::peek() const {
        if (isAtEnd()) return '\0';
        return m_sourceCode[m_pos];
    }

    char Lexer::advance() {
        ++columnNumber;
        return m_sourceCode[m_pos++];
    }

    bool Lexer::isAtEnd() const {
        return m_pos >= m_sourceCode.size();
    }
    
    std::string_view Lexer::getTokenView(){
        return m_sourceCode.substr(m_tokenStart, m_pos - m_tokenStart);
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
        m_tokenStart = m_pos;
        while (!isAtEnd() && isWordChar(peek())) {
            advance();
        }
        std::string_view tokenValue {getTokenView()};
        tokenVector.emplace_back(scc::lexer::makeKeywordToken(tokenValue, lineNumber, columnNumber));
    }

    void Lexer::handleIntegerConstant() {
        m_tokenStart = m_pos;
        while(!isAtEnd() && isConstant(peek())) {
            advance();
        }
        
        if (!isAtEnd() && !isDelimiter(peek()) && peek() != ' ' && peek() != '\n') {
            std::cerr << "Broken integer constant at line:" << lineNumber
                      << "  column:"<< columnNumber << ".\n";
            throw std::runtime_error("\nInvalid integer constant\n");
        }

        std::string_view tokenValue {getTokenView()};
        Token token {TokenType::integer_constant, tokenValue, lineNumber, columnNumber};
        tokenVector.emplace_back(token);
    }

    void Lexer::handleWhiteSpace() {
        std::cerr << "here, a whitespace " << peek() << "\n";
    }

    void Lexer::handleDelimiter() {
        std::cerr << "here, a delimiter: " << peek() << "\n";
        m_tokenStart = m_pos;
        tokenVector.push_back(scc::lexer::makeDelimiterToken(getTokenView(), lineNumber, columnNumber));
    }

    std::vector<Token> Lexer::analyze() {        
        while (!isAtEnd()) {
            ++columnNumber;
            
            if (peek() == '\n') {
                handleLine();
                advance();
                continue;
                
            } else if (isWordStart(peek())) {
                handleKeywordOrId();
                // handleKeywordOrId leaves m_pos one character forward
                continue;
                
            } else if (std::isdigit(peek())) {
                handleIntegerConstant();
                // handleIntegerConstant leaves m_pos one character forward
                continue;
                
            } else if (std::isspace(peek())) {
                advance();
                continue;
                
            } else if (isDelimiter(peek())) {
                handleDelimiter();
                advance();
                continue;
                
            }
        }
        return tokenVector;
    }
}