#include <iostream>
#include <charconv>
#include <memory>
#include <stdexcept>

#include "parser/parser.h"
#include "lexer/token.h"

namespace scc::parser
{

    namespace {
        void printUnexpectedTokenError(scc::lexer::Token token, scc::lexer::TokenType expectedType) {
            std::cerr << "Syntax error (line " << token.lineNumber
                      << ", column " << token.columnNumber
                      << "): Expected " << expectedType
                      << ", but found " << token.type << ".\n";
        }

        void printUnexpectedValueError(scc::lexer::Token token, std::string_view expectedValue) {
            std::cerr << "Syntax error (line " << token.lineNumber
                      << ", column " << token.columnNumber
                      << "): Expected the following value: " << expectedValue
                      << ", but found:" << token.value << ".\n";
        }
    }

    void Parser::expect(scc::lexer::TokenType expectedType) {
        scc::lexer::Token token {m_tokens.at(m_pos++)};
        if (expectedType != token.type) {
            printUnexpectedTokenError(token, scc::lexer::TokenType::integer_constant);
            throw std::runtime_error("Syntax error");
        } else {
            return;
        }
    }

    void Parser::expect(scc::lexer::TokenType expectedType, const scc::lexer::Token& token) {
        if (expectedType != token.type) {
            printUnexpectedTokenError(token, scc::lexer::TokenType::integer_constant);
            throw std::runtime_error("Syntax error");
        } else {
            return;
        }
    }

    void Parser::expect(scc::lexer::TokenType expectedType, std::string expectedValue, const scc::lexer::Token& token) {
        if (expectedType != token.type)  {
            printUnexpectedTokenError(token, scc::lexer::TokenType::integer_constant);
            throw std::runtime_error("Syntax error");
        } else if (expectedValue != token.value) {
            printUnexpectedValueError(token, expectedValue);
            throw std::runtime_error("Syntax error");
        } else {
            return;
        }
    }

    std::unique_ptr<ExpressionNode> Parser::parseExpression(size_t pos) {
        scc::lexer::Token token {m_tokens.at(pos)};

        expect(scc::lexer::TokenType::integer_constant);

        int integerValue {0};
        std::from_chars(token.value.data(), token.value.data() + token.value.size(), integerValue);

        return std::make_unique<IntegerConstantNode>(token.lineNumber, token.columnNumber, integerValue);
    }

    std::unique_ptr<StatementNode> Parser::parseStatement(size_t pos) {
        scc::lexer::Token token {m_tokens.at(pos)};

        expect(scc::lexer::TokenType::return_keyword);

        std::unique_ptr<ExpressionNode> expression {parseExpression(m_pos)};

        expect(scc::lexer::TokenType::semicolon);

        return std::make_unique<ReturnNode>(token.lineNumber, token.columnNumber, std::move(expression));
    }

    std::unique_ptr<FunctionDefinitionNode> Parser::parseFunction(size_t pos) {
        scc::lexer::Token token = m_tokens.at(pos);

        expect(scc::lexer::TokenType::int_keyword);
        expect(scc::lexer::TokenType::identifier);
        std::string_view name = m_tokens.at(m_pos-1).value;
        expect(scc::lexer::TokenType::open_parenthesis);
        expect(scc::lexer::TokenType::void_keyword);
        expect(scc::lexer::TokenType::close_parenthesis);
        expect(scc::lexer::TokenType::open_brace);

        std::unique_ptr<StatementNode> statement {parseStatement(m_pos)};

        expect(scc::lexer::TokenType::close_brace);

        return std::make_unique<FunctionDefinitionNode>(token.lineNumber, token.columnNumber, name, std::move(statement));
    }

    std::unique_ptr<ProgramNode> Parser::parseProgram(size_t pos) {
        scc::lexer::Token token = m_tokens.at(pos);
        
        std::unique_ptr<FunctionDefinitionNode> functionDefinition {parseFunction(m_pos)};

        return std::make_unique<ProgramNode>(token.lineNumber, token.columnNumber, std::move(functionDefinition));
    }

    void Parser::parse() {
        m_ast = std::make_unique<AST>(parseProgram(m_pos));
        if (m_pos < m_tokens.size()) {
            printUnexpectedTokenError(m_tokens.at(m_pos), scc::lexer::TokenType::undefined);
            throw std::runtime_error("Parsing Error");
        }
    }
    
    void Parser::print() {
        m_ast->print();
    }
} // namespace scc
