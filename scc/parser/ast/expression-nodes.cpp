#include <iostream>

#include "expression-nodes.h"

namespace scc::parser
{
    void IntegerConstantNode::expr() const {
        std::cerr << "override funciona en herencia de ExpressionNode a IntegerConstantNode\n";
    }
}
