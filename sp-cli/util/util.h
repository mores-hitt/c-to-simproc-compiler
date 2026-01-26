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

    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    [[nodiscard]] inline constexpr std::uint16_t mask12Bit(T value) noexcept {
        return static_cast<std::uint16_t>(value) & MAX_12BIT;
    }
    
    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    [[nodiscard]] inline constexpr std::uint16_t mask16BitU(T value) noexcept {
        return static_cast<std::uint16_t>(value) & MAX_16BIT;
    }

    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    [[nodiscard]] inline constexpr std::int16_t mask16BitI(T value) noexcept {
        return static_cast<std::int16_t>(static_cast<std::uint16_t>(value) & MAX_16BIT);
    }

    // Sign-extend 12-bit value to 16-bit signed
    [[nodiscard]] inline constexpr std::int16_t signExtend12To16(std::uint16_t value) noexcept {
        value &= MAX_12BIT;
        if (value & 0x0800) {  // Check bit 11 (sign bit)
            return static_cast<std::int16_t>(value | 0xF000);
        }
        return static_cast<std::int16_t>(value);
    }

    // Reinterpret unsigned as signed
    [[nodiscard]] inline constexpr std::int16_t asI16(std::uint16_t value) noexcept {
        return static_cast<std::int16_t>(value);
    }

    // Reinterpret signed as unsigned
    [[nodiscard]] inline constexpr std::uint16_t asU16(std::int16_t value) noexcept {
        return static_cast<std::uint16_t>(value);
    }

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
