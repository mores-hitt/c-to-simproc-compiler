#include <iostream>

#include "assembly-gen/visitors/ast-printer.h"
#include "assembly-gen/ast/nodes.h"
#include "assembly-gen/ast/instruction-nodes.h"
#include "assembly-gen/ast/operand-nodes.h"

namespace scc::asm_gen
{
    void ASTPrinter::visit(const Program& node) {
        std::cerr << "Program(\n";
        ++m_depth;
        node.getFunctionDefinition().accept(*this);
        --m_depth;
        std::cerr << ")\n";
    }

    void ASTPrinter::visit(const FunctionDefinition& node){
        std::string spaces(m_depth * 2, ' ');

        std::cerr << spaces << "Function(\n"
                  << spaces << spaces << "name=\"" << node.getName() << "\",\n"
                  << spaces << spaces << "instructions=\n";
        ++m_depth;
        for(auto& instruction : node.getInstructions()) {
            instruction->accept(*this);
        }
        --m_depth;
        std::cerr << spaces << ")\n";
    }

    void ASTPrinter::visit(const Mov& node){
        std::string spaces(m_depth * 3, ' ');
        std::cerr << spaces << "Mov" << '\n'
                  << spaces << "  Source: ";
        node.getSource().accept(*this);
        std::cerr << spaces << "  Destination: ";
        node.getDestination().accept(*this);
    }

    void ASTPrinter::visit([[maybe_unused]]const Ret& node){
        std::string spaces(m_depth * 3, ' ');
        std::cerr << spaces << "Ret" << '\n';
    }
        
    void ASTPrinter::visit(const Immediate& node){
        std::cerr << "Immediate: " << node.getValue() << '\n';
    }

    void ASTPrinter::visit(const Register& node){
        std::string spaces(m_depth * 2, ' ');
        std::cerr << "Register: " << node.getReg() << '\n';
    }
} // namespace scc::asm_gen
