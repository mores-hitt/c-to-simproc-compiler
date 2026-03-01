#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "assembly-gen/ds/instructions.h"

namespace scc::asm_gen
{
    class FunctionDefinition {
    private:
        std::string m_name;
        std::vector<std::unique_ptr<Instruction>> m_instructions;

    public:
        explicit FunctionDefinition(std::string name, std::vector<std::unique_ptr<Instruction>>& instructions)
            : m_name(std::move(name))
            , m_instructions(std::move(instructions)) {}

        ~FunctionDefinition() = default;

        FunctionDefinition(const FunctionDefinition&) = delete;
        FunctionDefinition& operator=(const FunctionDefinition&) = delete;

        FunctionDefinition(FunctionDefinition&&) = default;
        FunctionDefinition& operator=(FunctionDefinition&&) = default;

        [[nodiscard]] const std::vector<std::unique_ptr<Instruction>>& getInstructions() const noexcept { return m_instructions; }
        [[nodiscard]] const std::string_view getName() const noexcept { return m_name; } 
    };
} // namespace scc::asm_gen
