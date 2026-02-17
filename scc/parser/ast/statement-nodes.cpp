#include <iostream>

#include "statement-nodes.h"

namespace scc
{
    void ReturnNode::print(size_t depth) const {
        std::string spaces(depth * 2, ' ');
    
        std::cerr << "Return(\n";
        m_expression->print(depth+1);
        std::cerr << spaces << ")\n";
    }

    void ReturnNode::stmnt() const {
        std::cerr << "override funciona en herencia de StatementNode a ReturnNode\n";
    }
} // namespace scc
