// glitch::io::CWriteFile: plain file writer. Layout: +0x08 name (a 0x18-byte string: end +0x18,
// begin +0x1c, from the constructor), +0x20 handle, +0x24 size (see NOTES.md for the shared-handle
// replacement).
#pragma once

#include <cstddef>
#include <cstdio>
#include <string>

#include "glitch-core-io/deps/glitch_core.hpp"
#include "glitch-core-io/io/interfaces.hpp"

namespace glitch::io {

class CWriteFile : public IWriteFile {
public:
    CWriteFile(const char* name, bool append);        // 0x570b68, 0x570c40
    ~CWriteFile() override;                           // 0x570a90, 0x570b08
    const std::string& getFileName() const override;  // 0x5708f8
    const std::string& getFullPath() const override;  // 0x570900
    bool seek(long offset, bool relative) override;   // 0x57092c
    long getPos() const override;                     // 0x570950
    void openFile(bool append);                       // 0x57095c
    void flush() override;                            // 0x570a58
    std::size_t write(const void* data, unsigned int count) override;  // 0x570a6c
    bool isOpen() const { return file != nullptr; }

    CWriteFile(const CWriteFile&) = delete;
    CWriteFile& operator=(const CWriteFile&) = delete;

private:
    std::string fileName;                 // +0x08 (0x18 bytes, through +0x20)
    std::FILE* file = nullptr;            // +0x20
    long size = 0;                        // +0x24
};

// 0x570bf4 _ZN6glitch2io15createWriteFileEPKcb: opens the file for writing (or appending);
// null (and released) when it is not open.
IWriteFile* createWriteFile(const char* name, bool append);

}  // namespace glitch::io
