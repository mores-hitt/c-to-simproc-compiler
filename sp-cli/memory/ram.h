#pragma once

#include "const/const.h"

#include <array>
#include <cstdint>
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>

namespace sp_cli
{
    class Memory {
        private:
        std::array<std::string, MAX_ADDRESS + 1> memoryArray {};
        void readCode(std::string& file);

        public:
        Memory(std::string& code);
        void print(int begin = 0, int size = static_cast<int>(MAX_ADDRESS));
        std::string_view get(int address) const;
        void set(int address, std::string& content);

    };
} // namespace sp_cli
