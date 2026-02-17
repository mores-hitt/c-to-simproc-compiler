#include <iostream>
#include <charconv>
#include <memory>
#include <stdexcept>

#include "parser/parser.h"
#include "lexer/token.h"

namespace scc
{

    namespace {
        void printUnexpectedTokenError(Token token, TokenType expectedType) {
            std::cerr << "Syntax error (line " << token.lineNumber
                      << ", column " << token.columnNumber
                      << "): Expected " << expectedType
                      << ", but found " << token.type << ".\n";
        }

        void printUnexpectedValueError(Token token, std::string_view expectedValue) {
            std::cerr << "Syntax error (line " << token.lineNumber
                      << ", column " << token.columnNumber
                      << "): Expected the following value: " << expectedValue
                      << ", but found:" << token.value << ".\n";
        }
    }

    void Parser::expect(TokenType expectedType) {
        Token token {m_tokens.at(m_pos++)};
        if (expectedType != token.type) {
            printUnexpectedTokenError(token, TokenType::integer_constant);
            throw std::runtime_error("Syntax error");
        } else {
            return;
        }
    }

    void Parser::expect(TokenType expectedType, const Token& token) {
        if (expectedType != token.type) {
            printUnexpectedTokenError(token, TokenType::integer_constant);
            throw std::runtime_error("Syntax error");
        } else {
            return;
        }
    }

    void Parser::expect(TokenType expectedType, std::string expectedValue, const Token& token) {
        if (expectedType != token.type)  {
            printUnexpectedTokenError(token, TokenType::integer_constant);
            throw std::runtime_error("Syntax error");
        } else if (expectedValue != token.value) {
            printUnexpectedValueError(token, expectedValue);
            throw std::runtime_error("Syntax error");
        } else {
            return;
        }
    }

    std::unique_ptr<ExpressionNode> Parser::parseExpression(size_t pos) {
        Token token {m_tokens.at(pos)};

        expect(TokenType::integer_constant);

        int integerValue {0};
        std::from_chars(token.value.cbegin(), token.value.cend(), integerValue);

        return std::make_unique<IntegerConstantNode>(token.lineNumber, token.columnNumber, integerValue);
    }

    std::unique_ptr<StatementNode> Parser::parseStatement(size_t pos) {
        Token token {m_tokens.at(pos)};

        expect(TokenType::return_keyword);

        std::unique_ptr<ExpressionNode> expression {parseExpression(m_pos)};

        expect(TokenType::semicolon);

        return std::make_unique<ReturnNode>(token.lineNumber, token.columnNumber, std::move(expression));
    }

    std::unique_ptr<FunctionDefinitionNode> Parser::parseFunction(size_t pos) {
        Token token = m_tokens.at(pos);

        expect(TokenType::int_keyword);
        expect(TokenType::identifier);
        std::string_view name = m_tokens.at(m_pos-1).value;
        expect(TokenType::open_parenthesis);
        expect(TokenType::void_keyword);
        expect(TokenType::close_parenthesis);
        expect(TokenType::open_brace);

        std::unique_ptr<StatementNode> statement {parseStatement(m_pos)};

        expect(TokenType::close_brace);

        return std::make_unique<FunctionDefinitionNode>(token.lineNumber, token.columnNumber, name, std::move(statement));
    }

    std::unique_ptr<ProgramNode> Parser::parseProgram(size_t pos) {
        Token token = m_tokens.at(pos);
        
        std::unique_ptr<FunctionDefinitionNode> functionDefinition {parseFunction(m_pos)};

        return std::make_unique<ProgramNode>(token.lineNumber, token.columnNumber, std::move(functionDefinition));
    }

    void Parser::parse() {
        m_ast = std::make_unique<AST>(parseProgram(m_pos));
    }
    
    void Parser::print() {
        m_ast->print();
        if (m_pos < m_tokens.size()) {
            printUnexpectedTokenError(m_tokens.at(m_pos), TokenType::undefined);
            throw std::runtime_error("Parsing Error");
        }
    }
} // namespace scc
