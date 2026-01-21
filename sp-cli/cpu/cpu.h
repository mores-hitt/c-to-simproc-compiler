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
        Instruction stringToInstruction(std::string_view memContent);
        Instruction fetch();
        bool execute(Instruction instruction);

        public:
        CPU(std::string& code);
        void run();
        void runStep();
        void printState();
        CPUState getState();
    };
} // namespace sp_cli
