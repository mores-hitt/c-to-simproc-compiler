#include <vector>
#include <string>
#include <string_view>

#include "assembly-gen/assembly-gen.h"
#include "assembly-gen/ds/terminal.h"
#include "assembly-gen/ds/instructions.h"
#include "assembly-gen/ds/function-def.h"
#include "parser/ast/nodes.h"
#include "parser/ast/expression-nodes.h"
#include "parser/ast/statement-nodes.h"

namespace scc::asm_gen
{
    void AssemblyGen::visit(const scc::parser::ProgramNode& node) {
        node.getFunction().accept(*this);
        m_program = Program(std::move(m_functions));
    }

    void AssemblyGen::visit(const scc::parser::FunctionDefinitionNode& node) {
        m_instructions.clear();
        node.getBody().accept(*this);
        m_functions.push_back(std::move(FunctionDefinition(std::string(node.getName()), m_instructions)));
    }

    void AssemblyGen::visit(const scc::parser::ReturnNode& node) {
        node.getExpression().accept(*this);
        m_instructions.push_back(std::make_unique<Mov>(m_currentOperand, Reg{} ));
        m_instructions.push_back(std::make_unique<Ret>());
    }

    void AssemblyGen::visit(const scc::parser::IntegerConstantNode& node) {
        m_currentOperand = Imm{ node.getInteger() };
    }
} // namespace scc::asm_gen
