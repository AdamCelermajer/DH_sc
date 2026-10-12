#include "glitch-core-io/io/xml_attributes.hpp"

namespace glitch::io {

// 0x570cd0 _ZN6glitch2io20CXMLAttributesReaderC2EPNS0_13IIrrXMLReaderIwNS_17IReferenceCountedEEEbPKw
// 0x570d18 (C1) is the same body.
CXMLAttributesReader::CXMLAttributesReader(CXMLReaderImpl<wchar_t, glitch::IReferenceCounted>* readerArg,
                                           bool flagArg, const wchar_t* nameArg)
    : reader(readerArg), flag(flagArg), name(nameArg) {
    ++reader->referenceCounter;
}

// 0x570d60 _ZN6glitch2io20CXMLAttributesReaderD2Ev (D1 at 0x570d98 is the same body)
CXMLAttributesReader::~CXMLAttributesReader() {
    if (reader) reader->drop();
}

// 0x571b00 _ZN6glitch2io20CXMLAttributesWriterC2EPNS0_10IXMLWriterEbPKw
// 0x571b48 (C1) is the same body.
CXMLAttributesWriter::CXMLAttributesWriter(IXMLWriter* writerArg, bool flagArg, const wchar_t* nameArg)
    : writer(writerArg), flag(flagArg), name(nameArg) {
    ++writer->referenceCounter;
}

// 0x571b90 _ZN6glitch2io20CXMLAttributesWriterD2Ev (D1 at 0x571bc8 is the same body)
CXMLAttributesWriter::~CXMLAttributesWriter() {
    if (writer) writer->drop();
}

}  // namespace glitch::io
