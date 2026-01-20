#include "register/flags.h"

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
        return this->flags & (1 << static_cast<int>(flag)) != 0;
    }

} // namespace sp_cli