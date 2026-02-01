#include <gtest/gtest.h>
#include "memory/ram.h"
#include "cpu/cpu.h"
#include "const/const.h"

#include <string>
#include <cstdint>
#include <cstddef>
#include <stdexcept>

class MediumNormalTest : public testing::Test {
    protected:
    std::string code = R"(LDF FA0
ADDF FA0
STF FC0
JEQ 30
INC FF0
JME 40
INC FF0
JMA 50
INC FF0
JC 60
INC FF0
JNC 70
INC FF0
JO 80
INC FF0
JNO 90
INC FF0
JNE 100
HLT

#30
MOV BX,FF0; escritura para JEQ
MOV AX,FB0
STB FF1
JMP 004

#40
MOV BX,FF0; escritura para JME
MOV AX,FB0
STB FF1
JMP 006

#50
MOV BX,FF0; escritura para JMA
MOV AX,FB0
STB FF1
JMP 008

#60
MOV BX,FF0; escritura para JC
MOV AX,FB0
STB FF1
JMP 00A

#70
MOV BX,FF0; escritura para JNC
MOV AX,FB0
STB FF1
JMP 00C

#80
MOV BX,FF0; escritura para JO
MOV AX,FB0
STB FF1
JMP 00E

#90
MOV BX,FF0; escritura para JNO
MOV AX,FB0
STB FF1
JMP 010

#100
MOV BX,FF0; escritura para JNE
MOV AX,FB0
STB FF1
JMP 012

#FA0
0100000010100100; valores a operar
0001010001111011;

#FB0
1; valor 1. para indicar que salto ocurrio

#FC0
0; valor para escribir resultado de operacion
0;
#FF0
0; valor pa escribir
0; en FF1 comienza la escitura. JEQ
0; JME
0; JMA
0; JC
0; JNC
0; JO
0; JNO
0; JNE)";
    sp_cli::Memory ram;
    sp_cli::CPU cpu;

    MediumNormalTest(): ram(code), cpu(code) {}
};

TEST_F(MediumNormalTest, InitialState) {
    EXPECT_EQ(ram.get(0xFA0), "0100000010100100") << "Incorrect value at FF0";
    EXPECT_EQ(ram.get(0xFA1), "0001010001111011") << "Incorrect value at FA0";
    EXPECT_EQ(ram.get(0x103), "JMP 012") << "Incorrect value at 103";
    EXPECT_EQ(ram.get(0x093), "JMP 010") << "Incorrect value at 093";
    EXPECT_EQ(ram.get(0x083), "JMP 00E") << "Incorrect value at 083";
    EXPECT_EQ(ram.get(0x073), "JMP 00C") << "Incorrect value at 073";
    EXPECT_EQ(ram.get(0x063), "JMP 00A") << "Incorrect value at 063";
}

TEST_F(MediumNormalTest, RunCode) {
    cpu.run();
    sp_cli::CPUState state {cpu.getState()};

    EXPECT_EQ(state.memory.get(0xFC0), "0100000100100100") << "ADDF does not provide the correct result";
    EXPECT_EQ(state.memory.get(0xFC1), "0001010001111011") << "ADDF does not provide the correct result";

    EXPECT_EQ(state.CF.getFlag(sp_cli::Flags::Z), false) << "Z flag is set, and it should not";
    EXPECT_EQ(state.CF.getFlag(sp_cli::Flags::N), false) << "N flag is set, and it should not";
    EXPECT_EQ(state.CF.getFlag(sp_cli::Flags::C), false) << "C flag is set, and it should not";
    EXPECT_EQ(state.CF.getFlag(sp_cli::Flags::O), false) << "O flag is set, and it should not";

    EXPECT_EQ(state.memory.get(0xFF0), "0000000000000111") << "Iterator is not 111";
    EXPECT_EQ(state.memory.get(0xFF1), "0") << "JEQ jumped, but it should not have.";
    EXPECT_EQ(state.memory.get(0xFF2), "0") << "JME jumped, but it should not have";
    EXPECT_EQ(state.memory.get(0xFF3), "0000000000000001") << "JMA did not jump, but it should have";
    EXPECT_EQ(state.memory.get(0xFF4), "0") << "JC jumped, but it should not have";
    EXPECT_EQ(state.memory.get(0xFF5), "0000000000000001") << "JNC did not jump, but it should have";
    EXPECT_EQ(state.memory.get(0xFF6), "0") << "JO jumped, but it should not have";
    EXPECT_EQ(state.memory.get(0xFF7), "0000000000000001") << "JNO did not jump, but it should have";
    EXPECT_EQ(state.memory.get(0xFF8), "0000000000000001") << "JNE did not jump, but it should have";

}