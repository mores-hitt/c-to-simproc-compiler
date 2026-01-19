#pragma once

#include "const/const.h"

#include <cstdint>
#include <cstddef>
#include <string_view>
#include <array>
#include <iostream>

namespace sp_cli
{
    enum class ARKey : size_t {
        PC,
        SP,
        BP,
        MAR,
    };

    constexpr std::string_view getARKeyName(ARKey key) {
        switch (key)
        {
        case ARKey::PC: return "PC";
        case ARKey::SP: return "SP";
        case ARKey::BP: return "BP";
        case ARKey::MAR: return "MAR";
        }
        return "UNKNOWN";
    }

    inline std::ostream& operator<<(std::ostream& out, ARKey key) {
        return out << getARKeyName(key);
    }

    constexpr size_t toIdx(ARKey key) noexcept {
        return static_cast<size_t>(key);
    }

    enum class GPRKey : size_t {
        AX,
        BX,
        CX,
    };

    constexpr std::string_view getGPRKeyName(GPRKey key) {
        switch (key)
        {
        case GPRKey::AX: return "AX";
        case GPRKey::BX: return "BX";
        case GPRKey::CX: return "CX";
        }
        return "UNKNOWN";
    }

    inline std::ostream& operator<<(std::ostream& out, GPRKey key) {
        return out << getGPRKeyName(key);
    }

    constexpr size_t toIdx(GPRKey key) noexcept {
        return static_cast<size_t>(key);
    }

} // namespace sp_cli
