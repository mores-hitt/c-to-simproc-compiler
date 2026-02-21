#pragma once

#include "parser/ast/nodes.h"

namespace scc::parser
{
    class IntegerConstantNode : public ExpressionNode {
    protected:
        int m_integer {0};

    public:
        explicit IntegerConstantNode(int lineNumber, int columnNumber, int integer)
            : ExpressionNode(lineNumber, columnNumber)
            , m_integer(integer) {}

        void accept(Visitor& v) const override { v.visit(*this); }

        void expr() const override;

        [[nodiscard]] int getInteger() const noexcept { return m_integer; }
    };

} // namespace scc