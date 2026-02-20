#pragma once

#include "nodes.h"

namespace scc::parser
{
    class IntegerConstantNode : public ExpressionNode {
    protected:
        int m_integer {0};

    public:
        explicit IntegerConstantNode(int lineNumber, int columnNumber, int integer)
            : ExpressionNode(lineNumber, columnNumber)
            , m_integer(integer) {}
        
        void print(size_t depth = 0) const override;

        void expr() const override;
    };

} // namespace scc