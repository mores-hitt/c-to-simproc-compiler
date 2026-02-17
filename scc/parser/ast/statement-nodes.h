#pragma once

#include "ast-nodes.h"

namespace scc
{
    class ReturnNode : public StatementNode {
    protected:
        std::unique_ptr<ExpressionNode> m_expression;

    public:
        
        explicit ReturnNode(std::string_view codeRef, int lineNumber, int columnNumber, std::unique_ptr<ExpressionNode> expression)
            : StatementNode(codeRef, lineNumber, columnNumber)
            , m_expression(std::move(expression)) {}
        
        void print(int depth = 0) const override;

        void stmnt() const override;
    };
} // namespace scc
