#pragma once

#include <variant>
#include <iostream>

namespace scc::asm_gen
{
    template<class... Ts>
    struct overloads : Ts... { using Ts::operator()...; };

    template<class... Ts>
    overloads(Ts...) -> overloads<Ts...>;

    struct Imm {
        int value;
    };

    enum class Reg { // empty for now
    };

    using Operand = std::variant<Imm, Reg>;

    inline std::ostream& operator<<(std::ostream& os, const Operand& operand) {
        const auto visitor = overloads 
        {
            [&os](Imm imm) { os << "Immediate: " << imm.value;},
            [&os](Reg) { os << "Register: EAX"; }
        };

        std::visit(visitor, operand);

        return os;
    }
} // namespace scc::asm_gen
