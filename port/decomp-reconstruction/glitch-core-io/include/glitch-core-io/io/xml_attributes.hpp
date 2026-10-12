// glitch::io::CXMLAttributesReader and CXMLAttributesWriter: attribute (de)serialisers over an
// XML reader or writer. Layout from the original offsets: +0x04 the reader or writer (reference
// counted, taken by the constructor and released by the destructor), +0x08 flag byte,
// +0x0c element name (wide string). The attribute readers are not refcounted themselves.
#pragma once

#include "glitch-core-io/io/interfaces.hpp"
#include "glitch-core-io/io/xml_reader.hpp"
#include "glitch-core-io/io/xml_writer.hpp"

namespace glitch::io {

class CXMLAttributesReader : public IAttributesReader {
public:
    // 0x570cd0 (C2), 0x570d18 (C1). The reader is the port's wide XML reader implementation
    // (IIrrXMLReader<wchar_t> is not in the port).
    CXMLAttributesReader(CXMLReaderImpl<wchar_t, glitch::IReferenceCounted>* reader, bool flag, const wchar_t* name);
    ~CXMLAttributesReader() override;  // 0x570d60 (D2), 0x570d98 (D1)

    CXMLAttributesReader(const CXMLAttributesReader&) = delete;
    CXMLAttributesReader& operator=(const CXMLAttributesReader&) = delete;

    CXMLReaderImpl<wchar_t, glitch::IReferenceCounted>* reader = nullptr;  // +0x04
    bool flag = false;                                                       // +0x08
    const wchar_t* name = nullptr;                                           // +0x0c
};

class CXMLAttributesWriter : public IAttributesWriter {
public:
    // 0x571b00 (C2), 0x571b48 (C1)
    CXMLAttributesWriter(IXMLWriter* writer, bool flag, const wchar_t* name);
    ~CXMLAttributesWriter() override;  // 0x571b90 (D2), 0x571bc8 (D1)

    CXMLAttributesWriter(const CXMLAttributesWriter&) = delete;
    CXMLAttributesWriter& operator=(const CXMLAttributesWriter&) = delete;

    IXMLWriter* writer = nullptr;  // +0x04
    bool flag = false;             // +0x08
    const wchar_t* name = nullptr; // +0x0c
};

}  // namespace glitch::io
