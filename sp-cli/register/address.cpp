#include "address.h"
#include <bitset>
#include <iomanip>

namespace sp_cli
{
    void AddressRegisters::print(std::ostream& out) const {
        out << ARKey::PC << ": "
            << "Dec: " << std::dec << registerArray[toIdx(ARKey::PC)]
            << " Hex: 0x" << std::hex << std::setw(3) << std::setfill('0') 
            << registerArray[toIdx(ARKey::PC)]
            << " Bin: 0b" << std::bitset<12>(registerArray[toIdx(ARKey::PC)]) << "\n";
        
        out << ARKey::SP << ": "
            << "Dec: " << std::dec << registerArray[toIdx(ARKey::SP)]
            << " Hex: 0x" << std::hex << std::setw(3) << std::setfill('0') 
            << registerArray[toIdx(ARKey::SP)]
            << " Bin: 0b" << std::bitset<12>(registerArray[toIdx(ARKey::SP)]) << "\n";
        
        out << ARKey::BP << ": " 
            << "Dec: " << std::dec << registerArray[toIdx(ARKey::BP)]
            << " Hex: 0x" << std::hex << std::setw(3) << std::setfill('0') 
            << registerArray[toIdx(ARKey::BP)]
            << " Bin: 0b" << std::bitset<12>(registerArray[toIdx(ARKey::BP)]) << "\n";

        out << ARKey::MAR << ": "
            << "Dec: " << std::dec << registerArray[toIdx(ARKey::MAR)]
            << " Hex: 0x" << std::hex << std::setw(3) << std::setfill('0') 
            << registerArray[toIdx(ARKey::MAR)]
            << " Bin: 0b" << std::bitset<12>(registerArray[toIdx(ARKey::MAR)]) << "\n";
    }
} // namespace sp_cli