# include "memory/ram.h"

#include <sstream>
#include <charconv>
#include <iostream>
#include <bitset>
#include <iomanip>
#include <stdexcept>

namespace sp_cli
{
    void Memory::readCode(std::string& file){
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

            auto it = line.cbegin();
            auto end = line.cend();
            std::string buffer {};

            if (*it == '#') { // is a #lineNum 
                it++;
                auto start = it;
                while (it != end) {
                    it++;
                }
                buffer.append(start, it);
                std::from_chars(buffer.data(), buffer.data() + buffer.size(), address, 16);
                continue;
                //check if the number is between 0 and 0xFFF
            } else {

                size_t semicoloPos {line.find_first_of(';')};

                if (semicoloPos == std::string::npos) {
                    this->memoryArray[address] = line;
                } else {
                    this->memoryArray[address] = line.substr(0, semicoloPos);
                }
            }
            address++;
        }
    }

    Memory::Memory(std::string& code) {
        readCode(code);
    }

    void Memory::print(int begin, int size) {
        auto startIt {this->memoryArray.begin() + begin };

        if ( begin + size > static_cast<int>(MAX_ADDRESS)){
            std::cout << "Couldn print mem. Out of bound address. " << begin + size
            << " is bigger than " << MAX_ADDRESS << '\n';
        }

        auto endIt {this->memoryArray.begin() + begin + size };
        size_t lineNumber {static_cast<size_t>(begin)};

        for (auto it = startIt; it <= endIt; it++, lineNumber++) {
            std::cout << "Address: " << std::dec << lineNumber;
            std::cout << "(0x" << std::hex << std::setw(3) << std::setfill('0') << lineNumber;
            std::cout << std::dec << ") Content: " << *it << '\n';
        }
    }

    std::string_view Memory::get(int address) {
        return this->memoryArray.at(static_cast<size_t>(address));
    }

    void Memory::set(int address, std::string& content) {
        if (content.size() > 1024) {
            throw std::invalid_argument("memory set content is bigger than 1024");
        }
        if (address < 0 || address > MAX_ADDRESS) {
            throw std::out_of_range("Invalid index for ram");
        }
        this->memoryArray[static_cast<size_t>(address)] = content;
    }
} // namespace sp_cli
