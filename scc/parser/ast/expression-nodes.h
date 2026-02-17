#pragma once

#include "ast-nodes.h"

namespace scc
{
    class IntegerConstantNode : public ExpressionNode {
    protected:
        int m_integer {0};

    public:
        explicit IntegerConstantNode(std::string_view codeRef, int lineNumber, int columnNumber, int integer)
            : ExpressionNode(codeRef, lineNumber, columnNumber)
            , m_integer(integer) {}
        
        void print(int depth = 0) const override;

        void expr() const override;
    };

} // namespace scc