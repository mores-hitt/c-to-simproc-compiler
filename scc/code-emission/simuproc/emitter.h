#pragma once

#include <iostream>
#include <string_view>
#include <unordered_map>
#include <string>

#include "assembly-gen/ast/nodes.h"
#include "assembly-gen/ast/visitor.h"

namespace scc::code_em
{
    class Emitter : public scc::asm_gen::Visitor {
    protected:
        std::ostream& m_os;
        int m_currentLine {0};
        std::unordered_map<std::string, int> m_functionToAddressMap;
        std::vector<int> m_constants;
        std::string m_currentFunction;
        
        [[nodiscard]] int getOrAddConstant(int value);
        
    public:
        explicit Emitter(std::ostream& os )
            : m_os(os)
            , m_functionToAddressMap({})
            , m_constants({})
            , m_currentFunction({}) {}
        
        ~Emitter() override = default;

        Emitter(const Emitter&) = delete;
        Emitter& operator=(const Emitter&) = delete;

        void visit(const scc::asm_gen::Program& node) override;
        void visit(const scc::asm_gen::FunctionDefinition& node) override;

        void visit(const scc::asm_gen::Mov& node) override;
        void visit(const scc::asm_gen::Ret& node) override;
        
        void visit(const scc::asm_gen::Immediate& node) override;
        void visit(const scc::asm_gen::Register& node) override;
        
        void emit(std::string_view text);

    };
} // namespace scc::code_em