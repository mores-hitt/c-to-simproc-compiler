#pragma once

#include "const/const.h"
#include "util/util.h"
#include "register/address.h"
#include "register/general_purpose.h"
#include "memory/ram.h"
#include "cpu/instructions.h"
#include "register/flags.h"

#include <string_view>
#include <string>
#include <variant>


namespace sp_cli
{
    enum class Operands {
        LEFT,
        RIGHT
    };

    constexpr std::string_view getOperandsName(Operands op) {
        switch (op)
        {
        case Operands::LEFT : return "LEFT";
        case Operands::RIGHT : return "RIGHT";
        }
        return "UNKNOWN";
    }

    inline std::ostream& operator<<(std::ostream& out, Operands op) {
        return out << getOperandsName(op);
    }

    struct CPUState {
        const GeneralPurposeRegisters& GPR;
        const AddressRegisters& AR;
        const ControlFlags& CF;
        const bool terminate;
        const bool error;
        const std::string& exitMessage;
        const Memory& memory;
    };

    class CPU {
        private:
        Memory memory;
        GeneralPurposeRegisters GPR {};
        AddressRegisters AR {};
        ControlFlags CF {};
        bool terminate {};
        bool error {};
        std::string exitMessage;

        void completeInstruction(bool terminate = false, bool error = false, std::string exitMessage = "Everything good");
        uint16_t memoryContentToI16(int address);
        std::variant<GPRKey, ARKey, uint16_t> operandToAddressOrReg(Instruction instruction, Operands op, int base = 16);
        uint16_t read(const std::variant<GPRKey, ARKey, uint16_t>& operand);
        void write(const std::variant<GPRKey, ARKey, uint16_t>& operand, uint16_t value);
        float readFloat(const std::variant<GPRKey, ARKey, uint16_t>& operand);
        void writeFloat(const std::variant<GPRKey, ARKey, uint16_t>& operand, float value);

        template<typename T>
        void setFlags(T value);

        Instruction stringToInstruction(std::string_view memContent);
        Instruction fetch();
        void execute(Instruction instruction);


        public:
        CPU(std::string& code);
        void run();
        void runStep();
        void printState() const;
        void printStatus() const;
        [[nodiscard]] CPUState getState() const;
    };
} // namespace sp_cli
