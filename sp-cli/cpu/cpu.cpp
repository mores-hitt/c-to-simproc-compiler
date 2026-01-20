#include "cpu/cpu.h"
#include "const/const.h"
#include "util/util.h"
#include "cpu/instructions.h"
#include "register/address.h"
#include "register/general_purpose.h"

#include <string>
#include <string_view>

namespace sp_cli
{
    CPU::CPU(std::string& code): memory(code){}

    bool CPU::execute(Instruction instruction) {
        switch (instruction.opcode)
        {
        case SP_INSTRUCTIONS::HLT :
            this->terminate = true;
            return true;
            break;
        default:
            return true;
            break;
        }
        return false;
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

        Instruction instruction = stringToInstruction(addressContent);

        #ifdef DEBUG_MODE
        if (instruction.opcode != SP_INSTRUCTIONS::NO_INST) {
            instruction.print();
            std::cout << AR.getSignedReg(ARKey::PC);
        }
        #endif

        AR.setReg(ARKey::PC, ++PC);
        if (instruction.opcode == SP_INSTRUCTIONS::NO_INST) {
            AR.setReg(ARKey::MAR, static_cast<uint16_t>(10));
        } else {
            AR.setReg(ARKey::MAR, static_cast<uint16_t> (0));
        }

        return instruction;
    }

    void CPU::runStep() {
        fetch();
        // run instruction
        // print cpu state??
    }

    void CPU::printState() {
        this->AR.printRegs();
        this->GPR.printRegs();
    }

    void CPU::run(){
        int noInstCounter {};
        for (size_t i = 0; i < MAX_ADDRESS; i++) {
            runStep();
            if (static_cast<int>(this->AR.getUnsignedReg(ARKey::MAR)) == 10) {
                noInstCounter++;
            } else {
                noInstCounter = 0;
            }

            if (noInstCounter > 700) {
                return;
            }
        }
    }




} // namespace sp_cli
