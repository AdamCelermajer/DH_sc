// TiXmlAttribute (TinyXML 2.5.x), as far as TiXmlPrinterDH::VisitEnter uses it.
//
// Next() (0x514640, _ZNK14TiXmlAttribute4NextEv) and Print(FILE*, int, std::string*) (0x517f14 and 0x518a2c,
// _ZNK14TiXmlAttribute5PrintEP7__sFILEiPSs) are upstream TinyXML. TinyXML is not vendored in this repository, so
// they are declared here and not defined. Any binary that links tixml_dh.cpp must supply them. The tests do so with
// tests/tixml_upstream_stand_in.hpp. The data members are the TinyXML ones, kept so that a stand-in can walk the
// list.
#pragma once

#include <cstdio>
#include <string>
#include <utility>

class TiXmlAttribute {
public:
    TiXmlAttribute() = default;
    TiXmlAttribute(std::string name, std::string value) : name_(std::move(name)), value_(std::move(value)) {}

    // Next attribute of the element, or nullptr at the end of the list (upstream).
    const TiXmlAttribute* Next() const;
    // Appends the attribute to *str (upstream). Called by VisitEnter as Print(nullptr, 0, &buffer).
    void Print(std::FILE* cfile, int depth, std::string* str) const;

    std::string name_;
    std::string value_;
    const TiXmlAttribute* next_ = nullptr;
};
