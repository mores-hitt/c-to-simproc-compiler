#include <string_view>
#include <string>
#include <iostream>

#include "parser/ast/nodes.h"

namespace scc::parser
{

    void FunctionDefinitionNode::print(size_t depth) const {
        std::string spaces(depth * 2, ' ');

        std::cerr << spaces << "Function(\n"
                  << spaces << "  name=\"" << m_name << "\",\n"
                  << spaces << "  body=";
        m_body->print(depth+1);
        std::cerr << spaces << ")\n";
    }

    void FunctionDefinitionNode::funcDef() const {
        std::cerr << "Funciona funcDef supongo lol";
    }

    void ProgramNode::print(size_t depth) const {
        std::cerr << "Program(\n";
        m_functionDefinition->print(depth + 1);
        std::cerr << ")\n";
    }

    
    void ProgramNode::prgrm() const {
        std::cerr << "Functiona prgrm supongo lol";
    }
} // namespace scc