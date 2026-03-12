#include <iostream>
#include <string>

#include "code-emission/linux/emitter.h"
#include "assembly-gen/ast/nodes.h"
#include "assembly-gen/ast/instruction-nodes.h"
#include "assembly-gen/ast/operand-nodes.h"

namespace scc::code_em
{
    void Emitter::visit(const scc::asm_gen::Program& node) {
        node.getFunctionDefinition().accept(*this);
        //print .section .note.GNU-stack,"",@progbits
        m_os << "\t.section .note.GNU-stack,\"\",@progbits\n";
    }

    void Emitter::visit(const scc::asm_gen::FunctionDefinition& node){
        //print \t.globl + node.getName() + \n
        //print node.getName() + :\n
        m_os << "\t.globl " << node.getName() << "\n\n"
             << node.getName() << ":\n";

        for(auto& instruction : node.getInstructions()) {
            //print \t
            m_os << '\t';
            instruction->accept(*this);
            //print \n
            m_os << '\n';
        }

        m_os << '\n';
    }

    void Emitter::visit(const scc::asm_gen::Mov& node){
        //print movl\t
        m_os << "movl\t";
        node.getSource().accept(*this);
        //print ", "
        m_os << ", ";
        node.getDestination().accept(*this);
    }

    void Emitter::visit([[maybe_unused]] const scc::asm_gen::Ret& node){
        //print ret
        m_os << "ret";
    }
        
    void Emitter::visit(const scc::asm_gen::Immediate& node){
        //print $ + node.getValue()
        m_os << '$' << node.getValue();
    }

    void Emitter::visit([[maybe_unused]] const scc::asm_gen::Register& node){
        //print %eax
        m_os << "%eax";
    }
} // namespace scc::code_em
