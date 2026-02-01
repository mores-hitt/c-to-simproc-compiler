#include <gtest/gtest.h>
#include "register/address.h"
#include "const/const.h"

#include <string>
#include <cstdint>
#include <cstddef>

class AddressRegisterTest : public testing::Test {
    protected:
    sp_cli::AddressRegisters AR;
};

TEST_F(AddressRegisterTest, InitialState){
    EXPECT_EQ(AR.getUnsignedReg(sp_cli::ARKey::BP), 0);
    EXPECT_EQ(AR.getUnsignedReg(sp_cli::ARKey::SP), 0);
    EXPECT_EQ(AR.getUnsignedReg(sp_cli::ARKey::PC), 0);
    EXPECT_EQ(AR.getUnsignedReg(sp_cli::ARKey::MAR), 0);
}

TEST_F(AddressRegisterTest, WriteRegister) {

}