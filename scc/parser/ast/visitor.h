#pragma once

namespace scc::parser
{

    class ProgramNode;
    class FunctionDefinitionNode;
    class ReturnNode;
    class IntegerConstantNode;

    class Visitor {
    public:

        virtual ~Visitor() = default;

        virtual void visit(const ProgramNode& node) = 0;
        virtual void visit(const FunctionDefinitionNode& node) = 0; 
        virtual void visit(const ReturnNode& node) = 0;
        virtual void visit(const IntegerConstantNode& node) = 0;

    };
} // namespace scc
