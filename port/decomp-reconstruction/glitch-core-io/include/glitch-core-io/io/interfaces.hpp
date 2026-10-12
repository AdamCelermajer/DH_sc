// Interfaces of glitch::io that carry no behaviour of their own. Their destructors
// are empty (the original emits a single `ret` for the complete-object variant and a
// `delete` wrapper for the deleting variant), so they are declared with defaulted
// virtual destructors here.
#pragma once

#include <cstddef>
#include <string>

#include "glitch-core-io/deps/glitch_core.hpp"

namespace glitch::io {

class IAttributes {
public:
    virtual ~IAttributes() = default;
};

class IAttributesReader {
public:
    virtual ~IAttributesReader() = default;
};

class IAttributesWriter {
public:
    virtual ~IAttributesWriter() = default;
};

class IFileSystem : public glitch::IReferenceCounted {
public:
    virtual ~IFileSystem() = default;
};

// glitch::io::IWriteFile: reference counted write interface (the slots +0x0c .. +0x20 are
// abstract in the original). CWriteFile overrides them.
class IWriteFile : public glitch::IReferenceCounted {
public:
    virtual ~IWriteFile() = default;
    virtual std::size_t write(const void* data, unsigned int count) {  // +0x0c
        (void)data;
        (void)count;
        return 0;
    }
    virtual void flush() {}                                               // +0x10
    virtual bool seek(long offset, bool relative) {                       // +0x14
        (void)offset;
        (void)relative;
        return false;
    }
    virtual long getPos() const { return 0; }                             // +0x18
    virtual const std::string& getFileName() const {                      // +0x1c
        static const std::string none;
        return none;
    }
    virtual const std::string& getFullPath() const {                      // +0x20
        static const std::string none;
        return none;
    }
};

// glitch::io::IFileReadCallBack: the callback interface the XML reader reads through
// (IrrXML's IFileReadCallBack). The default slots do nothing.
class IFileReadCallBack {
public:
    virtual ~IFileReadCallBack() = default;
    virtual void read(void* buffer, int size) {
        (void)buffer;
        (void)size;
    }
    virtual long getSize() const { return 0; }
};

struct SAttributeReadWriteOptions;

// glitch::io::IAttributeExchangingObject: objects that write and read their state
// as attributes. The default bodies do nothing (both are `ret` in the original).
class IAttributeExchangingObject {
public:
    virtual ~IAttributeExchangingObject() = default;
    virtual void serializeAttributes(IAttributes* attributes, SAttributeReadWriteOptions* options) const;  // 0x350c7c
    virtual void deserializeAttributes(IAttributes* attributes, SAttributeReadWriteOptions* options);     // 0x350c80
};

}  // namespace glitch::io
