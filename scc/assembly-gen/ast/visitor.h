#pragma once

namespace scc::asm_gen
{
    class Program;
    class FunctionDefinition;
    class Immediate;
    class Register;
    class Mov;
    class Ret;

    class Visitor {
    public:

        virtual ~Visitor() = default;

        virtual void visit(const Program& node) = 0;
        virtual void visit(const FunctionDefinition& node) = 0;

        virtual void visit(const Mov& node) = 0;
        virtual void visit(const Ret& node) = 0;
        
        virtual void visit(const Immediate& node) = 0;
        virtual void visit(const Register& node) = 0;
    };
} // namespace scc::asm_gen
