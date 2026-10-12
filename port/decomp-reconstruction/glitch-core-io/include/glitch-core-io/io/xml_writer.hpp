// glitch::io::IXMLWriter and glitch::io::CXMLWriter. Text is written as 4-byte code units
// (the original's wchar_t is 4 bytes), each little-endian. Layout of CXMLWriter from the
// original offsets: +0x08 the write file (reference counted), +0x0c indent depth,
// +0x10 flag (cleared by writeXMLHeader and writeClosingTag, set by writeText).
#pragma once

#include <cstdint>

#include "glitch-core-io/deps/glitch_core.hpp"
#include "glitch-core-io/io/interfaces.hpp"

namespace glitch::io {

class IXMLWriter : public glitch::IReferenceCounted {
public:
    virtual void writeXMLHeader() {}                       // +0x0c
    // +0x10 and +0x14 are the writeElement overloads, not modelled here.
    virtual void writeComment(const wchar_t* text) { (void)text; }   // +0x18
    virtual void writeClosingTag(const wchar_t* name) { (void)name; }  // +0x1c
    virtual void writeText(const wchar_t* text) { (void)text; }      // +0x20
    virtual void writeLineBreak() {}                       // +0x24
};

class CXMLWriter : public IXMLWriter {
public:
    explicit CXMLWriter(IWriteFile* writeFile);  // 0x5763a4 (C2), 0x5763f4 (C1)
    ~CXMLWriter() override;                      // 0x576444 (D2), 0x576484 (D1)

    void writeXMLHeader() override;                         // 0x576b00
    void writeComment(const wchar_t* text) override;        // 0x5764c4
    void writeClosingTag(const wchar_t* name) override;     // 0x576590
    void writeText(const wchar_t* text) override;           // 0x576bb8 PARTIAL: no escaping
    void writeLineBreak() override;                         // 0x57653c
    void writeAttribute(const wchar_t* name, const wchar_t* value);  // 0x576694

    CXMLWriter(const CXMLWriter&) = delete;
    CXMLWriter& operator=(const CXMLWriter&) = delete;

    IWriteFile* file = nullptr;  // +0x08
    int indent = 0;              // +0x0c
    bool flag = false;           // +0x10
};

}  // namespace glitch::io
