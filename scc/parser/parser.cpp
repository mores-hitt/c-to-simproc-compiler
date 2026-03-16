#include <charconv>
#include <memory>

#include "parser/parser.h"
#include "lexer/token.h"

namespace scc::parser
{
    void Parser::expect(scc::lexer::TokenType expectedType) {
        scc::lexer::Token token {m_tokens.at(m_pos++)};
        if (expectedType != token.type) {
            m_diagnostics.error("Unexpected token", {token.lineNumber, token.columnNumber});
            jumpToSemicolon();
        } else {
            return;
        }
    }
    
    void Parser::jumpToSemicolon() {
        lexer::Token token {m_tokens.at(m_pos)};
        
        while (token.type != lexer::TokenType::semicolon) {
            token = m_tokens.at(m_pos++);
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
            m_diagnostics.error("Unexpected Token", {m_tokens.at(m_pos).lineNumber, m_tokens.at(m_pos).columnNumber});
        }
    }
    
} // namespace scc
