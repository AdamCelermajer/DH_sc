// glitch::io::CWriteFile: plain file writer. Layout: +0x20 handle, +0x24 size,
// +0x1c name (see NOTES.md for the shared-handle replacement).
#pragma once

#include <cstddef>
#include <cstdio>
#include <string>

#include "glitch-core-io/deps/glitch_core.hpp"

namespace glitch::io {

class CWriteFile : public glitch::IReferenceCounted {
public:
    CWriteFile(const char* name, bool append);        // 0x570b68, 0x570c40
    ~CWriteFile() override;                           // 0x570a90, 0x570b08
    const std::string& getFileName() const;           // 0x5708f8
    const std::string& getFullPath() const;           // 0x570900
    bool seek(long offset, bool relative);            // 0x57092c
    long getPos() const;                              // 0x570950
    void openFile(bool append);                       // 0x57095c
    void flush();                                     // 0x570a58
    std::size_t write(const void* data, unsigned int count);  // 0x570a6c

    CWriteFile(const CWriteFile&) = delete;
    CWriteFile& operator=(const CWriteFile&) = delete;

private:
    std::string fileName;                 // +0x1c
    std::FILE* file = nullptr;            // +0x20
    long size = 0;                        // +0x24
};

}  // namespace glitch::io
