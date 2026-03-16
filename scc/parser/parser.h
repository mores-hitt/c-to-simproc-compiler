#pragma once

#include <vector>
#include <memory>

#include "parser/ast/ast.h"
#include "lexer/token.h"
#include "errors/diagnostic.h"

namespace scc::parser
{
    class Parser {
    private:
        std::vector<scc::lexer::Token> m_tokens;
        std::unique_ptr<AST> m_ast;
        Diagnostics& m_diagnostics;
        
        size_t m_pos {0};

        scc::lexer::Token popFront();
        scc::lexer::Token peekFront();
        
        void jumpToSemicolon();

        void expect(scc::lexer::TokenType expectedType);
        void expect(scc::lexer::TokenType expectedType, const scc::lexer::Token& token);
        void expect(scc::lexer::TokenType expectedType, std::string expectedValue, const scc::lexer::Token& token);
        void expect(scc::lexer::TokenType expectedType, int expectedValue, const scc::lexer::Token& token);
        std::unique_ptr<ExpressionNode> parseExpression(size_t pos);
        std::unique_ptr<StatementNode> parseStatement(size_t pos);
        std::unique_ptr<FunctionDefinitionNode> parseFunction(size_t pos);
        std::unique_ptr<ProgramNode> parseProgram(size_t pos);

    
    public:

        Parser(std::vector<scc::lexer::Token> tokens, Diagnostics& diagnostics)
            : m_tokens(std::move(tokens))
            , m_ast(nullptr)
            , m_diagnostics(diagnostics){
                parse();
            }

        ~Parser() = default;

        Parser(const Parser&) = delete;
        Parser& operator=(const Parser&) = delete;

        Parser(Parser&&) = default;
        // No move assignment because Parser takes diagnostic as a reference

        void parse();

        void accept(Visitor& visitor) const { m_ast->accept(visitor); }

    };
} // namespace scc
