// glitch::io::CLimitReadFile: a window [start, start + size) of another read file. The
// reader keeps its own clone of the wrapped file (released in the destructor). Layout
// from the original offsets (kept as comments; std::string stands in for the glitch strings):
//   +0x0c name passed to the constructor, +0x24 full name, +0x3c size, +0x40 start,
//   +0x44 end, +0x48 wrapped file, +0x4c current position.
// Only the constructors, the destructors, init and clone are reconstructed here; the other
// IReadFile slots keep the IReadFile defaults (they are not in the queue).
#pragma once

#include <string>

#include "glitch-core-io/io/read_file.hpp"

namespace glitch::io {

class CLimitReadFile : public IReadFile {
public:
    // 0x6b49c4 (C1), 0x6b4aa8 (C2): the four-argument form, which calls init with no second name.
    CLimitReadFile(IReadFile* file, long size, const char* name);
    // 0x6b4810 (C1), 0x6b4918 (C2): the five-argument form.
    CLimitReadFile(IReadFile* file, long size, const char* name, const char* name2);

    ~CLimitReadFile() override;  // 0x6b4bcc (D1), 0x6b4b54 (D2): drops the wrapped file

    // 0x6b4740: takes the window from the wrapped file's position and full path.
    void init(const char* name2);

    IReadFile* clone() const override;  // 0x6b48bc

    CLimitReadFile(const CLimitReadFile&) = delete;
    CLimitReadFile& operator=(const CLimitReadFile&) = delete;

    std::string name;      // +0x0c
    std::string fullName;  // +0x24
    long size = 0;         // +0x3c
    long start = 0;        // +0x40
    long end = 0;          // +0x44
    IReadFile* file = nullptr;  // +0x48
    long position = 0;     // +0x4c
};

// 0x6b4a70 _ZN6glitch2io19createLimitReadFileEPKcPNS0_9IReadFileEl: name, file, size.
IReadFile* createLimitReadFile(const char* name, IReadFile* file, long size);

}  // namespace glitch::io
