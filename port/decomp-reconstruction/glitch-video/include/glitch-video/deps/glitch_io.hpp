// Stand-in for glitch::io::IReadFile. In the original build read() sits at
// vtable +0x0c (slot 3); the slot before it is an unidentified base-class
// virtual, kept here as a placeholder so the slot order matches.
#pragma once

#include <cstdint>

namespace glitch::io {

class IReadFile {
public:
    virtual ~IReadFile() = default;
    virtual void unidentifiedBaseSlot() = 0;
    // Returns the number of bytes read.
    virtual int32_t read(void* buffer, uint32_t sizeToRead) = 0;
};

}  // namespace glitch::io
