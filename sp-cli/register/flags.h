#pragma once

#include "register/enum.h"

#include <cstdint>

namespace sp_cli
{
    class ControlFlags {
        private:
        uint8_t flags {};

        public:
        void setFlag(Flags flag);
        bool getFlag(Flags flag);
        void clearFlag(Flags flag);
        void clear();
    };
} // namespace sp_cli
