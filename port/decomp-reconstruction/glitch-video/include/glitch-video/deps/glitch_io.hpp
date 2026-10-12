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
    // Placeholders for the slots between read and getSize (vtable +0x10..+0x14).
    virtual void unidentifiedSlot04() {}
    virtual void unidentifiedSlot05() {}
    // vtable +0x18: seek(position, relativeMovement). Returns true on success.
    virtual bool seek(int32_t position, bool relativeMovement) { (void)position; (void)relativeMovement; return false; }
    virtual void unidentifiedSlot07() {}
    // vtable +0x20: total size in bytes.
    virtual uint32_t getSize() const { return 0; }
    // vtable +0x24: current position.
    virtual int32_t getPos() const { return 0; }
    // vtable +0x28: file name as a C string. The default is a placeholder; the real
    // return value comes from the file object.
    virtual const char* getFileNameText() const { return ""; }
};

}  // namespace glitch::io
