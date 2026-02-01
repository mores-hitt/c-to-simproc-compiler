#include <gtest/gtest.h>
#include "memory/ram.h"
#include "const/const.h"

#include <string>
#include <cstdint>
#include <cstddef>
#include <stdexcept>

class MemoryTest : public testing::Test {
    protected:
    std::string code = R"(NOP
NOP
NOP
#10
1111
1010
#FFD
1111
1010
1100)";
    sp_cli::Memory ram;

    MemoryTest(): ram(code) {}
};

TEST_F(MemoryTest, ReadMemory){
    EXPECT_EQ(ram.get(0x0), "NOP") << "address 0x0 does not contain NOP";
    EXPECT_EQ(ram.get(0x1), "NOP") << "address 0x1 does not contain NOP";
    EXPECT_EQ(ram.get(0x2), "NOP") << "address 0x2 does not contain NOP";
    EXPECT_EQ(ram.get(0x3), "") << "address 0x3 does not contain '' ";
    EXPECT_EQ(ram.get(0x10), "1111") << "address 0x10 does not contain 1111";
    EXPECT_EQ(ram.get(0x11), "1010") << "address 0x11 does not contain 1010";
    EXPECT_EQ(ram.get(0x12), "") << "address 0x12 does not contain '' ";
    EXPECT_EQ(ram.get(0x13), "") << "address 0x13 does not contain '' ";
    EXPECT_EQ(ram.get(0x14), "") << "address 0x0 does not contain '' ";
    EXPECT_EQ(ram.get(0xFFD), "1111") << "address 0xFFD does not contain 1111";
    EXPECT_EQ(ram.get(0xFFE), "1010") << "address 0xFFE does not contain 1010";
    EXPECT_EQ(ram.get(sp_cli::MAX_ADDRESS), "1100") << "address 0xFFF does not contain 1100";
}

TEST_F(MemoryTest, InvalidReading) {
    ASSERT_THROW(ram.get(-1), std::out_of_range);
    ASSERT_THROW(ram.get(sp_cli::MAX_ADDRESS + 1), std::out_of_range);
}

TEST_F(MemoryTest, WriteMemory) {
    std::string content = "TEST";

    ram.set(0, content);
    ram.set(sp_cli::MAX_ADDRESS, content);
    ram.set(0x123, content);
    ram.set(123, content);

    EXPECT_NE(ram.get(0), "NOP") << "address 0x0 still contains NOP after write operation";
    EXPECT_EQ(ram.get(0), content) << "address 0x0 does not contain TEST";
    EXPECT_NE(ram.get(sp_cli::MAX_ADDRESS), "1100") << "address 0xFFF still contains 1100 after write operation";
    EXPECT_EQ(ram.get(sp_cli::MAX_ADDRESS), content) << "address 0xFFF does not contain TEST";
    EXPECT_NE(ram.get(0x123), "") << "address 0x123 still contains '' after write operation";
    EXPECT_EQ(ram.get(0x123), content) << "address 0x123 does not contain TEST";
    EXPECT_NE(ram.get(123), "") << "address 123 (0x07B) still contains '' after write operation";
    EXPECT_EQ(ram.get(123), content) << "address 123 (0x07B) does not contain TEST";
}

TEST_F(MemoryTest, InvalidWriting) {
    std::string stringContent {"TEST"};
    std::string bigString {"ASDFGHJKLÑLKJHGFDSASDFGHJKLKJHGFDSSDFGHJKLKJHGFDSSDFGHJKLKJHGFDSDFGHJKL\
        ASDFGHJKLÑLKJHGFDSASDFGHJKLKJHGFDSSDFGHJKLKJHGFDSSDFGHJKLKJHGFDSDFGHJKL\
        ASDFGHJKLÑLKJHGFDSASDFGHJKLKJHGFDSSDFGHJKLKJHGFDSSDFGHJKLKJHGFDSDFGHJKL\
        ASDFGHJKLÑLKJHGFDSASDFGHJKLKJHGFDSSDFGHJKLKJHGFDSSDFGHJKLKJHGFDSDFGHJKL\
        ASDFGHJKLÑLKJHGFDSASDFGHJKLKJHGFDSSDFGHJKLKJHGFDSSDFGHJKLKJHGFDSDFGHJKL\
        ASDFGHJKLÑLKJHGFDSASDFGHJKLKJHGFDSSDFGHJKLKJHGFDSSDFGHJKLKJHGFDSDFGHJKL\
        ASDFGHJKLÑLKJHGFDSASDFGHJKLKJHGFDSSDFGHJKLKJHGFDSSDFGHJKLKJHGFDSDFGHJKL\
        ASDFGHJKLÑLKJHGFDSASDFGHJKLKJHGFDSSDFGHJKLKJHGFDSSDFGHJKLKJHGFDSDFGHJKL\
        ASDFGHJKLÑLKJHGFDSASDFGHJKLKJHGFDSSDFGHJKLKJHGFDSSDFGHJKLKJHGFDSDFGHJKL\
        ASDFGHJKLÑLKJHGFDSASDFGHJKLKJHGFDSSDFGHJKLKJHGFDSSDFGHJKLKJHGFDSDFGHJKL\
        ASDFGHJKLÑLKJHGFDSASDFGHJKLKJHGFDSSDFGHJKLKJHGFDSSDFGHJKLKJHGFDSDFGHJKL\
        ASDFGHJKLÑLKJHGFDSASDFGHJKLKJHGFDSSDFGHJKLKJHGFDSSDFGHJKLKJHGFDSDFGHJKL\
        ASDFGHJKLÑLKJHGFDSASDFGHJKLKJHGFDSSDFGHJKLKJHGFDSSDFGHJKLKJHGFDSDFGHJKL\
        ASDFGHJKLÑLKJHGFDSASDFGHJKLKJHGFDSSDFGHJKLKJHGFDSSDFGHJKLKJHGFDSDFGHJKL\
        ASDFGHJKLÑLKJHGFDSASDFGHJKLKJHGFDSSDFGHJKLKJHGFDSSDFGHJKLKJHGFDSDFGHJKL\
        ASDFGHJKLÑLKJHGFDSASDFGHJKLKJHGFDSSDFGHJKLKJHGFDSSDFGHJKLKJHGFDSDFGHJKL\
        ASDFGHJKLÑLKJHGFDSASDFGHJKLKJHGFDSSDFGHJKLKJHGFDSSDFGHJKLKJHGFDSDFGHJKL"};

    EXPECT_THROW(ram.set(-1, stringContent), std::out_of_range);
    EXPECT_THROW(ram.set(-999999999, stringContent), std::out_of_range);
    EXPECT_THROW(ram.set(99999999, stringContent), std::out_of_range);
    EXPECT_THROW(ram.set(sp_cli::MAX_ADDRESS + 1, stringContent), std::out_of_range);

    EXPECT_THROW(ram.set(-1, bigString), std::invalid_argument);
    EXPECT_THROW(ram.set(-999999999, bigString), std::invalid_argument);
    EXPECT_THROW(ram.set(99999999, bigString), std::invalid_argument);
    EXPECT_THROW(ram.set(sp_cli::MAX_ADDRESS + 1, bigString), std::invalid_argument);

}


