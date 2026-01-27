#include <gtest/gtest.h>
#include "cpu/cpu.h"
#include "memory/ram.h"
#include "const/const.h"

#include <string>
#include <cstdint>
#include <cstddef>

class CpuTest : public testing::Test {
    protected:
    std::string code;
    sp_cli::CPU cpu;

    CpuTest(): code {"HLT"}, cpu(code) {}
};

TEST_F(CpuTest, InitialState){
    sp_cli::CPUState state {cpu.getState()};

    EXPECT_EQ(state.AR.getUnsignedReg(sp_cli::ARKey::PC), static_cast<uint16_t>(0)) << "PC is not equal to 0";
    EXPECT_EQ(state.AR.getUnsignedReg(sp_cli::ARKey::SP), static_cast<uint16_t>(sp_cli::STACK_ADDRESS)) << "SP is not equal to STACK_ADDRESS";
    EXPECT_EQ(state.AR.getUnsignedReg(sp_cli::ARKey::BP), static_cast<uint16_t>(sp_cli::STACK_ADDRESS)) << "BP is not equal to STACK_ADDRESS";
    EXPECT_EQ(state.AR.getUnsignedReg(sp_cli::ARKey::MAR), static_cast<uint16_t>(0)) << "MAR is not equal to 0";
    EXPECT_EQ(state.GPR.getUnsignedReg(sp_cli::GPRKey::AX), static_cast<uint16_t>(0)) << "AX is not equal to 0";
    EXPECT_EQ(state.GPR.getUnsignedReg(sp_cli::GPRKey::BX), static_cast<uint16_t>(0)) << "BX is not equal to 0";
    EXPECT_EQ(state.GPR.getUnsignedReg(sp_cli::GPRKey::CX), static_cast<uint16_t>(0)) << "CX is not equal to 0";
    EXPECT_EQ(state.terminate, false) << "CPU has terminated (terminate is true)";

}