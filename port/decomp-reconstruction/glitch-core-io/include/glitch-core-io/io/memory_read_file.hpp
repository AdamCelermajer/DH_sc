// glitch::io::CMemoryReadFile: a read file over a block of memory. The block is
// owned by the file (shared between clones) only when the constructor takes ownership.
// Only the constructors, set() and clone() are reconstructed here.
#pragma once

#include <memory>
#include <string>

#include "glitch-core-io/io/read_file.hpp"

namespace glitch::io {

class CMemoryReadFile : public IReadFile {
public:
    CMemoryReadFile();                                                                      // 0x56ed1c, 0x56ed8c
    CMemoryReadFile(void* data, long length, const char* fileName, bool takeOwnership);     // 0x56f2ec, 0x56f3b8

    void set(void* data, long length, const char* fileName);  // 0x56edfc: resets the read position
    CMemoryReadFile* clone() const;                           // 0x56f150: a new file sharing the block

    // +0x0c: block start (void*)
    void* buffer = nullptr;
    // +0x10: owner of the block; the default deleter (delete) matches the original shared_ptr<char>
    std::shared_ptr<char> owned;
    // +0x18: block length
    long size = 0;
    // +0x1c: read position
    long position = 0;
    // +0x20: file name
    std::string name;
};

// 0x56f370 _ZN6glitch2io20createMemoryReadFileEPvlPKcb: a memory reader over a block.
CMemoryReadFile* createMemoryReadFile(void* data, long length, const char* fileName, bool takeOwnership);

}  // namespace glitch::io
