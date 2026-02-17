#include <iostream>

#include "expression-nodes.h"

namespace scc
{
    void IntegerConstantNode::print(size_t depth) const {
        std::string spaces(depth * 2, ' ');

        std::cerr << spaces << "IntegerConstant(" << m_integer << ")\n";
    }

    void IntegerConstantNode::expr() const {
        std::cerr << "override funciona en herencia de ExpressionNode a IntegerConstantNode\n";
    }
}
