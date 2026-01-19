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
    class AddressRegisters {
        private:
        std::array<std::uint16_t, TOTAL_AR> registerArray {};

        [[nodiscard]] constexpr std::uint16_t mask(std::uint16_t registerContent) noexcept {
            return registerContent & MAX_ADDRESS;
        }

        [[nodiscard]] constexpr std::uint16_t mask(std::int16_t registerContent) noexcept {
            return registerContent & MAX_ADDRESS;
        }

        public:

        constexpr AddressRegisters() noexcept = default;

        [[nodiscard]] constexpr std::uint16_t getUnsignedReg(ARKey key) const noexcept {
            return registerArray[toIdx(key)];
        }

        [[nodiscard]] constexpr std::int16_t getSignedReg(ARKey key) const noexcept {
            return static_cast<std::int16_t>(registerArray[toIdx(key)]);
        }

        constexpr void setReg(ARKey key, std::uint16_t value) noexcept {
            registerArray[toIdx(key)] = mask(value);
        }

        constexpr void setReg(ARKey key, std::int16_t value) noexcept {
            registerArray[toIdx(key)] = mask(value);
        }

        void printRegs(std::ostream& out = std::cout) const;
    };
} // namespace sp_cli