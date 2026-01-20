#include "general_purpose.h"
#include <bitset>
#include <iomanip>

namespace sp_cli
{
    void GeneralPurposeRegisters::print(std::ostream& out) const {
        out << GPRKey::AX << ": "
            << "Dec: " << std::dec << registerArray[toIdx(GPRKey::AX)]
            << " Hex: 0x" << std::hex << std::setw(4) << std::setfill('0') 
            << registerArray[toIdx(GPRKey::AX)]
            << " Bin: 0b" << std::bitset<16>(registerArray[toIdx(GPRKey::AX)]) << "\n";
        
        out << GPRKey::BX << ": "
            << "Dec: " << std::dec << registerArray[toIdx(GPRKey::BX)]
            << " Hex: 0x" << std::hex << std::setw(4) << std::setfill('0') 
            << registerArray[toIdx(GPRKey::BX)]
            << " Bin: 0b" << std::bitset<16>(registerArray[toIdx(GPRKey::BX)]) << "\n";
        
        out << GPRKey::CX << ": " 
            << "Dec: " << std::dec << registerArray[toIdx(GPRKey::CX)]
            << " Hex: 0x" << std::hex << std::setw(4) << std::setfill('0') 
            << registerArray[toIdx(GPRKey::CX)]
            << " Bin: 0b" << std::bitset<16>(registerArray[toIdx(GPRKey::CX)]) << "\n";
    }
} // namespace sp_cli