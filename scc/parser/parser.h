#pragma once

#include <vector>
#include <memory>

#include "parser/ast/ast.h"
#include "lexer/token.h"

namespace scc
{
    /*
    What I need:
    Parser class:
        std::vector<Tokens> tokens; (moved)
        AST ast; (actual fucking tree though)

    */
    class Parser {
    private:
        std::vector<Token> m_tokens;
        std::unique_ptr<AST> m_ast;
        size_t m_pos {0};

        Token popFront();
        Token peekFront();

        void expect(TokenType expectedType);
        void expect(TokenType expectedType, const Token& token);
        void expect(TokenType expectedType, std::string expectedValue, const Token& token);
        void expect(TokenType expectedType, int expectedValue, const Token& token);
        std::unique_ptr<ExpressionNode> parseExpression(size_t pos);
        std::unique_ptr<StatementNode> parseStatement(size_t pos);
        std::unique_ptr<FunctionDefinitionNode> parseFunction(size_t pos);
        std::unique_ptr<ProgramNode> parseProgram(size_t pos);

    
    public:

        Parser(std::vector<Token> tokens)
            : m_tokens(std::move(tokens))
            , m_ast(nullptr){
                parse();
            }

        ~Parser() = default;

        Parser(const Parser&) = delete;
        Parser& operator=(const Parser&) = delete;

        Parser(Parser&&) = default;
        Parser& operator=(Parser&&) = default;

        void parse();
        void print();

    };
} // namespace scc
