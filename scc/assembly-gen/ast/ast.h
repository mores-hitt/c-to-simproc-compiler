#pragma once

#include <memory>

#include "assembly-gen/ast/nodes.h"
#include "assembly-gen/ast/visitor.h"

namespace scc::asm_gen
{
    class AST {
    private:
        std::unique_ptr<ASTNode> m_root;
    
    public:

        explicit AST(std::unique_ptr<ASTNode> root)
            : m_root(std::move(root)) {}

        ~AST() = default;

        AST(const AST&) = delete;
        AST& operator=(const AST&) = delete;

        AST(AST&&) = default;
        AST& operator=(AST&&) = default;

        void accept(Visitor& visitor) const { m_root->accept(visitor); }

    };
} // namespace scc::asm_gen
