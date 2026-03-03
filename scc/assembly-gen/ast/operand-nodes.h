#pragma once

#include <iostream>

#include "assembly-gen/ast/nodes.h"

namespace scc::asm_gen
{
    class Immediate : public Operand {
    protected:
        int m_value;
    
    public:
        explicit Immediate(int value)
            : m_value(value) {}
        
        [[nodiscard]] int getValue() const noexcept { return m_value; }

        void accept(Visitor& v) const override { v.visit(*this); }
    };

    class Register : public Operand {
    public:
        enum class Regs {
            EAX,
            EBX,
            ECX,
        };

        explicit Register(Regs reg)
            : m_reg(reg) {}

        [[nodiscard]] Regs getReg() const noexcept { return m_reg; }

        void accept(Visitor& v) const override { v.visit(*this); }
        
    protected:
        Regs m_reg;
    };

    inline std::ostream& operator<<(std::ostream& os, Register::Regs reg) {
        switch (reg) {
            case Register::Regs::EAX: return os << "EAX";
            case Register::Regs::EBX: return os << "EBX";
            case Register::Regs::ECX: return os << "ECX";
        }
        return os;
    }
} // namespace scc::asm_gen