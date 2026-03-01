#pragma once

#include <vector>

#include "assembly-gen/ds/function-def.h"

namespace scc::asm_gen
{
    class Program {
    private:
        std::vector<FunctionDefinition> m_functionDefs;

    public:
        Program() = default;

        explicit Program(std::vector<FunctionDefinition> functionDefs)
            : m_functionDefs(std::move(functionDefs)) {}

        ~Program() = default;

        Program(const Program&) = delete;
        Program& operator=(const Program&) = delete;

        Program(Program&&) = default;
        Program& operator=(Program&&) = default;

        [[nodiscard]] const std::vector<FunctionDefinition>& getFunctionDefs() const noexcept { return m_functionDefs; }
        [[nodiscard]] std::vector<FunctionDefinition> takeFunctionDefs() { return std::move(m_functionDefs); }

        void print() const;
    };
} // namespace scc::asm_gen
