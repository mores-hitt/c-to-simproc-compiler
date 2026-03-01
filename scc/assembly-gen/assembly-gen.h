#pragma once

#include <vector>
#include <memory>

#include "assembly-gen/ds/program.h"
#include "assembly-gen/ds/terminal.h"
#include "parser/ast/visitor.h"

namespace scc::asm_gen
{
    class AssemblyGen : public scc::parser::Visitor {
    private:
        Program m_program;
        std::vector<FunctionDefinition> m_functions;
        std::vector<std::unique_ptr<Instruction>> m_instructions;
        Operand m_currentOperand;

    public: 
        explicit AssemblyGen()
            : m_program(std::vector<FunctionDefinition>())
            , m_functions(std::vector<FunctionDefinition>())
            , m_instructions(std::vector<std::unique_ptr<Instruction>>())
            , m_currentOperand(Imm{0}) {}

        ~AssemblyGen() = default;

        AssemblyGen(const AssemblyGen&) = delete;
        AssemblyGen& operator=(const AssemblyGen&) = delete;

        AssemblyGen(AssemblyGen&&) = default;
        AssemblyGen& operator=(AssemblyGen&&) = default;

        void visit(const scc::parser::ProgramNode& node) override;
        void visit(const scc::parser::FunctionDefinitionNode& node) override;
        void visit(const scc::parser::ReturnNode& node) override;
        void visit(const scc::parser::IntegerConstantNode& node) override;

        [[nodiscard]] const Program& getProgram() const noexcept { return m_program; }
        [[nodiscard]] Program takeProgram() { return std::move(m_program); }
    };
} // namespace scc::asm_gen
