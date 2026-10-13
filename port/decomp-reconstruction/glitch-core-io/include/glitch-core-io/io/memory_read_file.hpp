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

    // Accessors (batch e1-glitch-core-io-glitch_io_CMemoryReadFile-0).
    void* getBuffer(long* size) override;                     // 0x56eb6c: block; *size = read position
    bool isAllInMemory() const override;                      // 0x56eb80: always true
    bool isValid() const;                                     // 0x56eb8c: block present and size >= 0
    bool seek(long offset, bool relative) override;           // 0x56ec3c
    long getSize() const override;                            // 0x56ec78
    long getPos() const override;                             // 0x56ec80
    // 0x56ec88, 0x56ec90: the name. The original returns the character data of the name string
    // (ldr r0, [r0, #0x34]: the begin pointer of the string at +0x20); the port's slot returns the string.
    const std::string& getFileName() const override;  // 0x56ec88
    const std::string& getFullPath() const override;  // 0x56ec90
    // Not declared yet (see the batch ledger): read (0x56ecc4), readAsync (0x56ebac, 0x56ebe0). Their
    // bodies need the count returned by read; IReadFile::read is declared void in read_file.hpp.

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
