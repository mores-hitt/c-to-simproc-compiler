#include <vector>
#include <string_view>
#include <iostream>
#include <cctype>

#include "lexer/lexer.h"
#include "lexer/token.h"

namespace scc::lexer {
    
    char Lexer::peek() const {
        if (isAtEnd()) return '\0';
        return m_sourceCode[m_pos];
    }

    char Lexer::advance() {
        ++m_columnNumber;
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
        m_lineNumber++;
        m_columnNumber = 0;
    }

    void Lexer::handleKeywordOrId(){
        m_tokenStart = m_pos;
        while (!isAtEnd() && isWordChar(peek())) {
            advance();
        }
        
        std::string_view tokenValue {getTokenView()};
        m_tokenVector.emplace_back(scc::lexer::makeKeywordOrIdentifierToken(tokenValue, m_lineNumber, m_columnNumber - 1));
    }

    void Lexer::handleIntegerConstant() {
        m_tokenStart = m_pos;
        while(!isAtEnd() && isConstant(peek())) {
            advance();
        }
        
        if (!isAtEnd() && !isDelimiter(peek()) && peek() != ' ' && peek() != '\n') {
            m_diagnostics.warning("Invalid character in integer constant", {m_lineNumber, m_columnNumber});
            return;
        }

        std::string_view tokenValue {getTokenView()};
        Token token {TokenType::integer_constant, tokenValue, m_lineNumber, m_columnNumber - 1};
        m_tokenVector.emplace_back(token);
    }

    void Lexer::handleDelimiter() {
        m_tokenStart = m_pos;
        Token token {makeDelimiterToken(getTokenView(), m_lineNumber, m_columnNumber)};
        
        if (token.type == TokenType::undefined) {
            std::cerr << "something";
        }
        
        m_tokenVector.emplace_back(token);
    }

    std::vector<Token> Lexer::analyze() {        
        while (!isAtEnd()) {
            
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
            } else {
                m_diagnostics.error("Character is not tokenizeable", {m_lineNumber, m_columnNumber});
            }
        }
        return m_tokenVector;
    }
}