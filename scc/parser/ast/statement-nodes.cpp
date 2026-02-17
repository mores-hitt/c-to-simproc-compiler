#include <iostream>

#include "statement-nodes.h"

namespace scc
{
    void ReturnNode::print(int depth) const {
        std::string spaces {(depth * 2, ' ')};
    
        std::cout << spaces << "Return(\n";
        m_expression->print(depth+1);
        std::cout << "\n" << spaces << ")";
    }

    void ReturnNode::stmnt() const {
        std::cout << "override funciona en herencia de StatementNode a ReturnNode\n";
    }
} // namespace scc
