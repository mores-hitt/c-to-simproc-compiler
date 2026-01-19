#pragma once

#include "const/const.h"

#include <array>
#include <cstdint>
#include <cstddef>
#include <iostream>
#include <string>

namespace sp_cli
{
    class Memory {
        private:
        std::array<std::string, MAX_ADDRESS> memoryArray {};
        void readCode(std::string& file);

        public:
        Memory(std::string& code);
        
        void print(int begin = 0, int size = static_cast<int>(MAX_ADDRESS));

        /*

        void printMemUntil(int end);

        void printMemFrom(int begin);
        */

    };
} // namespace sp_cli
