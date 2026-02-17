#pragma once

#include <memory>

#include "ast-nodes.h"
#include "expression-nodes.h"
#include "statement-nodes.h"

namespace scc
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

        void print() const;

        /*
        traverse()?
        somethingelses()?
        */

    };
} // namespace scc
