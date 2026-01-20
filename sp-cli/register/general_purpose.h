#pragma once

#include "const/const.h"
#include "enum.h"

#include <cstdint>
#include <cstddef>
#include <string_view>
#include <array>
#include <iostream>

namespace sp_cli
{
    class GeneralPurposeRegisters {
            private:
            std::array<std::uint16_t, TOTAL_GPR> registerArray {};
    
            public:
            constexpr GeneralPurposeRegisters() noexcept = default;
    
            [[nodiscard]] constexpr std::uint16_t getUnsignedReg(GPRKey key) const noexcept {
                return registerArray[toIdx(key)];
            }
    
            [[nodiscard]] constexpr std::int16_t getSignedReg(GPRKey key) const noexcept {
                return static_cast<std::int16_t>(registerArray[toIdx(key)]);
            }
    
            constexpr void setReg(GPRKey key, std::uint16_t value) noexcept {
                registerArray[toIdx(key)] = value;
            }
    
            constexpr void setReg(GPRKey key, std::int16_t value) noexcept {
                registerArray[toIdx(key)] = static_cast<std::uint16_t>(value);
            }
    
            void print(std::ostream& out = std::cout) const;
    
        };
    
} // namespace sp_cli

