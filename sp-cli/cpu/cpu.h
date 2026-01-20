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
    class CPU {
        private:
        Memory memory;
        GeneralPurposeRegisters GPR {};
        AddressRegisters AR {};
        ControlFlags CF {};
        bool terminate {};

        Instruction stringToInstruction(std::string_view memContent);
        Instruction fetch();
        bool execute(Instruction instruction);

        public:
        CPU(std::string& code);
        void run();
        void runStep();
        void printState();
    };
} // namespace sp_cli
