#include <ios>
#include <ostream>
#include <string_view>
#include <bitset>
#include <iomanip>

#include "code-emission/simuproc/emitter.h"
#include "assembly-gen/ast/nodes.h"
#include "assembly-gen/ast/instruction-nodes.h"
#include "assembly-gen/ast/operand-nodes.h"

namespace scc::code_em
{   
    int Emitter::getOrAddConstant(int value) {
        for (size_t i = 0; i < m_constants.size(); i++) {
            if (m_constants[i] == value) {
                return 0xC01 + static_cast<int>(i); // found, index is the address offset
            }
        }
        m_constants.push_back(value);
        return 0xC01 + static_cast<int>(m_constants.size()) - 1;
    }
    
    void Emitter::emit(std::string_view text) {
        m_os << text << '\n';
        ++m_currentLine;
    }
    
    void Emitter::visit(const scc::asm_gen::Program& node) {
        emit("JMP 006; Program start at address 10");
        emit("0; Stack Pointer");
        emit("0; Base Pointer");
        emit("0; Temp Address");
        emit("0; Temp Address");
        emit("0; Temp Address");
        emit("MOV 001,C00; Bootstrap | Start Stack Pointer to D00");
        emit("MOV 002,001; Start Base Pointer to Stack Pointer");    
        
        node.getFunctionDefinition().accept(*this);
        
        emit("#C00");
        emit("0000110100000000; D00: Stack Start Address");
             
        for (auto& constant : m_constants) {
            m_os << std::bitset<16>(static_cast<unsigned long long>(constant)) << "; Constant " << constant << '\n';
        }
        
        // JUMP HACK INSTRUCTIONS
    }

    void Emitter::visit(const scc::asm_gen::FunctionDefinition& node){
        // store current line as a label using the nodes name (string name -> int line)
        m_functionToAddressMap[std::string(node.getName())] = m_currentLine;
        m_currentFunction = node.getName();
        
        // add function prologue if it isnt the main function
        if (m_currentFunction != "main") {
            emit("MOV BX001; \\");
            emit("LDA 002;  | Store caller base pointer in the stack");
            emit("STB 0; /");
            emit("MOV 002,001; Convert base pointer to stack pointer");
            emit("INC 001; Allocate enough memory. TO BE CHANGED");
        }
        
        for(auto& instruction : node.getInstructions()) {
            instruction->accept(*this);
        }
    }

    void Emitter::visit(const scc::asm_gen::Mov& node){
        //print movl\t
        m_os << "MOV ";
        node.getDestination().accept(*this);
        m_os << ',';
        node.getSource().accept(*this);
        m_os << '\n';
        ++m_currentLine;
    }

    void Emitter::visit([[maybe_unused]] const scc::asm_gen::Ret& node){
        emit("STA 003; Save AX to temp");   
        emit("MOV 001,002; SP = BP");        
        emit("MOV BX,001; BX = SP");    
        emit("LDB 0; Get caller BP. AX = caller BP");
        emit("STA 002; Destroy stack frame. BP = caller BP");
        
        if (m_currentFunction == "main") {
            emit("LDA 003; Obtain back AX");
            emit("HLT; End program");
        }
    }
        
    void Emitter::visit(const scc::asm_gen::Immediate& node){
        int address = getOrAddConstant(node.getValue());
        m_os << std::hex << std::setw(3) << address << std::dec;
    }

    void Emitter::visit([[maybe_unused]] const scc::asm_gen::Register& node){
        m_os << node.getReg();
    }
} // namespace scc::code_em
