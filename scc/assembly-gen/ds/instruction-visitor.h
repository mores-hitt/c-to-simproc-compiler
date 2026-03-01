#pragma once

namespace scc::asm_gen
{
    class Mov;
    class Ret;

    class InstructionVisitor {
    public:
        virtual ~InstructionVisitor() = default;

        virtual void visit(const Mov& inst) = 0;
        virtual void visit(const Ret& inst) = 0;
    };
} // namespace asm_gen
