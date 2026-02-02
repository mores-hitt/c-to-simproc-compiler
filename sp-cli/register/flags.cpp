#include "register/flags.h"

#include <iostream>

namespace sp_cli
{
    void ControlFlags::print(std::ostream& out) const {
        out << Flags::Z << ": " << getFlag(Flags::Z) << '\n'
            << Flags::N << ": " << getFlag(Flags::N) << '\n'
            << Flags::C << ": " << getFlag(Flags::C) << '\n'
            << Flags::O << ": " << getFlag(Flags::O) << '\n';
    }
} // namespace sp_cli