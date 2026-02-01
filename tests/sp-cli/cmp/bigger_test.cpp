#include <gtest/gtest.h>
#include "memory/ram.h"
#include "cpu/cpu.h"
#include "const/const.h"

#include <string>
#include <cstdint>
#include <cstddef>
#include <stdexcept>

class BiggerComparasionTest : public testing::Test {
    protected:
    std::string code = R"(LDA FA0
CMP FA1
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
JMP 003

#40
MOV BX,FF0; escritura para JME
MOV AX,FB0
STB FF1
JMP 005

#50
MOV BX,FF0; escritura para JMA
MOV AX,FB0
STB FF1
JMP 007

#60
MOV BX,FF0; escritura para JC
MOV AX,FB0
STB FF1
JMP 009

#70
MOV BX,FF0; escritura para JNC
MOV AX,FB0
STB FF1
JMP 00B

#80
MOV BX,FF0; escritura para JO
MOV AX,FB0
STB FF1
JMP 00D

#90
MOV BX,FF0; escritura para JNO
MOV AX,FB0
STB FF1
JMP 00F

#100
MOV BX,FF0; escritura para JNE
MOV AX,FB0
STB FF1
JMP 011

#FA0
11; valores a operar
10;

#FB0
1; valor 1. para indicar que salto ocurrio

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

    BiggerComparasionTest(): ram(code), cpu(code) {}
};

TEST_F(BiggerComparasionTest, InitialState) {
    EXPECT_EQ(ram.get(0xFA0), "11") << "Incorrect value at FF0";
    EXPECT_EQ(ram.get(0xFA1), "10") << "Incorrect value at FA0";
    EXPECT_EQ(ram.get(0x103), "JMP 011") << "Incorrect value at 103";
    EXPECT_EQ(ram.get(0x093), "JMP 00F") << "Incorrect value at 093";
    EXPECT_EQ(ram.get(0x083), "JMP 00D") << "Incorrect value at 083";
    EXPECT_EQ(ram.get(0x073), "JMP 00B") << "Incorrect value at 073";
    EXPECT_EQ(ram.get(0x063), "JMP 009") << "Incorrect value at 063";
}

TEST_F(BiggerComparasionTest, RunCode) {
    cpu.run();
    sp_cli::CPUState state {cpu.getState()};

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