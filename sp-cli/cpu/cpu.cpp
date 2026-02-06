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
#include <iomanip>

namespace sp_cli
{
    CPU::CPU(const std::string& code): memory(code), exitMessage(" "){
        uint16_t bp {STACK_ADDRESS};
        AR.setReg(ARKey::BP, bp);
        AR.setReg(ARKey::SP, bp);
    }

    void CPU::completeInstruction(bool terminate, bool error, const std::string_view& exitMessage){
        this->error = error;
        this->terminate = terminate;
        this->exitMessage = exitMessage;
    }

    uint16_t CPU::memoryContentToI16(size_t address) const {
        std::string content(this->memory.get(address));
        uint16_t bitContent {0};
        if (content != "") {
            bitContent = static_cast<uint16_t>(std::stoi(content, nullptr, 2));
        }
        return bitContent;
    }

    std::variant<GPRKey, ARKey, uint16_t> CPU::operandToAddressOrReg(const Instruction& instruction, Operands op, int base) const {
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

        if (stringOperand == "AX") return GPRKey::AX;
        if (stringOperand == "BX") return GPRKey::BX;
        if (stringOperand == "CX") return GPRKey::CX;
        if (stringOperand == "PC") return ARKey::PC;
        if (stringOperand == "SP") return ARKey::SP;
        if (stringOperand == "BP") return ARKey::BP;
        if (stringOperand == "MAR") return ARKey::MAR;

        // Otherwise, it's a memory address or immediate value
        std::uint16_t value {};
        std::from_chars(stringOperand.data(), stringOperand.data() + stringOperand.size(), value, base);
        return value;

    }

    uint16_t CPU::read(const std::variant<GPRKey, ARKey, uint16_t>& operand) const {
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

    std::optional<float> CPU::readFloat(const std::variant<GPRKey, ARKey, uint16_t>& operand) const {
        if (std::holds_alternative<GPRKey>(operand)) { 
            uint16_t regMsb {read(GPRKey::BX)};
            uint16_t regLsb {read(GPRKey::AX)};
            return makeFloat(regLsb, regMsb);
        } else if (std::holds_alternative<uint16_t>(operand)) {
            uint16_t address {std::get<uint16_t>(operand)};
            uint16_t memMsb {read(address)};
            address++;
            uint16_t memLsb {read(address)};
            return makeFloat(memLsb, memMsb);
        } else {
            return std::nullopt;
        }
    }

    void CPU::writeFloat(const std::variant<GPRKey, ARKey, uint16_t>& operand, float value) {
        if (std::holds_alternative<GPRKey>(operand)) { 
            FloatParts floatParts {splitFloat(value)};
            write(GPRKey::BX, floatParts.msb);
            write(GPRKey::AX, floatParts.lsb);
            return;
        } else if (std::holds_alternative<uint16_t>(operand)) {
            FloatParts floatParts {splitFloat(value)};
            uint16_t address {(std::get<uint16_t>(operand))};
            write(address, floatParts.msb);
            address++;
            write(address, floatParts.lsb);
            return;
        } else {
            return;
        }
    }

    template<typename T>
    void CPU::setFlags(T value) {
        if (value == 0){
            this->CF.setFlag(Flags::Z);
        } else if (value < 0) {
            this->CF.setFlag(Flags::N);
        }
    }

    void CPU::execute(const Instruction& instruction)  {
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
            // Bugged behaviour: Instruction does not seem affected by control flags, and creates numbers over 16
            // bit of lenght

            auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
            uint16_t content {read(operand)};
            write(operand, ++content);
            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::DEC : {
            // Decremento en 1 el destino especificado,  Si el destino queda = 0, se vuelve Z = 1
            // bugged behaviour: Instruction does not seem affected by control flags

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
        case SP_INSTRUCTIONS::ROL : {
            /*
            Rota los bits a la izquierda las veces especificadas(en decimal),
            los bits que salen por la izquierda re-entran por la Derecha.
            En el Carry Flag queda el ultimo bit rotado.
            */
            
            auto destination {operandToAddressOrReg(instruction, Operands::LEFT)};
            auto shift {operandToAddressOrReg(instruction, Operands::RIGHT, 10)};

            uint16_t content {read(destination)};
            uint16_t int16Shift {std::get<uint16_t>(shift)};
            uint16_t reverseShift {static_cast<uint16_t>(16 - int16Shift)};
            uint16_t aux {static_cast<uint16_t>(content >> (reverseShift))};

            content = static_cast<uint16_t>(content << int16Shift);
            content |= aux;

            write(destination, content);
            completeInstruction();

            bool carry {(aux & 1) != 0};
            if (carry) {
                CF.setFlag(Flags::C);
            } else {
                CF.clearFlag(Flags::C);
            }

            return;

        }
        case SP_INSTRUCTIONS::ROR : {
            /*
            Rota los bits a la derecha las veces especificadas(en decimal),
            los Bits que salen por la derecha re-entran por la izquierda.
            El Carry Flag guarda el ultimo bit rotado.
            */

            auto destination {operandToAddressOrReg(instruction, Operands::LEFT)};
            auto shift {operandToAddressOrReg(instruction, Operands::RIGHT, 10)};

            uint16_t content {read(destination)};
            uint16_t int16Shift {std::get<uint16_t>(shift)};
            uint16_t reverseShift {static_cast<uint16_t>(16 - int16Shift)};
            uint16_t aux {static_cast<uint16_t>(content << (reverseShift))};

            content = static_cast<uint16_t>(content >> int16Shift);
            content |= aux;

            write(destination, content);
            completeInstruction();

            bool carry {(aux & (1 << 15)) != 0}; // check last rotated bit
            if (carry) {
                CF.setFlag(Flags::C);
            } else {
                CF.clearFlag(Flags::C);
            }

            return;
        }
        case SP_INSTRUCTIONS::SHL : {
            /*
            Desplaza los bits a la izquierda el numero de veces especificado(en decimal),
            agregando ceros a la derecha, el Carry Flag guarda ultimo bit desplazado.
            */

            auto destination {operandToAddressOrReg(instruction, Operands::LEFT)};
            auto shift {operandToAddressOrReg(instruction, Operands::RIGHT, 10)};

            uint16_t content {read(destination)};
            uint16_t int16Shift {std::get<uint16_t>(shift)};
            uint16_t shiftMinusOne {static_cast<uint16_t>(int16Shift - 1)};
            bool carry { ((static_cast<uint16_t>(content << shiftMinusOne)) & (1 << 15)) != 0}; // to not lose last shifted bit, we shift one less, then detect if las bit is 0 or 1

            content = static_cast<uint16_t>(content << int16Shift);
            
            write(destination, content);
            completeInstruction();

            if(carry) {
                CF.setFlag(Flags::C);
            } else {
                CF.clearFlag(Flags::C);
            }

            return;

        }
        case SP_INSTRUCTIONS::SHR : {
            /*
            Desplaza los bits a la Derecha el numero de veces especificado(en decimal),
            agregando ceros a la izquierda, el Carry Flag guarda ultimo bit desplazado.
            */

            auto destination {operandToAddressOrReg(instruction, Operands::LEFT)};
            auto shift {operandToAddressOrReg(instruction, Operands::RIGHT, 10)};

            uint16_t content {read(destination)};
            uint16_t int16Shift {std::get<uint16_t>(shift)};

            uint16_t shiftMinusOne {static_cast<uint16_t>(int16Shift - 1)};
            bool carry { ((static_cast<uint16_t>(content >> shiftMinusOne)) & 1) != 0}; // to not lose last shifted bit, we shift one less, then detect if first bit is 0 or 1

            content = static_cast<uint16_t>(content >> int16Shift);
            
            write(destination, content);
            completeInstruction();

            if(carry) {
                CF.setFlag(Flags::C);
            } else {
                CF.clearFlag(Flags::C);
            }

            return;
        }
        case SP_INSTRUCTIONS::ADD : {
            /*
            AX = AX + el contenido de la dirección de memoria.
            Si el resultado de la suma supera los 16 bits, el
            resultado queda asi: en BX los bits mas significativos -> documentation has a typo lol
            y en BX los menos, tambien se activa el Overflow flag.
            */

            auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
            uint16_t memContent {read(operand)};
            uint16_t axContent {read(GPRKey::AX)};
            uint16_t sum {static_cast<uint16_t>(memContent + axContent)};

            write(GPRKey::AX, sum);
            completeInstruction();

            CF.clear();

            if (sum < axContent || sum < memContent) { // if sum is smaller than one of its operands, then an overflow happened
                write(GPRKey::BX, 0x001); // simuproc stores overflow bit in BX
                CF.setFlag(Flags::O); // simuproc signals overflow
            }

            return;
        }
        case SP_INSTRUCTIONS::SUB : {
            // AX = AX - el contenido de la dirección de memoria.

            auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
            uint16_t memContent {read(operand)};
            uint16_t axContent {read(GPRKey::AX)};
            uint16_t sub;

            CF.clear();

            if (memContent > axContent) {
                sub = static_cast<uint16_t>(memContent - axContent);
                CF.setFlag(Flags::N);
            } else {
                sub = static_cast<uint16_t>(axContent - memContent);
            }

            write(GPRKey::AX, sub);
            setFlags(sub);
            completeInstruction();

            return;
        }
        case SP_INSTRUCTIONS::MUL : {
            /*
            AX = AX * el contenido de la dirección de memoria.
            Si el numero resultante supera su longitud en binario de 16 bits,
            este resultado se parte almacenando los bits mas significativos en el Registro BX.
            */
            // This instruction is bugged. it stores every result bit (even over 16 bits) on AX. no flags
            // to be faithful to the real behaviour, it will imitate this bug as close as possible,
            // but i discourage ever using this instruction in this emulator
            // it also resets n flag before execution

            CF.clearFlag(Flags::N);

            auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
            uint16_t memContent {read(operand)};
            uint16_t axContent {read(GPRKey::AX)};

            int result = memContent * axContent;
            uint16_t leastSignificant = static_cast<uint16_t>(result & 0xFFFF); // get least significant bits
            CF.clear();
            setFlags(result);

            write(GPRKey::AX, leastSignificant);
            return;
        }
        case SP_INSTRUCTIONS::DIV : {
            /*
            AX = AX / el contenido de la dirección de memoria,
            BX=AX % el contenido de la dir de memoria (BX = modulo o residuo).
            */
           
            auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
            uint16_t memContent {read(operand)};
            uint16_t axContent {read(GPRKey::AX)};

            if (memContent == 0) {
                std::string msg {"Su programa intentó hacer una división por cero. Simulación detenida"};
                completeInstruction(true, true, msg);
                return;
            }

            uint16_t result = static_cast<uint16_t>(axContent / memContent);
            uint16_t remainder = static_cast<uint16_t>(axContent % memContent);

            CF.clear();
            setFlags(result);

            write(GPRKey::AX, result);
            write(GPRKey::BX, remainder);

            completeInstruction();
            return;


        }
        case SP_INSTRUCTIONS::CLN : {
            // Limpia el Negative Flag.  N = 0

            CF.clearFlag(Flags::N);
            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::CLC : {
            // Limpia el Carry Flag.  C = 0

            CF.clearFlag(Flags::C);
            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::STC : {
            // Pone el Carry Flag.  C = 1

            CF.setFlag(Flags::C);
            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::CMC : {
            // Complementa (invierte) el Carry Flag.  Si C = 1 vuelve C = 0 y viceversa.

            CF.toggleFlag(Flags::C);
            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::LOOP : {
            // Decrementa CX y salta a la Pos de memoria si CX no es cero.

            auto address {operandToAddressOrReg(instruction, Operands::LEFT)};
            uint16_t cxContent {read(GPRKey::CX)};
            if (cxContent == 0 || --cxContent == 0) {
                write(GPRKey::CX, 0);
                completeInstruction();
                return;
            }
            write(GPRKey::CX, cxContent);
            write(ARKey::PC, std::get<uint16_t>(address));
            completeInstruction();
            return;

        }
        case SP_INSTRUCTIONS::JMP : {
            // Salto incondicional.  PC = dirección de memoria donde esta la siguiente instrucción a ejecutar"

            auto address {operandToAddressOrReg(instruction, Operands::LEFT)};
            write(ARKey::PC, std::get<uint16_t>(address));
            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::JEQ : {
            // Saltar si son iguales.
            // Si Z = 1, PC = contenido de la memoria.

            if (CF.getFlag(Flags::Z)) {
                auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
                uint16_t memory {std::get<uint16_t>(operand)};
                write(ARKey::PC, memory);
            }

            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::CMP : {
            /*
            Compara AX con [mem], si AX es mayor,Z=0 N=0,
            si es igual Z=1 N=0, si es menor Z=0 N=1 
            */
            
            CF.clear();
            uint16_t axContent {read(GPRKey::AX)};
            auto mem {operandToAddressOrReg(instruction, Operands::LEFT)};
            uint16_t memContent {read(mem)};
            if (axContent == memContent) {
                CF.setFlag(Flags::Z);
            } else if (axContent < memContent) {
                CF.setFlag(Flags::N);
            }
            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::JME : {
            // Saltar si es Menor. Si N = 1, PC = contenido de la memoria. 

            if (CF.getFlag(Flags::N)) {
                auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
                uint16_t memory {std::get<uint16_t>(operand)};
                write(ARKey::PC, memory);
            }

            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::JMA : {
            // Saltar si es Mayor. Si Z = 0 y N = 0, PC = contenido de memoria.

            if (!CF.getFlag(Flags::N) && !CF.getFlag(Flags::Z)) {
                auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
                uint16_t memory {std::get<uint16_t>(operand)};
                write(ARKey::PC, memory);
            }

            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::JC : {
            // Saltar si el Carry Flag esta activado. Si C = 1, PC = contenido de memoria.

            if (CF.getFlag(Flags::C)) {
                auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
                uint16_t memory {std::get<uint16_t>(operand)};
                write(ARKey::PC, memory);
            }

            completeInstruction();
            return;            
        }
        case SP_INSTRUCTIONS::JNC : {
            // Saltar si el Carry Flag no esta activado. Si C = 0, PC = contenido de memoria
            
            if (!CF.getFlag(Flags::C)) {
                auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
                uint16_t memory {std::get<uint16_t>(operand)};
                write(ARKey::PC, memory);
            }

            completeInstruction();
            return; 
        }
        case SP_INSTRUCTIONS::JO : {
            // Saltar si el Overflow Flag esta Activado. Si O = 1, PC = contenido de memoria

            if (CF.getFlag(Flags::O)) {
                auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
                uint16_t memory {std::get<uint16_t>(operand)};
                write(ARKey::PC, memory);
            }

            completeInstruction();
            return; 
        }
        case SP_INSTRUCTIONS::JNO : {
            // Saltar si el Overflow Flag no esta activado. Si O = 0, PC = contenido de memoria

            if (!CF.getFlag(Flags::O)) {
                auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
                uint16_t memory {std::get<uint16_t>(operand)};
                write(ARKey::PC, memory);
            }

            completeInstruction();
            return; 
        }
        case SP_INSTRUCTIONS::JNE : {
            // Saltar si no son iguales. Si Z = 0, PC = contenido de memoria.

            if (!CF.getFlag(Flags::Z)) {
                auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
                uint16_t memory {std::get<uint16_t>(operand)};
                write(ARKey::PC, memory);
            }

            completeInstruction();
            return; 
        }
        case SP_INSTRUCTIONS::LDT : {
            // Lee un valor del Teclado y lo lleva al registro AX

            if (instruction.left_operand != "") {
                std::cout << instruction.left_operand << '\n';
            }
            std::cout << "ingresaras un decimal o un binario?\n1) decimal\n2) binario\n";
            int option;
            std::cin >> option;

            uint16_t bitContent {0};

            //do {
                std::cout << "ingresa tu numero";
                std::string buffer;
                std::cin >> buffer;
            //} while (bitContent > 65535 || bitContent < 0);

            if (option == 2) {
                bitContent = static_cast<uint16_t>(std::stoi(buffer, nullptr, 2));
            } else {
                bitContent = static_cast<uint16_t>(std::stoi(buffer));
            }

            write(GPRKey::AX, bitContent);
            completeInstruction();
            return;

        }
        case SP_INSTRUCTIONS::EAP : {
            // Escribe en Pantalla el contenido del registro AX
            // show in decimal

            if (instruction.left_operand != "") {
                std::cout << instruction.left_operand << '\n';
            }
            auto axContent {read(GPRKey::AX)};
            std::cout << axContent << '\n';

            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::MSG : {
            // Muestra un mensaje en pantalla
            if (instruction.left_operand != "") {
                std::cout << instruction.left_operand << '\n';
            }
            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::LDB : {
            // La instrucción carga en AX el contenido de memoria almacenado en [mem] + BX
            // simuproc checks at runtime if sum is bigger than 0xFFF
            // giving the user the choice of continuing execution

            uint16_t bxContent {read(GPRKey::BX)};
            auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
            uint16_t address {std::get<uint16_t>(operand)};
            if ((bxContent + address) > MAX_ADDRESS) {
                std::cout << "El valor de bx usado para sumarle a la Dir de mem de LDB\n";
                std::cout << "Ha superado el fin de memoria.\n Desea pausar la simulación?\n";
                std::cout << "1) si\n2) no\n";
                int option;
                std::cin >> option;
                if (option == 1) {
                    completeInstruction(true, true, "Simulación terminada");
                    return;
                } else {// if user chooses to continue, simuproc loads 0 into AX
                    write(GPRKey::AX, 0);
                    completeInstruction();
                    return;
                }
            } else {
                uint16_t effectiveAddress {static_cast<uint16_t>(address + bxContent)};
                uint16_t effectiveAddressContent {read(effectiveAddress)};
                write(GPRKey::AX, effectiveAddressContent);
                completeInstruction();
                return;
            }


        }
        case SP_INSTRUCTIONS::STB : {
            // guarda el contenido de AX en la dirección [mem] + BX
            // if effective address is bigger than 0xFFF, it writes in 0xFFF

            uint16_t bxContent {read(GPRKey::BX)};
            auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
            uint16_t address {std::get<uint16_t>(operand)};
            auto axContent {read(GPRKey::AX)};

            if ((bxContent + address) > MAX_ADDRESS) {
                std::cout << "El valor de bx usado para sumarle a la Dir de mem de LDB\n"; // simuproc says here "LDB" despite being the STB instruction
                std::cout << "Ha superado el fin de memoria.\n Desea pausar la simulación?\n";
                std::cout << "1) si\n2) no\n";
                int option;
                std::cin >> option;
                if (option == 1) {
                    completeInstruction(true, true, "Simulación terminada");
                    return;
                } else {// if user chooses to continue, simuproc writes AX into 0xFFF
                    write(MAX_ADDRESS, axContent);
                    completeInstruction();
                    return;
                }
            } else {
                uint16_t effectiveAddress {static_cast<uint16_t>(address + bxContent)};
                write(effectiveAddress, axContent);
                completeInstruction();
                return;
            }


        }
        case SP_INSTRUCTIONS::LDF : {
            /*
            Carga en BX y AX un numero de 32 bits (IEEE) que esta almacenado
            en la dir [mem] y mem+1. En BX quedan los digitos mas Significativos
            */
           // for this instruction, Simuproc explicitly prevents FFF as a operand,
           // so we assume that it is clean when its being executed

            auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
            uint16_t address {(std::get<uint16_t>(operand))}; // static_cast<uint16_t>
            auto floatContent {readFloat(address)};
            if (floatContent) {
                writeFloat(GPRKey::AX, *floatContent);
                completeInstruction();
                return;
            }

            completeInstruction(true, true, "Invalid operand in LDF instruction");
            return;

        }
        case SP_INSTRUCTIONS::STF : {
            // Guarda en [mem] y mem+1 el contenido de BX y AX
            // for this instruction, Simuproc explicitly prevents FFF as a operand,
            // so we assume that it is clean when its being executed
            
            auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};
            uint16_t address {(std::get<uint16_t>(operand))};
            auto floatContent {readFloat(GPRKey::AX)};

            if (floatContent) {
                writeFloat(address, *floatContent);
                completeInstruction();
                return;
            }

            completeInstruction(true, true, "Invalid operand in STF instruction");
            return;


        }
        case SP_INSTRUCTIONS::ADDF : {
            /*
            Suma números de 32 bits: En BX y AX queda el resultado
            de la suma de estos mas el contenido de [mem] y mem+1
            */

            auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};

            auto memFloat {readFloat(operand)};

            if (!memFloat) {
                completeInstruction("Invalid operand in ADDF instruction");
                return;
            }

            auto regFloat {readFloat(GPRKey::AX)};

            if (!regFloat) {
                completeInstruction("Invalid operand in ADDF instruction");
                return;
            }


            float result = *regFloat + *memFloat;
            writeFloat(GPRKey::AX, result);

            CF.clear();

            setFlags(result);

            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::SUBF : {
            // Resta el numero de 32 bits: BX y AX = BX y AX - [mem]y mem+1

            auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};

            auto memFloat {readFloat(operand)};

            if (!memFloat) {
                completeInstruction("Invalid operand in SUBF instruction");
                return;
            }

            auto regFloat {readFloat(GPRKey::AX)};

            if (!regFloat) {
                completeInstruction("Invalid operand in SUBF instruction");
                return;
            }

            float result = *regFloat - *memFloat;
            writeFloat(GPRKey::AX, result);

            CF.clear();

            setFlags(result);

            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::MULF : {
            // Multiplicación: BX y AX = BX y AX * [mem]y mem+1

            auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};

            auto memFloat {readFloat(operand)};

            if (!memFloat) {
                completeInstruction("Invalid operand in MULF instruction");
                return;
            }

            auto regFloat {readFloat(GPRKey::AX)};

            if (!regFloat) {
                completeInstruction("Invalid operand in MULF instruction");
                return;
            }

            float result = *regFloat * *memFloat;
            writeFloat(GPRKey::AX, result);

            CF.clear();

            setFlags(result);

            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::DIVF : {
            // BX y AX = BX y AX / [mem]y mem+1
            // en CX queda el residuo de la division en entero de 16 bits
            // currently, i do not understand how could there be a remainder in this arithmetic

            auto operand {operandToAddressOrReg(instruction, Operands::LEFT)};

            auto memFloat {readFloat(operand)};

            if (!memFloat) {
                completeInstruction("Invalid operand in DIVF instruction");
                return;
            }

            auto regFloat {readFloat(GPRKey::AX)};

            if (!regFloat) {
                completeInstruction("Invalid operand in DIVF instruction");
                return;
            }

            float result = *regFloat / *memFloat;
            writeFloat(GPRKey::AX, result);
            
            CF.clear();

            setFlags(result);

            completeInstruction();
            return;

        }
        case SP_INSTRUCTIONS::ITOF : {
            /*
            Convierte un número entero (16bits) almacenado en AX
            al mismo numero pero representado en Real IEEE754(32bits)
            el Resultado de la conversión queda en BX (bits mas significativos) y AX
            */

            uint16_t axContent {read(GPRKey::AX)};
            float floatNumber = axContent;
            CF.clear();
            if (CF.getFlag(Flags::N)){
                floatNumber *= -1;
            } else if (axContent == 0) {
                CF.setFlag(Flags::Z);
            } else if (axContent != 0) {
                CF.clearFlag(Flags::Z);
            }
            writeFloat(GPRKey::AX, floatNumber);

            completeInstruction();
            return;
        }
        case SP_INSTRUCTIONS::FTOI : {
            /*
            Convierte un número Real(32bits) a su equivalente en entero
            BX y AX en un entero (16bits), el Resultado queda en AX.
            */

            auto regFloat {readFloat(GPRKey::AX)};

            if (!regFloat) {
                completeInstruction("Invalid operand in FTOI instruction");
                return;
            }

            float floatNum {*regFloat};

            CF.clear();
            if (floatNum < 0) {
                CF.setFlag(Flags::N);
                floatNum *= -1;
            } else if (floatNum == 0) {
                CF.setFlag(Flags::Z);
            }

            uint16_t regInt;

            if (floatNum > MAX_16BIT || MAX_16BIT* (-1) > floatNum) {
                CF.setFlag(Flags::O);
                regInt = MAX_16BIT;
            } else {
                regInt = static_cast<uint16_t>(floatNum);
            }
            write(GPRKey::AX, regInt);

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
        case SP_INSTRUCTIONS::IN : {
            // Lleva al Registro el valor retornado por el puerto especificado.
            // currently only port 1 is implemented (read/write floats from user)
            // simuproc ignores the left operand when the port is 1
            // which means, the only valid IN instruction currently is "IN AX,1"

            std::cout << "Decimal Flotante positivo o negativo\n";
            float option;
            std::cin >> option;
            FloatParts floatParts {splitFloat(option)};
            write(GPRKey::BX, floatParts.msb);
            write(GPRKey::AX, floatParts.lsb);

            completeInstruction();
            return;

        }
        case SP_INSTRUCTIONS::OUT : {
            // Escribe en el puerto especificado, el valor del registro.
            // currently only port 1 is implemented (read/write floats from user)
            // simuproc ignores the left operand when the port is 1
            // which means, the only valid OUT instruction currently is "OUT 1,AX"

            uint16_t lsb {read(GPRKey::AX)};
            uint16_t msb {read(GPRKey::BX)};
            float num {makeFloat(lsb, msb)};
            std::cout << std::setprecision(16) << num << '\n';

            completeInstruction();
            return;
        }
        default:
            return;
        }
        return;
    }

    Instruction CPU::stringToInstruction(const std::string_view& memContent) {
        //read until new line, space or at most 4 characters
        auto it {memContent.cbegin()};
        auto end {memContent.cend()};
        Instruction instruction {};

        while (it != end && std::isalpha(*it)) {
            it++;
        }

        size_t opcodeLen = static_cast<size_t>(it - memContent.cbegin());
        instruction.opcode = getInstruction(memContent.substr(0, opcodeLen));

        // at this point, it is pointing either to white space or the end
        if (it == end) {
            return instruction;
        }
        
        it++; // now pointing to start of left parameter
        auto lStart = it; // save start of left parameter for append

        while (it != end && *it != ',') {
            it++;
        }

        size_t leftStart = static_cast<size_t>(lStart - memContent.cbegin());
        size_t leftLen = static_cast<size_t>(it - lStart);
        instruction.left_operand = memContent.substr(leftStart, leftLen);

        // now, it is pointing to comma or to the end
        if (it == end) {
            return instruction;
        }

        it++; // now pointing to the start of the right operand

        size_t rightStart = static_cast<size_t>(it - memContent.cbegin());
        size_t rightLen = static_cast<size_t>(end - it);
        instruction.right_operand = memContent.substr(rightStart, rightLen);
        
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

    void CPU::printState() const {
        this->AR.print();
        this->GPR.print();
        this->CF.print();
        size_t start = this->AR.getUnsignedReg(ARKey::PC) > MEMORY_PRINT_WINDOW ?
                    this->AR.getUnsignedReg(ARKey::PC) - MEMORY_PRINT_WINDOW:
                    0;
        
        size_t size = MEMORY_PRINT_WINDOW * 2;

        this->memory.print(start, size);

        // print stack memory
        std::cout << "\n" << "STACK MEMORY:\n";
        this->memory.print(STACK_ADDRESS, MEMORY_PRINT_WINDOW);
    }

    void CPU::run(){
        while(!this->terminate) {
            runStep();
        }
    }

    CPUState CPU::getState() const {
        return CPUState {
            this->GPR,
            this->AR,
            this->CF,
            this->terminate,
            this->error,
            this->exitMessage,
            this->memory,
        };
    }

    void CPU::printStatus() const {
        std::cout << "Error: " << error << '\n'
                  << "Message: " << exitMessage << '\n';
        return;
    }
} // namespace sp_cli
