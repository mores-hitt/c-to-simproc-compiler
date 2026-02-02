#pragma once

#include "const/const.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string_view>

namespace sp_cli
{
    struct FloatParts {
        uint16_t msb;
        uint16_t lsb;
    };

    [[nodiscard]] inline float makeFloat(std::uint16_t lsb, std::uint16_t msb) {
        std::uint32_t comb {(static_cast<std::uint32_t>(msb) << 16) | static_cast<uint32_t>(lsb)};
        float result;
        std::memcpy(&result, &comb, sizeof(float));
        return result;
    }

    [[nodiscard]] inline FloatParts splitFloat(float number) {
        uint32_t floatBits;
        std::memcpy(&floatBits, &number, sizeof(uint32_t));
        uint16_t msb = static_cast<uint16_t>(floatBits >> 16);
        uint16_t lsb = static_cast<uint16_t>(floatBits & 0xFFFF);
        return {msb, lsb};
    }

} // namespace sp_cli
