// glitch::io::IReadFile: the read-file interface. Only the default
// implementations are reconstructed here; CReadFile and the archive readers
// override them.
#pragma once

#include <cstdint>
#include <cstdio>
#include <string>

#include "glitch-core-io/deps/glitch_core.hpp"

namespace glitch::io {

class IReadFile : public glitch::IReferenceCounted {
public:
    // Return type inferred: the original returns a 32-bit pointer (0 here).
    virtual void* getBuffer(long* size) {  // 0x56eb30
        (void)size;
        return nullptr;
    }
    virtual void* getExternalBuffer(const char* name, long* size) {  // 0x56eb38
        (void)name;
        (void)size;
        return nullptr;
    }
    virtual bool isAllInMemory() const { return false; }  // 0x56eb40
};

// glitch::io::CReadFile. The original keeps a shared CFile handle; here the
// reader owns the FILE* directly (see NOTES.md). Layout: +0x0c handle,
// +0x10 size, +0x14 name, +0x2c obfuscation flag.
class CReadFile : public IReadFile {
public:
    CReadFile(const char* name, bool obfuscated);     // 0x570734, 0x570874
    ~CReadFile() override;                            // 0x570660, 0x5706d8
    CReadFile* clone() const;                         // 0x570808
    bool isOpen() const;                              // 0x5703e4
    long getSize() const;                             // 0x57044c
    const std::string& getFileName() const;           // 0x570454
    const std::string& getFullPath() const;           // 0x57045c
    bool seek(long offset, bool relative);            // 0x570488
    long getPos() const;                              // 0x5705a8
    void read(void* buffer, unsigned int count);      // 0x5705b4
    void openFile();                                  // 0x5704cc

    CReadFile(const CReadFile&) = delete;
    CReadFile& operator=(const CReadFile&) = delete;

private:
    std::FILE* file = nullptr;   // +0x0c
    long size = 0;               // +0x10
    std::string fileName;        // +0x14
    bool obfuscated = false;     // +0x2c
};

}  // namespace glitch::io
