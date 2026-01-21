#pragma once

#include <cstdint>

namespace sp_cli {
    inline constexpr std::uint16_t MAX_ADDRESS = 0x0FFF;
    inline constexpr std::uint16_t MAX_VALUE = 0xFFFF;
    inline constexpr std::uint16_t MAX_12BIT = 0x0FFF;
    inline constexpr std::uint16_t MAX_16BIT = 0xFFFF;
    inline constexpr int MEMORY_SIZE = 16;
    inline constexpr int TOTAL_AR = 4;
    inline constexpr int TOTAL_GPR = 3;
    inline constexpr int MEMORY_PRINT_WINDOW = 10;
    inline constexpr int STACK_ADDRESS = 0xF80;
} // namespace sp_cli