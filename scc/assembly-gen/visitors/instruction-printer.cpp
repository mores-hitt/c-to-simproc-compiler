#include <iostream>

#include "assembly-gen/visitors/instruction-printer.h"
#include "assembly-gen/ds/instructions.h"

namespace scc::asm_gen
{
    void InstructionPrinter::visit(const Mov& inst) {
        std::cerr << "Mov instruction:\n"
                  << "Source: " << inst.getSrc() << '\n'
                  << "Destination: " << inst.getDest() << '\n';
    }

    void InstructionPrinter::visit([[maybe_unused]] const Ret& inst) {
        std::cerr << "Ret instruction\n";
    }
} // namespace scc::asm_gen
