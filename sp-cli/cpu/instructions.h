#pragma once

#include <string_view>
#include <iostream>

namespace sp_cli
{
    enum class SP_INSTRUCTIONS {
        NO_INST,
        LDA = 1,
        STA,
        XAB,
        CLA,
        PUSH = 6,
        POP,
        INC,
        DEC,
        MOV,
        AND,
        NOT,
        OR,
        XOR,
        ROL,
        ROR,
        SHL,
        SHR,
        ADD = 20,
        SUB,
        MUL,
        DIV,
        CLN,
        CLC,
        STC,
        CMC,
        LOOP = 29,
        JMP,
        JEQ,
        CMP,
        JME,
        JMA,
        JC,
        JNC,
        JO,
        JNO,
        JNE,
        LDT,
        EAP,
        MSG,
        LDB = 50,
        STB,
        LDF = 55,
        STF,
        ADDF = 60,
        SUBF,
        MULF,
        DIVF,
        ITOF,
        FTOI,
        IN = 80,
        OUT,
        NOP = 90,
        HLT = 99

    };

    inline constexpr SP_INSTRUCTIONS getInstruction(std::string_view string) {
        if (string == "LDA") return SP_INSTRUCTIONS::LDA;
        if (string == "STA") return SP_INSTRUCTIONS::STA;
        if (string == "XAB") return SP_INSTRUCTIONS::XAB;
        if (string == "CLA") return SP_INSTRUCTIONS::CLA;
        if (string == "PUSH") return SP_INSTRUCTIONS::PUSH;
        if (string == "POP") return SP_INSTRUCTIONS::POP;
        if (string == "INC") return SP_INSTRUCTIONS::INC;
        if (string == "DEC") return SP_INSTRUCTIONS::DEC;
        if (string == "MOV") return SP_INSTRUCTIONS::MOV;
        if (string == "AND") return SP_INSTRUCTIONS::AND;
        if (string == "NOT") return SP_INSTRUCTIONS::NOT;
        if (string == "OR") return SP_INSTRUCTIONS::OR;
        if (string == "XOR") return SP_INSTRUCTIONS::XOR;
        if (string == "ROL") return SP_INSTRUCTIONS::ROL;
        if (string == "ROR") return SP_INSTRUCTIONS::ROR;
        if (string == "SHL") return SP_INSTRUCTIONS::SHL;
        if (string == "SHR") return SP_INSTRUCTIONS::SHR;
        if (string == "ADD") return SP_INSTRUCTIONS::ADD;
        if (string == "SUB") return SP_INSTRUCTIONS::SUB;
        if (string == "MUL") return SP_INSTRUCTIONS::MUL;
        if (string == "DIV") return SP_INSTRUCTIONS::DIV;
        if (string == "CLN") return SP_INSTRUCTIONS::CLN;
        if (string == "CLC") return SP_INSTRUCTIONS::CLC;
        if (string == "STC") return SP_INSTRUCTIONS::STC;
        if (string == "CMC") return SP_INSTRUCTIONS::CMC;
        if (string == "LOOP") return SP_INSTRUCTIONS::LOOP;
        if (string == "JMP") return SP_INSTRUCTIONS::JMP;
        if (string == "JEQ") return SP_INSTRUCTIONS::JEQ;
        if (string == "CMP") return SP_INSTRUCTIONS::CMP;
        if (string == "JME") return SP_INSTRUCTIONS::JME;
        if (string == "JMA") return SP_INSTRUCTIONS::JMA;
        if (string == "JC") return SP_INSTRUCTIONS::JC;
        if (string == "JNC") return SP_INSTRUCTIONS::JNC;
        if (string == "JO") return SP_INSTRUCTIONS::JO;
        if (string == "JNO") return SP_INSTRUCTIONS::JNO;
        if (string == "JNE") return SP_INSTRUCTIONS::JNE;
        if (string == "LDT") return SP_INSTRUCTIONS::LDT;
        if (string == "EAP") return SP_INSTRUCTIONS::EAP;
        if (string == "MSG") return SP_INSTRUCTIONS::MSG;
        if (string == "LDB") return SP_INSTRUCTIONS::LDB;
        if (string == "STB") return SP_INSTRUCTIONS::STB;
        if (string == "LDF") return SP_INSTRUCTIONS::LDF;
        if (string == "STF") return SP_INSTRUCTIONS::STF;
        if (string == "ADDF") return SP_INSTRUCTIONS::ADDF;
        if (string == "SUBF") return SP_INSTRUCTIONS::SUBF;
        if (string == "MULF") return SP_INSTRUCTIONS::MULF;
        if (string == "DIVF") return SP_INSTRUCTIONS::DIVF;
        if (string == "ITOF") return SP_INSTRUCTIONS::ITOF;
        if (string == "FTOI") return SP_INSTRUCTIONS::FTOI;
        if (string == "IN") return SP_INSTRUCTIONS::IN;
        if (string == "OUT") return SP_INSTRUCTIONS::OUT;
        if (string == "NOP") return SP_INSTRUCTIONS::NOP;
        if (string == "HLT") return SP_INSTRUCTIONS::HLT;
        return SP_INSTRUCTIONS::NO_INST;
    }

    inline constexpr std::string_view getInstructionName(SP_INSTRUCTIONS inst) {
        switch (inst) {
            case SP_INSTRUCTIONS::LDA: return "LDA";
            case SP_INSTRUCTIONS::STA: return "STA";
            case SP_INSTRUCTIONS::XAB: return "XAB";
            case SP_INSTRUCTIONS::CLA: return "CLA";
            case SP_INSTRUCTIONS::PUSH: return "PUSH";
            case SP_INSTRUCTIONS::POP: return "POP";
            case SP_INSTRUCTIONS::INC: return "INC";
            case SP_INSTRUCTIONS::DEC: return "DEC";
            case SP_INSTRUCTIONS::MOV: return "MOV";
            case SP_INSTRUCTIONS::AND: return "AND";
            case SP_INSTRUCTIONS::NOT: return "NOT";
            case SP_INSTRUCTIONS::OR: return "OR";
            case SP_INSTRUCTIONS::XOR: return "XOR";
            case SP_INSTRUCTIONS::ROL: return "ROL";
            case SP_INSTRUCTIONS::ROR: return "ROR";
            case SP_INSTRUCTIONS::SHL: return "SHL";
            case SP_INSTRUCTIONS::SHR: return "SHR";
            case SP_INSTRUCTIONS::ADD: return "ADD";
            case SP_INSTRUCTIONS::SUB: return "SUB";
            case SP_INSTRUCTIONS::MUL: return "MUL";
            case SP_INSTRUCTIONS::DIV: return "DIV";
            case SP_INSTRUCTIONS::CLN: return "CLN";
            case SP_INSTRUCTIONS::CLC: return "CLC";
            case SP_INSTRUCTIONS::STC: return "STC";
            case SP_INSTRUCTIONS::CMC: return "CMC";
            case SP_INSTRUCTIONS::LOOP: return "LOOP";
            case SP_INSTRUCTIONS::JMP: return "JMP";
            case SP_INSTRUCTIONS::JEQ: return "JEQ";
            case SP_INSTRUCTIONS::CMP: return "CMP";
            case SP_INSTRUCTIONS::JME: return "JME";
            case SP_INSTRUCTIONS::JMA: return "JMA";
            case SP_INSTRUCTIONS::JC: return "JC";
            case SP_INSTRUCTIONS::JNC: return "JNC";
            case SP_INSTRUCTIONS::JO: return "JO";
            case SP_INSTRUCTIONS::JNO: return "JNO";
            case SP_INSTRUCTIONS::JNE: return "JNE";
            case SP_INSTRUCTIONS::LDT: return "LDT";
            case SP_INSTRUCTIONS::EAP: return "EAP";
            case SP_INSTRUCTIONS::MSG: return "MSG";
            case SP_INSTRUCTIONS::LDB: return "LDB";
            case SP_INSTRUCTIONS::STB: return "STB";
            case SP_INSTRUCTIONS::LDF: return "LDF";
            case SP_INSTRUCTIONS::STF: return "STF";
            case SP_INSTRUCTIONS::ADDF: return "ADDF";
            case SP_INSTRUCTIONS::SUBF: return "SUBF";
            case SP_INSTRUCTIONS::MULF: return "MULF";
            case SP_INSTRUCTIONS::DIVF: return "DIVF";
            case SP_INSTRUCTIONS::ITOF: return "ITOF";
            case SP_INSTRUCTIONS::FTOI: return "FTOI";
            case SP_INSTRUCTIONS::IN: return "IN";
            case SP_INSTRUCTIONS::OUT: return "OUT";
            case SP_INSTRUCTIONS::NOP: return "NOP";
            case SP_INSTRUCTIONS::HLT: return "HLT";
            case SP_INSTRUCTIONS::NO_INST: return "NO_INST";
        }
        return "UNKNOWN";
    }

    inline std::ostream& operator<<(std::ostream& out, SP_INSTRUCTIONS inst) {
        return out << getInstructionName(inst);
    }

    struct Instruction {
        SP_INSTRUCTIONS opcode {};
        std::string_view left_operand {};
        std::string_view right_operand {};

        void print() noexcept {
            std::cout << "opcode: " << opcode << '\n'
                      << "left operand: " << left_operand << '\n'
                      << "right operand: " << right_operand << '\n';
        }
    };

} // namespace sp_cli
