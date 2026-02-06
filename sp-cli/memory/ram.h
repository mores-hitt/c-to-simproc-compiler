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
        void readCode(const std::string& file);

        public:
        explicit Memory(const std::string& code);
        void print(std::size_t begin = 0, std::size_t size = MAX_ADDRESS) const;
        [[nodiscard]] std::string_view get(std::size_t address) const;
        void set(std::size_t address, const std::string_view& content);
    };
} // namespace sp_cli