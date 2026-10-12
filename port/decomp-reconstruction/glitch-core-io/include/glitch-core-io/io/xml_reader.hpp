// glitch::io::CXMLReaderImpl<char / wchar_t, glitch::IReferenceCounted>: the accessors
// of the XML reader. The parser (read(), vtable +0x0c) is implemented by derived readers
// and is not reconstructed here. The two instantiations do not share a layout: the
// attribute records are 0x30 bytes for char and 0x90 bytes for wchar_t, so each is an
// explicit specialisation. Offsets are the original's, kept as comments.
#pragma once

#include <string>
#include <vector>

#include "glitch-core-io/deps/glitch_core.hpp"

namespace glitch::io {

template <typename CharT, typename Super>
class CXMLReaderImpl;

template <>
class CXMLReaderImpl<char, glitch::IReferenceCounted> : public glitch::IReferenceCounted {
public:
    // One attribute record (0x30 bytes in the original).
    struct SAttribute {
        std::string name;   // +0x14 (its c_str is returned)
        std::string value;  // +0x2c (its c_str is returned)
    };

    // 0x573eb0 (D1): PARTIAL. As the wide reader: the defaulted destructor releases the std members
    // in place of the original buffer frees (layout differs).
    ~CXMLReaderImpl() override = default;

    // Parser step (vtable +0x0c). Provided by the concrete reader.
    virtual bool read() = 0;

    // 0x572634 (vtable +0x10): reads until the node type is 1, false at the end.
    virtual bool readToNextElement();
    virtual int getNodeType() const;                                  // 0x572678 (+0x14)
    virtual int getAttributeCount() const;                            // 0x572680 (+0x18)
    virtual const char* getAttributeName(int index) const;            // 0x5726a8 (+0x1c)
    virtual const char* getAttributeValue(int index) const;           // 0x5726e4 (+0x20)
    virtual const char* getAttributeValue(const char* name) const;    // 0x5742a4 (+0x24)
    virtual const char* getAttributeValueSafe(const char* name) const;  // 0x574288 (+0x28)
    virtual int getAttributeValueAsInt(const char* name) const;       // 0x572720 (+0x2c)
    virtual int getAttributeValueAsInt(int index) const;              // 0x572738 (+0x30)
    virtual float getAttributeValueAsFloat(const char* name) const;   // 0x5742b8 (+0x34)
    virtual float getAttributeValueAsFloat(int index) const;          // 0x574354 (+0x38)
    virtual const char* getNodeName() const;                          // 0x572750 (+0x3c)
    virtual const char* getNodeData() const;                          // 0x572758 (+0x40)
    virtual bool isEmptyElement() const;                              // 0x572760 (+0x44)
    virtual int getSourceFormat() const;                              // 0x572768 (+0x48)
    virtual int getParserFormat() const;                              // 0x572770 (+0x4c)

    // First attribute with this name, or nullptr. 0x574164
    const SAttribute* getAttributeByName(const char* name) const;

    int nodeType = 0;                    // +0x18
    int sourceFormat = 0;                // +0x1c
    int parserFormat = 0;                // +0x20
    std::string nodeName;                // +0x38 (the node data is the same string)
    bool emptyElement = false;           // +0x54
    std::string defaultValue;            // +0x50: returned by getAttributeValueSafe when missing
    std::vector<SAttribute> attributes;  // +0x64 begin, +0x68 end
};

template <>
class CXMLReaderImpl<wchar_t, glitch::IReferenceCounted> : public glitch::IReferenceCounted {
public:
    // One attribute record (0x90 bytes in the original).
    struct SAttribute {
        std::wstring name;   // +0x44 (its c_str is returned)
        std::wstring value;  // +0x8c (its c_str is returned)
    };

    // 0x573928 (D1): PARTIAL. The original frees its text and attribute buffers in member
    // order; the defaulted destructor releases the std members the same way (layout differs).
    ~CXMLReaderImpl() override = default;

    virtual bool read() = 0;  // vtable +0x0c

    virtual bool readToNextElement();                                    // 0x572778 (+0x10)
    virtual int getNodeType() const;                                     // 0x5727bc (+0x14)
    virtual int getAttributeCount() const;                               // 0x5727c4 (+0x18)
    virtual const wchar_t* getAttributeName(int index) const;            // 0x5727f4 (+0x1c)
    virtual const wchar_t* getAttributeValue(int index) const;           // 0x572838 (+0x20)
    virtual const wchar_t* getAttributeValue(const wchar_t* name) const;  // 0x573ab0 (+0x24)
    virtual const wchar_t* getAttributeValueSafe(const wchar_t* name) const;  // 0x573a94 (+0x28)
    virtual int getAttributeValueAsInt(const wchar_t* name) const;       // 0x57287c (+0x2c)
    virtual int getAttributeValueAsInt(int index) const;                 // 0x572894 (+0x30)
    // Float accessors (0x574488 and 0x5743f0): PARTIAL, defined in xml_reader.cpp. The text is
    // narrowed through stringw2stringc, which is itself partial.
    virtual float getAttributeValueAsFloat(const wchar_t* name) const;  // 0x574488 (+0x34)
    virtual float getAttributeValueAsFloat(int index) const;            // 0x5743f0 (+0x38)
    virtual const wchar_t* getNodeName() const;                          // 0x5728ac (+0x3c)
    virtual const wchar_t* getNodeData() const;                          // 0x5728b4 (+0x40)
    virtual bool isEmptyElement() const;                                 // 0x5728bc (+0x44)
    virtual int getSourceFormat() const;                                 // 0x5728c4 (+0x48)
    virtual int getParserFormat() const;                                 // 0x5728cc (+0x4c)

    // First attribute with this name, or nullptr. 0x5739cc
    const SAttribute* getAttributeByName(const wchar_t* name) const;

    int nodeType = 0;                    // +0x18
    int sourceFormat = 0;                // +0x1c
    int parserFormat = 0;                // +0x20
    std::wstring nodeName;               // +0x68 (the node data is the same string)
    bool emptyElement = false;           // +0xb4
    std::wstring defaultValue;           // +0xb0: returned by getAttributeValueSafe when missing
    std::vector<SAttribute> attributes;  // +0xc4 begin, +0xc8 end
};

}  // namespace glitch::io
