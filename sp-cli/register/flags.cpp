#include "register/flags.h"

#include "iostream"
namespace sp_cli
{
    
    void ControlFlags::clear() {
        this->flags = this->flags & 0;
    }

    void ControlFlags::clearFlag(Flags flag){
        this->flags &= ~(1 << static_cast<int>(flag));
    }

    void ControlFlags::setFlag(Flags flag) {
        this->flags |= (1 << static_cast<int>(flag));
    }

    bool ControlFlags::getFlag(Flags flag) {
        return (this->flags & (1 << static_cast<int>(flag))) != 0;
    }

    void ControlFlags::print(std::ostream& out) {
        out << Flags::Z << ": " << getFlag(Flags::Z) << '\n'
                  << Flags::N << ": " << getFlag(Flags::N) << '\n'
                  << Flags::C << ": " << getFlag(Flags::C) << '\n'
                  << Flags::O << ": " << getFlag(Flags::O) << '\n';
    }

} // namespace sp_cli