// Test-only stand-ins for the upstream TinyXML methods that TiXmlPrinterDH::VisitEnter calls:
// TiXmlAttribute::Next and TiXmlAttribute::Print. TinyXML 2.5.x is not vendored in this repository, so these are not
// the upstream bodies. They walk the attribute list and write name="value", which is enough to check the control flow
// of VisitEnter. The definitions are out of line, not inline, because tixml_dh.o (in the library) calls them from its own
// translation unit. Include this header from exactly one translation unit per test executable, and only from a test.
#pragma once

#include "tinyxml-and-c-libs/tb/tixml_attribute.hpp"

const TiXmlAttribute* TiXmlAttribute::Next() const {
    return next_;
}

void TiXmlAttribute::Print(std::FILE* /*cfile*/, int /*depth*/, std::string* str) const {
    if (str != nullptr) {
        *str += name_;
        *str += "=\"";
        *str += value_;
        *str += "\"";
    }
}
