#pragma once

#include "parser/ast/visitor.h"

namespace scc::parser
{
    class Printer : public Visitor {
    protected:
        size_t m_depth {0};    

    public:
        Printer() = default;
        ~Printer() override = default;

        Printer(const Printer&) = delete;
        Printer& operator=(const Printer&) = delete;

        void visit(const ProgramNode& node) override;
        void visit(const FunctionDefinitionNode& node) override;
        void visit(const ReturnNode& node) override;
        void visit(const IntegerConstantNode& node) override;

    };
} // namespace scc::parser
