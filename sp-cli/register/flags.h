#pragma once

#include "register/enum.h"

#include <cstdint>
#include <iostream>

namespace sp_cli
{
    class ControlFlags {
        private:
        std::uint8_t flags {};

        public:
        constexpr void setFlag(Flags flag) noexcept {
            flags |= (1 << static_cast<int>(flag));
        }

        [[nodiscard]] constexpr bool getFlag(Flags flag) const noexcept {
            return (flags & (1 << static_cast<int>(flag))) != 0;
        }

        constexpr void toggleFlag(Flags flag) noexcept {
            flags ^= (1 << static_cast<int>(flag));
        }

        constexpr void clearFlag(Flags flag) noexcept {
            flags &= static_cast<std::uint8_t>(~(1 << static_cast<int>(flag)));
        }

        constexpr void clear() noexcept {
            flags = 0;
        }

        void print(std::ostream& out = std::cout) const;
    };
} // namespace sp_cli