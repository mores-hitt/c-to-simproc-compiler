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
#include <sstream>
#include <variant>

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

    uint16_t CPU::memoryContentToI16(int address) {
        std::string content(this->memory.get(address));
        uint16_t bitContent {0};
        if (content != "") {
            bitContent = static_cast<uint16_t>(std::stoi(content, nullptr, 2));
        }
        return bitContent;
    }

    std::variant<GPRKey, ARKey, uint16_t> CPU::operandToAddressOrReg(Instruction instruction, Operands op) {
        std::string stringOperand;
        switch (op)
        {
        case Operands::LEFT : {
            stringOperand = instruction.left_operand;
            break;
        }
        case Operands::RIGHT : {
            stringOperand = instruction.right_operand;
            break;
        }
        }

        // lets be super explicit

        if (stringOperand == "AX") {
            return GPRKey::AX;
        } else if (stringOperand == "BX") {
            return GPRKey::BX;
        } else if (stringOperand == "CX") {
            return GPRKey::CX;
        } else if (stringOperand == "PC") {
            return ARKey::PC;
        } else if (stringOperand == "SP") {
            return ARKey::SP;
        } else if (stringOperand == "BP") {
            return ARKey::BP;
        } else if (stringOperand == "MAR") {
            return ARKey::MAR;
        } else {
            uint16_t intOperand;
            std::from_chars(stringOperand.data(), stringOperand.data() + stringOperand.size(), intOperand, 16);
            return intOperand;
        }

    }

    uint16_t CPU::read(const std::variant<GPRKey, ARKey, uint16_t>& operand) {
        if (std::holds_alternative<GPRKey>(operand)) {
            return GPR.getUnsignedReg(std::get<GPRKey>(operand));
        } else if (std::holds_alternative<ARKey>(operand)){
            return AR.getUnsignedReg(std::get<ARKey>(operand));
        } else {
            return memoryContentToI16(std::get<uint16_t>(operand));
        }
    }

    void CPU::write(const std::variant<GPRKey, ARKey, uint16_t>& operand, uint16_t value) {
        if (std::holds_alternative<GPRKey>(operand)) {
            GPR.setReg(std::get<GPRKey>(operand), value);
        } else if (std::holds_alternative<ARKey>(operand)) {
            AR.setReg(std::get<ARKey>(operand), value);
        } else {
            std::string content {std::bitset<16>(value).to_string()};
            memory.set(std::get<uint16_t>(operand), content);
        }
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

            auto address {operandToAddressOrReg(instruction, Operands::LEFT)};
            uint16_t content {read(address)};
            write(GPRKey::AX, content);

            completeInstruction();

            return;

        }
        case SP_INSTRUCTIONS::STA : {
            // Guarde el contenido de AX en la dirección de Memoria especificada.

            uint16_t content { read(GPRKey::AX) };
            auto address { operandToAddressOrReg(instruction, Operands::LEFT) };
            write(address, content);

            completeInstruction();

            return;

        }
        case SP_INSTRUCTIONS::XAB : {
            // Intercambia los valores de los registros AX y BX

            uint16_t ax {read(GPRKey::AX)};
            uint16_t bx {read(GPRKey::BX)};
            write(GPRKey::AX, bx);
            write(GPRKey::BX, ax);
            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::CLA : {
            // Hace AX = 0

            uint16_t zero {};
            write(GPRKey::AX, zero);
            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::PUSH : {
            // Envía el valor del registro especificado a la pila

            auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
            uint16_t content {read(operand)};
            uint16_t spAddress {read(ARKey::SP)};
            write(spAddress, content);
            write(ARKey::SP, ++spAddress);

            completeInstruction();
            return;

        }
        case SP_INSTRUCTIONS::POP : {
            /*
            Trae de la Pila el ultimo Valor llevado por PUSH (indicado por el registro SP)
            y lo almacena en el registro especificado.
            */

            uint16_t sp {read(ARKey::SP)};
            uint16_t content {read(--sp)};

            write(sp, 0);

            auto reg {operandToAddressOrReg(instruction, Operands::LEFT)};
            write(reg, content);
            write(ARKey::SP, sp);

            completeInstruction();
            return;

        }
        case SP_INSTRUCTIONS::INC : {
            // Incrementa en 1 el destino especificado, el parámetro puede ser una dirección de memoria o un registro.
            // Instruction does not seem affected by control flags

            auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
            uint16_t content {read(operand)};
            write(operand, ++content);
            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::DEC : {
            // Decremento en 1 el destino especificado,  Si el destino queda = 0, se vuelve Z = 1
            // Instruction does not seem affected by control flags

            auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
            uint16_t content {read(operand)};
            content--;
            if (content == 0) {
                CF.setFlag(Flags::Z);
            } else if (content == MAX_16BIT) { // if it decrements below 0 (-1 or 0xFFFF), simuproc sets it to 1. sets N flag
                content = 1;
                CF.setFlag(Flags::N);
            }
            write(operand, content);
            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::MOV : {
            /*
            Copia el valor almacenado en el origen al destino.  El destino y/o origen pueden ser
            registros o direcciones de memoria o combinación de estos.
            */

            auto origin {operandToAddressOrReg(instruction, Operands::RIGHT)};
            auto destination {operandToAddressOrReg(instruction, Operands::LEFT)};
            uint16_t content {read(origin)};
            write(destination, content);

            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::AND : {
            /*
            Y lógico, hace un Y lógico entre todos los bits de los
            dos operándos escribiendo el resultado en el destino.
            Los parámetros pueden ser direcciones de memoria o Registros.
            */
            auto leftOp {operandToAddressOrReg(instruction, Operands::LEFT)};
            auto rightOp {operandToAddressOrReg(instruction, Operands::RIGHT)};

            uint16_t leftContent {read(leftOp)};
            uint16_t rightContent {read(rightOp)};
            uint16_t result = leftContent & rightContent;
            write(leftOp, result);

            completeInstruction();
            return;

        }
        case SP_INSTRUCTIONS::NOT : {
            // NO lógico, invierte los bits del operando formando el complemento del primero.
            
            auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
            uint16_t content {read(operand)};
            content = ~content;
            write(operand, content);

            completeInstruction();
            return;

        }
        case SP_INSTRUCTIONS::OR : {
            // O inclusive lógico, todo bit activo en cualquiera de los operándoos será activado en el destino.
            auto leftOp {operandToAddressOrReg(instruction, Operands::LEFT)};
            auto rightOp {operandToAddressOrReg(instruction, Operands::RIGHT)};

            uint16_t leftContent {read(leftOp)};
            uint16_t rightContent {read(rightOp)};
            uint16_t result = leftContent | rightContent;
            write(leftOp, result);

            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::XOR : {
            // O exclusivo, realiza un O exclusivo entre los operándoos y almacena el resultado en destino
            auto leftOp {operandToAddressOrReg(instruction, Operands::LEFT)};
            auto rightOp {operandToAddressOrReg(instruction, Operands::RIGHT)};

            uint16_t leftContent {read(leftOp)};
            uint16_t rightContent {read(rightOp)};
            uint16_t result = leftContent ^ rightContent;
            write(leftOp, result);

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

        auto pc {read(ARKey::PC)};
        auto addressContent {this->memory.get(static_cast<int>(pc))};

        Instruction instruction {stringToInstruction(addressContent)};

        write(ARKey::PC, ++pc);
        
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

        // print stack memory
        std::cout << "\n" << "STACK MEMORY:\n";
        this->memory.print(STACK_ADDRESS, MEMORY_PRINT_WINDOW);
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
