#pragma once

#include <iostream>

#include "assembly-gen/ds/instruction-visitor.h"
#include "assembly-gen/ds/instructions.h"

namespace scc::asm_gen
{
    class InstructionPrinter : public InstructionVisitor {
        public:
        
        InstructionPrinter() = default;
        ~InstructionPrinter() override = default;

        InstructionPrinter(const InstructionPrinter&) = delete;
        InstructionPrinter& operator=(const InstructionPrinter&) = delete;

        void visit(const Mov& inst) override;
        void visit([[maybe_unused]] const Ret& inst) override;
    };
} // namespace scc::asm_gen
