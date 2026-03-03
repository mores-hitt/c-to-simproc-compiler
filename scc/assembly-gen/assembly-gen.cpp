#include <vector>
#include <string>
#include <string_view>

#include "assembly-gen/assembly-gen2.h"
#include "assembly-gen/ast/nodes.h"
#include "assembly-gen/ast/instruction-nodes.h"
#include "assembly-gen/ast/operand-nodes.h"

#include "parser/ast/nodes.h"
#include "parser/ast/expression-nodes.h"
#include "parser/ast/statement-nodes.h"

namespace scc::asm_gen
{
    void AssemblyGen::visit(const scc::parser::ProgramNode& node) {
        node.getFunction().accept(*this);
        m_program = std::make_unique<Program>(std::move(m_functionDef));
        m_ast = std::make_unique<AST>(std::move(m_program));
    }

    void AssemblyGen::visit(const scc::parser::FunctionDefinitionNode& node) {
        node.getBody().accept(*this);
        m_functionDef = std::make_unique<FunctionDefinition>(std::string(node.getName()), std::move(m_instructions));
    }

    void AssemblyGen::visit(const scc::parser::ReturnNode& node) {
        node.getExpression().accept(*this);
        m_instructions.push_back(std::make_unique<Mov>(std::move(m_currentOperand), std::make_unique<Register>(Register::Regs::EAX)));
        m_instructions.push_back(std::make_unique<Ret>());
    }

    void AssemblyGen::visit(const scc::parser::IntegerConstantNode& node) {
        m_currentOperand = std::make_unique<Immediate>(node.getInteger());
    }
} // namespace scc::asm_gen
