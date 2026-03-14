#pragma once

#include <iostream>
#include <cstdint>

#include "assembly-gen/ast/nodes.h"

namespace scc::asm_gen
{
    class Immediate : public Operand {
    protected:
        uint16_t m_value;
    
    public:
        explicit Immediate(int value)
            : m_value(static_cast<uint16_t>(value)) {}
        
        [[nodiscard]] int getValue() const noexcept { return m_value; }

        void accept(Visitor& v) const override { v.visit(*this); }
    };

    class Register : public Operand {
    public:
        enum class Regs : uint8_t {
            AX,
            BX,
            CX,
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
            case Register::Regs::AX: return os << "AX";
            case Register::Regs::BX: return os << "BX";
            case Register::Regs::CX: return os << "CX";
        }
        return os;
    }
} // namespace scc::asm_gen