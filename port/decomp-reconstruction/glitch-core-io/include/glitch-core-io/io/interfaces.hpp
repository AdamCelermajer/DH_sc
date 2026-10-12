// Interfaces of glitch::io that carry no behaviour of their own. Their destructors
// are empty (the original emits a single `ret` for the complete-object variant and a
// `delete` wrapper for the deleting variant), so they are declared with defaulted
// virtual destructors here.
#pragma once

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

class IFileSystem {
public:
    virtual ~IFileSystem() = default;
};

class IWriteFile {
public:
    virtual ~IWriteFile() = default;
};

class IFileReadCallBack {
public:
    virtual ~IFileReadCallBack() = default;
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
