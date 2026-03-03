#pragma once

#include <memory>

#include "assembly-gen/ast/nodes.h"

namespace scc::asm_gen
{
    class Mov : public Instruction {
    protected:
        std::unique_ptr<Operand> m_source;
        std::unique_ptr<Operand> m_destination;

    public:
        explicit Mov(std::unique_ptr<Operand> source, std::unique_ptr<Operand> destination)
            : m_source(std::move(source))
            , m_destination(std::move(destination)) {}

        void accept(Visitor& v) const override { v.visit(*this); }

        [[nodiscard]] const Operand& getSource() const noexcept { return *m_source; }
        [[nodiscard]] const Operand& getDestination() const noexcept { return *m_destination; }
    };

    class Ret : public Instruction {
    public:
        Ret() = default;

        void accept(Visitor& v) const override { v.visit(*this); }
    };
} // namespace asm_gen
