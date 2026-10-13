// Game-specific TinyXML pieces (TiXmlPrinterDH and the minimal TiXml surface it
// needs). The rest of the TinyXML 2.5.x-era API in libDungeonHunter2.so is
// unmodified upstream and is ledgered as `upstream`, not rebuilt here; see
// tinyxml-and-c-libs/NOTES.md.
#pragma once

#include <string>
#include <utility>

#include "tinyxml-and-c-libs/tb/tixml_attribute.hpp"
#include "tinyxml-and-c-libs/tb/tixml_node.hpp"

class TiXmlDocument;
class TiXmlElement;
class TiXmlAttribute;

// Subset of TiXmlVisitor used by the printer. Virtual slots follow the
// original vtable order (VisitEnter/VisitExit first).
class TiXmlVisitor {
public:
    virtual ~TiXmlVisitor() = default;
    virtual bool VisitEnter(const TiXmlDocument&) { return true; }
    virtual bool VisitExit(const TiXmlDocument&) { return true; }
    virtual bool VisitEnter(const TiXmlElement&, const TiXmlAttribute*) { return true; }
    virtual bool VisitExit(const TiXmlElement&) { return true; }
};

// Subset of TiXmlElement: the element name, and the child links (from TiXmlNode) that the printer reads.
class TiXmlElement : public TiXmlNode {
public:
    explicit TiXmlElement(std::string value) : value_(std::move(value)) {}
    const std::string& Value() const { return value_; }
    // TinyXML 2.5.x, unmodified upstream in the binary (ledgered `upstream` rows, no source file in the tree yet):
    const char* Attribute(const char* name) const;                          // 0x514c70
    void SetAttribute(const std::string& name, const std::string& value);   // 0x516400
private:
    std::string value_;  // original: the name's begin pointer at +0x34 (VisitEnter reads it)
};

// TiXmlPrinter layout (original, 32-bit): vptr +0x00, depth +0x04, simpleTextPrint +0x08,
// buffer +0x0c, indent +0x24 (string object), lineBreak +0x3c (string object). The defaults are set
// by TiXmlPrinter::TiXmlPrinter (0x485aa0): indent is a tab and lineBreak is a newline.
class TiXmlPrinter : public TiXmlVisitor {
public:
    TiXmlPrinter() = default;
    ~TiXmlPrinter() override = default;

    int depth = 0;                  // +0x04
    bool simpleTextPrint = false;   // +0x08
    std::string buffer;             // +0x0c
    std::string indent = "\t";      // +0x24
    std::string lineBreak = "\n";   // +0x3c
};

// Game subclass. VisitEnter(element, attributes) writes the start tag into the buffer: indentation, "<name",
// attributes (each on its own line), and then either " />" or ">" with a line break, or the simpleTextPrint flag.
class TiXmlPrinterDH : public TiXmlPrinter {
public:
    using TiXmlPrinter::TiXmlPrinter;
    bool VisitEnter(const TiXmlElement& element, const TiXmlAttribute* firstAttribute) override;
};
