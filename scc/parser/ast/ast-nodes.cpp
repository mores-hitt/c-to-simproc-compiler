#pragma once

#include <string_view>
#include <string>
#include <iostream>

#include "ast-nodes.h"

namespace scc
{
    void IntegerConstantNode::print(int depth) const {
        std::string spaces {(depth * 2, ' ')};

        std::cout << spaces << "IntegerConstant(" << m_integer << ")";
    }

    void IntegerConstantNode::expr() const {
        std::cout << "override funciona en herencia de ExpressionNode a IntegerConstantNode\n";
    }

    void ReturnNode::print(int depth) const {
        std::string spaces {(depth * 2, ' ')};
    
        std::cout << spaces << "Return(\n";
        m_expression->print(depth+1);
        std::cout << "\n" << spaces << ")";
    }

    void ReturnNode::stmnt() const {
        std::cout << "override funciona en herencia de StatementNode a ReturnNode\n";
    }

    void FunctionDefinitionNode::print(int depth) const {
        std::string spaces {(depth * 2, ' ')};

        std::cout << spaces << "Function(\n"
                  << spaces << "  name=\"" << m_name << "\",\n"
                  << spaces << "  body=";
        m_body->print(depth+1);
        std::cout << spaces << ")";
    }

    void FunctionDefinitionNode::funcDef() const {
        std::cout << "Funciona funcDef supongo lol";
    }

    void ProgramNode::print(int depth) const {
        std::cout << "Program(\n";
        m_functionDefinition->print(depth + 1);
        std::cout << ")\n";
    }

    void ProgramNode::prgrm() const {
        std::cout << "Functiona prgrm supongo lol";
    }
} // namespace scc