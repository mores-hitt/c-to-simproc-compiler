#include <iostream>

#include "ast.h"

namespace scc
{
    void AST::print() const {
        m_root->print();
    }
} // namespace scc
