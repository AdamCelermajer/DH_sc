// glitch::io::IReadFile: the read-file interface. The original leaves vtable slots
// +0x0c .. +0x2c abstract; the defaults below are the port's stand-ins (no data, position
// 0, empty names, no clone) so readers that are not reconstructed still link.
// CReadFile and the archive readers override them.
#pragma once

#include <cstdint>
#include <cstdio>
#include <string>

#include "glitch-core-io/deps/glitch_core.hpp"

namespace glitch::io {

class IReadFile;

// Completion callback of the asynchronous reads: void (*)(int, int, IReadFile*, void*).
using ReadAsyncCallBack = void (*)(int, int, IReadFile*, void*);

class IReadFile : public glitch::IReferenceCounted {
public:
    // +0x0c
    virtual void read(void* buffer, unsigned int count) {
        (void)buffer;
        (void)count;
    }
    // +0x10: reads at the current position (the original forwards to the +0x14 overload)
    virtual void readAsync(void* buffer, unsigned int count, ReadAsyncCallBack callBack, void* context);
    // +0x14
    virtual bool readAsync(void* buffer, unsigned int count, long position, ReadAsyncCallBack callBack,
                           void* context) {
        (void)buffer;
        (void)count;
        (void)position;
        (void)callBack;
        (void)context;
        return false;
    }
    // +0x18
    virtual bool seek(long offset, bool relative) {
        (void)offset;
        (void)relative;
        return false;
    }
    // +0x1c: a new file over the same data; null in the base (not cloned)
    virtual IReadFile* clone() const { return nullptr; }
    // +0x20
    virtual long getSize() const { return 0; }
    // +0x24
    virtual long getPos() const { return 0; }
    // +0x28
    virtual const std::string& getFileName() const {
        static const std::string none;
        return none;
    }
    // +0x2c
    virtual const std::string& getFullPath() const {
        static const std::string none;
        return none;
    }
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
    CReadFile* clone() const override;                // 0x570808
    bool isOpen() const;                              // 0x5703e4
    long getSize() const override;                    // 0x57044c
    const std::string& getFileName() const override;  // 0x570454
    const std::string& getFullPath() const override;  // 0x57045c
    bool seek(long offset, bool relative) override;   // 0x570488
    long getPos() const override;                     // 0x5705a8
    void read(void* buffer, unsigned int count) override;  // 0x5705b4
    void readAsync(void* buffer, unsigned int count, ReadAsyncCallBack callBack,
                   void* context) override;           // 0x5703f4: forwards with getPos()
    bool readAsync(void* buffer, unsigned int count, long position, ReadAsyncCallBack callBack,
                   void* context) override;           // 0x570444: returns true, no read
    void openFile();                                  // 0x5704cc

    CReadFile(const CReadFile&) = delete;
    CReadFile& operator=(const CReadFile&) = delete;

private:
    std::FILE* file = nullptr;   // +0x0c
    long size = 0;               // +0x10
    std::string fileName;        // +0x14
    bool obfuscated = false;     // +0x2c
};

// 0x5707b4 _ZN6glitch2io14createReadFileEPKc: opens the file; null (and released) when it
// is not open.
IReadFile* createReadFile(const char* name);

}  // namespace glitch::io
