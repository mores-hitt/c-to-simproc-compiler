#pragma once

#include <string_view>
#include <string>
#include <memory>


namespace scc
{
    class ASTNode {
    protected:
        std::string_view m_codeRef {};
        int m_lineNumber {1};
        int m_columnNumber {1};

    public:

        explicit ASTNode(std::string_view codeRef, int lineNumber, int columnNumber)
            : m_codeRef{codeRef}
            , m_lineNumber{lineNumber}
            , m_columnNumber{columnNumber} {}


        virtual ~ASTNode() = default;

        ASTNode(const ASTNode&) = delete;
        ASTNode& operator=(const ASTNode&) = delete;

        ASTNode(ASTNode&&) = default;
        ASTNode& operator=(ASTNode&&) = default;

        virtual void print(int depth = 0) const = 0;

        /*
        parserVisit()?
        codeGenVisit()?
        generateAssembly()?
        checkSyntax()?
        */

        std::string_view getCodeRef() const { return m_codeRef; }
        int getLineNumber() const { return m_lineNumber; }
        int getColumnNumber() const { return m_columnNumber; }
    };

    class ExpressionNode : public ASTNode {
    public:

        explicit ExpressionNode(std::string_view codeRef, int lineNumber, int columnNumber)
            : ASTNode(codeRef, lineNumber, columnNumber) {}

        virtual void expr() const = 0;
    };
    
    class StatementNode : public ASTNode {
    public:
        explicit StatementNode(std::string_view codeRef, int lineNumber, int columnNumber)
            : ASTNode(codeRef, lineNumber, columnNumber) {}
        
        virtual void stmnt() const = 0;
    };

    class FunctionDefinitionNode : public ASTNode {
    protected:
        std::string_view m_name {};
        std::unique_ptr<StatementNode> m_body;

    public:
        FunctionDefinitionNode(std::string_view codeRef, int lineNumber, int columnNumber, std::string_view name, std::unique_ptr<StatementNode> body)
            : ASTNode(codeRef, lineNumber, columnNumber)
            , m_name(name)
            , m_body(std::move(body)) {}

        void print(int depth = 0) const override;

        void funcDef() const;
    };

    class ProgramNode : public ASTNode {
    protected:
        std::unique_ptr<FunctionDefinitionNode> m_functionDefinition;

    public:
        ProgramNode(std::string_view codeRef, int lineNumber, int columnNumber, std::unique_ptr<FunctionDefinitionNode> functionDefinition)
            : ASTNode(codeRef, lineNumber, columnNumber)
            , m_functionDefinition(std::move(functionDefinition)) {}
        
        void print(int depth = 0) const override;

        void prgrm() const;
    };
} // namespace scc