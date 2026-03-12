#pragma once

#include <cstddef>

#include "assembly-gen/ast/visitor.h"

namespace scc::asm_gen
{
    class ASTPrinter : public Visitor {
    protected:
        size_t m_depth {0};
    
    public:
        
        ASTPrinter() = default;
        ~ASTPrinter() override = default;

        ASTPrinter(const ASTPrinter&) = delete;
        ASTPrinter& operator=(const ASTPrinter&) = delete;


        void visit(const Program& node) override;
        void visit(const FunctionDefinition& node) override;

        void visit(const Mov& node) override;
        void visit(const Ret& node) override;
        
        void visit(const Immediate& node) override;
        void visit(const Register& node) override;
    };
} // namespace scc::asm_gen
