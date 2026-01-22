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
        void toggleFlag(Flags flag);
        void clearFlag(Flags flag);
        void clear();
        void print(std::ostream& out = std::cout);
    };
} // namespace sp_cli
