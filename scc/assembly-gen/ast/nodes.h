#pragma once

#include <memory>
#include <string>
#include <vector>

#include "assembly-gen/ast/visitor.h"

namespace scc::asm_gen
{
    class ASTNode {
    public:

        ASTNode() = default;
        virtual ~ASTNode() = default;

        ASTNode(const ASTNode&) = delete;
        ASTNode& operator=(const ASTNode&) = delete;

        ASTNode(ASTNode&&) = default;
        ASTNode& operator=(ASTNode&&) = default;

        virtual void accept(Visitor& v) const = 0;
    };

    class Operand : public ASTNode {
    public:
        Operand() = default;

    };

    class Instruction : public ASTNode {
    public:
        Instruction() = default;
    };

    class FunctionDefinition : public ASTNode {
    protected:
        std::string m_name;
        std::vector<std::unique_ptr<Instruction>> m_instructions;

    public:
        explicit FunctionDefinition(std::string name, std::vector<std::unique_ptr<Instruction>> instructions)
            : m_name(name)
            , m_instructions(std::move(instructions)) {}

        void accept(Visitor& v) const override { v.visit(*this); }

        [[nodiscard]] const std::string_view getName() const noexcept { return m_name; }
        [[nodiscard]] const std::vector<std::unique_ptr<Instruction>>& getInstructions() const noexcept { return m_instructions; }
    };

    class Program : public ASTNode {
    protected:
        std::unique_ptr<FunctionDefinition>  m_functionDefinition;
    
    public:
        explicit Program(std::unique_ptr<FunctionDefinition> functionDefinition)
            : m_functionDefinition(std::move(functionDefinition)) {}

        void accept(Visitor& v) const override { v.visit(*this); }

        [[nodiscard]] const FunctionDefinition& getFunctionDefinition() const noexcept {return *m_functionDefinition; }

    };

} // namespace scc::asm_gen
