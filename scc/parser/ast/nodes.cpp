#include <string_view>
#include <string>
#include <iostream>

#include "parser/ast/nodes.h"

namespace scc::parser
{

    void FunctionDefinitionNode::funcDef() const {
        std::cerr << "Funciona funcDef supongo lol";
    }
    
    void ProgramNode::prgrm() const {
        std::cerr << "Functiona prgrm supongo lol";
    }
} // namespace scc