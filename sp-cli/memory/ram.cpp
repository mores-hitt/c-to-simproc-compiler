# include "memory/ram.h"

#include <sstream>
#include <charconv>
#include <iostream>
#include <bitset>
#include <iomanip>
#include <stdexcept>

namespace sp_cli
{
    void Memory::readCode(const std::string& file){
        // read a line
        // add it sequentially
        // spaces between instruction does not map into in between space in memory
        // ASSUMING THE STRING IS CLEAN AND CORRECT
        std::istringstream stream(file);
        std::string line {};
        size_t address {};

        while (std::getline(stream, line)) {

            if (line.empty()){
                continue;
            }

            if (line[0] == '#') { // is a #lineNum 
                std::string_view numView {line.data() + 1, line.size() - 1};
                std::from_chars(numView.data(), numView.data() + numView.size(), address, 16);
                continue;
            }

            size_t semicoloPos {line.find_first_of(';')};

            if (semicoloPos == std::string::npos) {
                this->memoryArray[address] = line;
            } else {
                this->memoryArray[address] = line.substr(0, semicoloPos);
            }
            address++;
        }
    }

    Memory::Memory(const std::string& code) {
        readCode(code);
    }

    void Memory::print(size_t begin, size_t size) const {
        if (begin + size > static_cast<int>(MAX_ADDRESS)){
            std::cout << "Couldnt print mem. Out of bound address. " << begin + size
                      << " is bigger than " << MAX_ADDRESS << '\n';
            return;
        }

        for (size_t addr = begin; addr < begin + size; ++addr) {
            std::cout << "Address: " << std::dec << addr
                      << " (0x" << std::hex << std::setw(3) << std::setfill('0') << addr
                      << std::dec << ") Content: " << memoryArray[addr] << '\n';
        }
    }

    std::string_view Memory::get(size_t address) const {
        return this->memoryArray.at(address);
    }

    void Memory::set(size_t address, const std::string_view& content) {
        if (content.size() > 1024) {
            throw std::invalid_argument("memory set content is bigger than 1024");
        }
        if (address > MAX_ADDRESS) {
            throw std::out_of_range("Invalid index for ram");
        }
        this->memoryArray[static_cast<size_t>(address)] = content;
    }
} // namespace sp_cli
