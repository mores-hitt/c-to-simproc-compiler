#include <iostream>

#include "ast.h"

namespace scc::parser
{
    void AST::print() const {
        m_root->print();
    }
} // namespace scc
