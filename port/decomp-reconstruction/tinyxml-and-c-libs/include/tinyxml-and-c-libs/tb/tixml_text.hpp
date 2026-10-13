// TiXmlText (TinyXML 2.5.x), as far as TiXmlPrinterDH::VisitEnter uses it.
#pragma once

#include "tinyxml-and-c-libs/tb/tixml_node.hpp"

class TiXmlText : public TiXmlNode {
public:
    explicit TiXmlText(bool cdata = false) : cdata_(cdata) {}

    // Vtable slot +0x20. TiXmlText::ToText() const (0x514298) returns this.
    const TiXmlText* ToText() const override { return this; }

    // The cdata flag is the byte at +0x40. VisitEnter reads it with ldrb [r0, #0x40].
    bool CDATA() const { return cdata_; }

private:
    bool cdata_;
};
