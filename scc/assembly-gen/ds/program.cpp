#include <iostream>

#include "assembly-gen/ds/program.h"
#include "assembly-gen/visitors/instruction-printer.h"

namespace scc::asm_gen
{
    void Program::print() const {
        InstructionPrinter printer {};
        for(auto& function : m_functionDefs) {
            std::cout << "function name: " << function.getName() << '\n';
            for(auto& instruction : function.getInstructions()) {
                instruction->accept(printer);
            }
        }        
    }
} // namespace scc::asm_gen
