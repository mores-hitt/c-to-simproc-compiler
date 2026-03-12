#pragma once

#include <filesystem>
#include <iostream>

#include "assembly-gen/ast/nodes.h"
#include "assembly-gen/ast/visitor.h"

namespace scc::code_em
{
    class Emitter : public scc::asm_gen::Visitor {
    protected:
        std::ostream& m_os;
        
    public:
        explicit Emitter(std::ostream& os )
            : m_os(os) {}
        
        ~Emitter() override = default;

        Emitter(const Emitter&) = delete;
        Emitter& operator=(const Emitter&) = delete;

        void visit(const scc::asm_gen::Program& node) override;
        void visit(const scc::asm_gen::FunctionDefinition& node) override;

        void visit(const scc::asm_gen::Mov& node) override;
        void visit(const scc::asm_gen::Ret& node) override;
        
        void visit(const scc::asm_gen::Immediate& node) override;
        void visit(const scc::asm_gen::Register& node) override;

    };
} // namespace scc::code_em
