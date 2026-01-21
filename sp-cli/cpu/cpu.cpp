#include "cpu/cpu.h"
#include "const/const.h"
#include "util/util.h"
#include "cpu/instructions.h"
#include "register/address.h"
#include "register/general_purpose.h"
#include "register/flags.h"

#include <string>
#include <string_view>
#include <charconv>
#include <bitset>

namespace sp_cli
{
    CPU::CPU(std::string& code): memory(code), exitMessage(" "){
        uint16_t bp {STACK_ADDRESS};
        AR.setReg(ARKey::BP, bp);
        AR.setReg(ARKey::SP, bp);
    }

    void CPU::completeInstruction(bool terminate, bool error, std::string exitMessage){
        this->error = error;
        this->terminate = terminate;
        this->exitMessage = exitMessage;
    }

    int CPU::addressOperandToInt(Instruction instruction, Operands op) {
        switch (op)
        {
        case Operands::LEFT : {
            std::string left_op(instruction.left_operand);
            return std::stoi(left_op);
        }
        case Operands::RIGHT : {
            std::string right_op(instruction.left_operand);
            return std::stoi(right_op);
        }
        default:
            return -1;
        }
        return -1;
    }

    GPRKey CPU::registerOperandToKey(Instruction instruction, Operands op) {
        std::string operand;
        switch (op)
        {
        case Operands::LEFT : {
            operand = instruction.left_operand;
            break;
        }
        case Operands::RIGHT : {
            operand = instruction.right_operand;
            break;
        }
        }
        if (operand == "AX"){
            return GPRKey::AX;
        } else if (operand == "BX") {
            return GPRKey::BX;
        } else {
            return GPRKey::CX;
        }
    }

    std::string CPU::registerContentToString(GPRKey reg) {
        uint16_t content {GPR.getUnsignedReg(reg)};
        return std::bitset<16>(content).to_string();
    }

    void CPU::execute(Instruction instruction) {
        switch (instruction.opcode)
        {
        case SP_INSTRUCTIONS::NO_INST : {
            std::string exiMessage {"Intento de ejecución de una dirección de memoria en blanco en la dir: " + this->AR.getUnsignedReg(ARKey::PC)};
            completeInstruction(true, true, exitMessage);
            return;
        }
        case SP_INSTRUCTIONS::LDA : {
            /*
            Cargue en AX el contenido de la dirección de Memoria especificada. 
            Si es que es una instrucción, tirar error
            */

            int address {addressOperandToInt(instruction, Operands::LEFT)};

            std::string content(this->memory.get(address));

            uint16_t bitContent {0};

            if (content != "") {
                bitContent = static_cast<uint16_t>(std::stoi(content, nullptr, 2));
            }

            this->GPR.setReg(GPRKey::AX, bitContent);

            completeInstruction();

            return;

        }
        case SP_INSTRUCTIONS::STA : {
            // Guarde el contenido de AX en la dirección de Memoria especificada.
            int address {addressOperandToInt(instruction, Operands::LEFT)};

            std::string stringContent { registerContentToString(GPRKey::AX)};

            this->memory.set(address, stringContent);

            completeInstruction();
            return;

        }
        case SP_INSTRUCTIONS::XAB : {
            // Intercambia los valores de los registros AX y BX
            uint16_t ax {GPR.getUnsignedReg(GPRKey::AX)};
            uint16_t bx {GPR.getUnsignedReg(GPRKey::BX)};
            GPR.setReg(GPRKey::AX, bx);
            GPR.setReg(GPRKey::BX, ax);
            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::CLA : {
            // Hace AX = 0
            uint16_t zero {};
            GPR.setReg(GPRKey::AX, zero);
            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::NOP : {
            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::HLT : {
            std::string exiMessage {"Ha terminado su ejecución con éxito"};
            completeInstruction(true, false, exitMessage);
            return;
        }
        default:
            return;
        }
        return;
    }

    Instruction CPU::stringToInstruction(std::string_view memContent) {
        //read until new line, space or at most 4 characters
        auto it {memContent.cbegin()};
        auto end {memContent.cend()};
        Instruction instruction {};

        while (it != end && std::isalpha(*it)) {
            it++;
        }

        instruction.opcode = getInstruction(std::string_view(memContent.cbegin(), static_cast<size_t>(it - memContent.cbegin())));

        // at this point, it is pointing either to white space or the end
        if (it == end) {
            return instruction;
        }
        
        it++; // now pointing to start of left parameter
        auto lStart = it; // save start of left parameter for append

        while (it != end && *it != ',') {
            it++;
        }

        instruction.left_operand = std::string_view(lStart,static_cast<size_t>(it - lStart));

        // now, it is pointing to comma or to the end
        if (it == end) {
            return instruction;
        }

        it++; // now pointing to the start of the right operand
        instruction.right_operand = std::string_view(it, static_cast<size_t>( end - it));
        return instruction;

    }

    Instruction CPU::fetch() {
        // see the current value of PC
        // go to address
        // get instruction
        // PC + 1

        auto PC {AR.getUnsignedReg(ARKey::PC)};
        auto addressContent {this->memory.get(static_cast<int>(PC))};

        Instruction instruction {stringToInstruction(addressContent)};

        AR.setReg(ARKey::PC, ++PC);
        if (instruction.opcode == SP_INSTRUCTIONS::NO_INST) {
            AR.setReg(ARKey::MAR, static_cast<uint16_t>(10));
        } else {
            AR.setReg(ARKey::MAR, static_cast<uint16_t> (0));
        }

        return instruction;
    }

    void CPU::runStep() {
        #ifdef DEBUG_MODE
        std::cout << "###################### Program counter: " << AR.getUnsignedReg(ARKey::PC)
                  << " ########################" << '\n'
                  << "[DEBUG] Fetching instruction." << '\n';
        #endif

        Instruction inst = fetch();

        #ifdef DEBUG_MODE
        std::cout << "[DEBUG] Instruction fetched:" << '\n';
        inst.print();
        std::cout << "[DEBUG] Executing " << inst.opcode << '\n';
        #endif

        execute(inst);

        #ifdef DEBUG_MODE
        std::cout << "[DEBUG] CPU State after " << inst.opcode <<  '\n'; 
        printState();
        std::cout << "#################### End of instruction cycle #####################" << '\n';
        #endif
    }

    void CPU::printState() {
        this->AR.print();
        this->GPR.print();
        this->CF.print();
        int start = this->AR.getUnsignedReg(ARKey::PC) > MEMORY_PRINT_WINDOW ?
                    this->AR.getUnsignedReg(ARKey::PC) - MEMORY_PRINT_WINDOW:
                    0;
        
        int size = MEMORY_PRINT_WINDOW + this->AR.getUnsignedReg(ARKey::PC);

        this->memory.print(start, size);
    }

    void CPU::run(){
        for (size_t i {0}; i < MAX_ADDRESS; i++) {
            runStep();
            if (this->terminate) {
                return;
            }
        }
    }

    CPUState CPU::getState() {
        return CPUState {
            this->GPR,
            this->AR,
            this->CF,
            this->terminate,
            this->error,
            this->exitMessage,
        };
    }
} // namespace sp_cli
