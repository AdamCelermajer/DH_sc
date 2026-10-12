// Stand-in for the glitch IFileStream interface. Only the destructor is used here:
// the original closes a stream through vtable slot +0x4, which is the deleting
// destructor (see the IFileStream vtable).
#pragma once

class IFileStream {
public:
    virtual ~IFileStream() = default;
};
