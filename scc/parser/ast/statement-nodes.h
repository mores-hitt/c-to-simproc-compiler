#pragma once

#include "nodes.h"

namespace scc::parser
{
    class ReturnNode : public StatementNode {
    protected:
        std::unique_ptr<ExpressionNode> m_expression;

    public:
        
        explicit ReturnNode(int lineNumber, int columnNumber, std::unique_ptr<ExpressionNode> expression)
            : StatementNode(lineNumber, columnNumber)
            , m_expression(std::move(expression)) {}

        void accept(Visitor& v) const override { v.visit(*this); }

        void stmnt() const override;

        [[nodiscard]] const ExpressionNode& getExpression() const noexcept { return *m_expression;}
        
    };
} // namespace scc
