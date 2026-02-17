#pragma once

#include <string_view>
#include <string>
#include <memory>


namespace scc
{
    class ASTNode {
    protected:
        int m_lineNumber {1};
        int m_columnNumber {1};

    public:

        explicit ASTNode(int lineNumber, int columnNumber)
            : m_lineNumber{lineNumber}
            , m_columnNumber{columnNumber} {}


        virtual ~ASTNode() = default;

        ASTNode(const ASTNode&) = delete;
        ASTNode& operator=(const ASTNode&) = delete;

        ASTNode(ASTNode&&) = default;
        ASTNode& operator=(ASTNode&&) = default;

        virtual void print(size_t depth = 0) const = 0;

        /*
        parserVisit()?
        codeGenVisit()?
        generateAssembly()?
        checkSyntax()?
        */

        [[nodiscard]] int getLineNumber() const noexcept { return m_lineNumber; }
        [[nodiscard]] int getColumnNumber() const noexcept { return m_columnNumber; }
    };

    class ExpressionNode : public ASTNode {
    public:

        explicit ExpressionNode(int lineNumber, int columnNumber)
            : ASTNode(lineNumber, columnNumber) {}

        virtual void expr() const = 0;
    };
    
    class StatementNode : public ASTNode {
    public:
        explicit StatementNode(int lineNumber, int columnNumber)
            : ASTNode(lineNumber, columnNumber) {}
        
        virtual void stmnt() const = 0;
    };

    class FunctionDefinitionNode : public ASTNode {
    protected:
        std::string_view m_name {};
        std::unique_ptr<StatementNode> m_body;

    public:
        FunctionDefinitionNode(int lineNumber, int columnNumber, std::string_view name, std::unique_ptr<StatementNode> body)
            : ASTNode(lineNumber, columnNumber)
            , m_name(name)
            , m_body(std::move(body)) {}

        void print(size_t depth = 0) const override;

        void funcDef() const;
    };

    class ProgramNode : public ASTNode {
    protected:
        std::unique_ptr<FunctionDefinitionNode> m_functionDefinition;

    public:
        ProgramNode(int lineNumber, int columnNumber, std::unique_ptr<FunctionDefinitionNode> functionDefinition)
            : ASTNode(lineNumber, columnNumber)
            , m_functionDefinition(std::move(functionDefinition)) {}
        
        void print(size_t depth = 0) const override;

        void prgrm() const;
    };
} // namespace scc