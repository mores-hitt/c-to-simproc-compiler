#pragma once

#include <vector>
#include <memory>

#include "assembly-gen/ast/nodes.h"
#include "assembly-gen/ast/operand-nodes.h"
#include "assembly-gen/ast/ast.h"
#include "assembly-gen/ast/visitor.h"
#include "parser/ast/visitor.h"

namespace scc::asm_gen
{
    class AssemblyGen : public scc::parser::Visitor {
    private:
        std::unique_ptr<AST> m_ast;
        std::unique_ptr<Program> m_program;
        std::unique_ptr<FunctionDefinition> m_functionDef;
        std::vector<std::unique_ptr<Instruction>> m_instructions;
        std::unique_ptr<Operand> m_currentOperand;

    public: 
        explicit AssemblyGen()
            : m_ast(nullptr)
            , m_program(nullptr)
            , m_functionDef(nullptr)
            , m_instructions(std::vector<std::unique_ptr<Instruction>>())
            , m_currentOperand(nullptr) {}

        ~AssemblyGen() override = default;

        AssemblyGen(const AssemblyGen&) = delete;
        AssemblyGen& operator=(const AssemblyGen&) = delete;

        AssemblyGen(AssemblyGen&&) = default;
        AssemblyGen& operator=(AssemblyGen&&) = default;

        void visit(const scc::parser::ProgramNode& node) override;
        void visit(const scc::parser::FunctionDefinitionNode& node) override;
        void visit(const scc::parser::ReturnNode& node) override;
        void visit(const scc::parser::IntegerConstantNode& node) override;

        void accept(scc::asm_gen::Visitor& visitor) const { m_ast->accept(visitor); }
    };
} // namespace scc::asm_gen
