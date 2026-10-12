#include "tinyxml-and-c-libs/tixml_dh.hpp"

// 0x518110 _ZN14TiXmlPrinterDH10VisitEnterERK12TiXmlElementPK14TiXmlAttribute
// Partial: the decompiled body ends in a non-returning call after the element
// name is appended, so the return value (true, per TiXmlVisitor) and any
// attribute output are inferred, not read from the pseudocode.
bool TiXmlPrinterDH::VisitEnter(const TiXmlElement& element, const TiXmlAttribute* /*firstAttribute*/) {
    for (int i = 0; i < depth; ++i) {
        buffer += indent;
    }
    buffer += "<";
    buffer += element.Value();
    return true;
}
