#include <iostream>

#include "expression-nodes.h"

namespace scc
{
    void IntegerConstantNode::print(int depth) const {
        std::string spaces {(depth * 2, ' ')};

        std::cout << spaces << "IntegerConstant(" << m_integer << ")";
    }

    void IntegerConstantNode::expr() const {
        std::cout << "override funciona en herencia de ExpressionNode a IntegerConstantNode\n";
    }
}
