#pragma once

#include <iostream>

#include "assembly-gen/ds/terminal.h"
#include "assembly-gen/ds/instruction-visitor.h"

namespace scc::asm_gen
{
    class Instruction {
    public:
        Instruction() = default;

        virtual ~Instruction() = default;

        Instruction(const Instruction&) = delete;
        Instruction& operator=(const Instruction&) = delete;

        Instruction(Instruction&&) = default;
        Instruction& operator=(Instruction&&) = default;

        virtual void accept(InstructionVisitor& v) const = 0;
    };

    class Mov : public Instruction {
    private:
        Operand m_src;
        Operand m_dest;
    public:
        explicit Mov(Operand src, Operand dest)
            : m_src(src)
            , m_dest(dest) {} 
        
        [[nodiscard]] const Operand& getSrc() const { return m_src; }
        [[nodiscard]] const Operand& getDest() const { return m_dest; }

        void accept(InstructionVisitor& v) const override { v.visit(*this); }

    };

    class Ret : public Instruction {
    public:
        Ret() = default;

        void accept(InstructionVisitor& v) const override { v.visit(*this); }

    };

} // namespace scc::asm_gen
