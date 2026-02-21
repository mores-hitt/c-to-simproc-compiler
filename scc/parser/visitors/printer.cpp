#include <iostream>

#include "parser/visitors/printer.h"
#include "parser/ast/nodes.h"
#include "parser/ast/expression-nodes.h"
#include "parser/ast/statement-nodes.h"

namespace scc::parser
{
    void Printer::visit(const ProgramNode& node) {
        std::cerr << "Program(\n";
        ++m_depth;
        node.getFunction().accept(*this);
        --m_depth;
        std::cerr << ")\n";
    }

    void Printer::visit(const FunctionDefinitionNode& node) {
        std::string spaces(m_depth * 2, ' ');

        std::cerr << spaces << "Function(\n"
                  << spaces << "  name=\"" << node.getName() << "\",\n"
                  << spaces << "  body=";
        ++m_depth;
        node.getBody().accept(*this);
        --m_depth;
        std::cerr << spaces << ")\n";
    }

    void Printer::visit(const ReturnNode& node) {
        std::string spaces(m_depth * 2, ' ');
    
        std::cerr << "Return(\n";
        ++m_depth;
        node.getExpression().accept(*this);
        --m_depth;
        std::cerr << spaces << ")\n";
    }

    void Printer::visit(const IntegerConstantNode& node) {
        std::string spaces(m_depth * 2, ' ');
        std::cerr << spaces << "IntegerConstant(" << node.getInteger() << ")\n";
    }
} // namespace scc::parser
