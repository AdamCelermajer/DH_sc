// glitch::io::IReadFile: the read-file interface. Only the default
// implementations are reconstructed here; CReadFile and the archive readers
// override them.
#pragma once

#include <cstdint>

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

}  // namespace glitch::io
