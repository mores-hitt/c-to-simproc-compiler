#include <iostream>

#include "statement-nodes.h"

namespace scc::parser
{
    void ReturnNode::stmnt() const {
        std::cerr << "override funciona en herencia de StatementNode a ReturnNode\n";
    }
} // namespace scc
