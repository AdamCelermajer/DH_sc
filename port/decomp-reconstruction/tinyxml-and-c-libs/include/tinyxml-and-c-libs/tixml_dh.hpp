// Game-specific TinyXML pieces (TiXmlPrinterDH and the minimal TiXml surface it
// needs). The rest of the TinyXML 2.5.x-era API in libDungeonHunter2.so is
// unmodified upstream and is ledgered as `upstream`, not rebuilt here; see
// tinyxml-and-c-libs/NOTES.md.
#pragma once

#include <string>
#include <utility>

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

// Subset of TiXmlElement: only the element name is needed by the printer.
class TiXmlElement {
public:
    explicit TiXmlElement(std::string value) : value_(std::move(value)) {}
    const std::string& Value() const { return value_; }
private:
    std::string value_;  // original: std::string at +0x34 (data pointer)
};

// TiXmlPrinter layout (original, 32-bit): vptr +0x00, depth +0x04,
// buffer +0x0c, indent +0x34..+0x38 (string data/length, see NOTES.md).
class TiXmlPrinter : public TiXmlVisitor {
public:
    TiXmlPrinter() = default;
    ~TiXmlPrinter() override = default;

    int depth = 0;              // +0x04
    std::string buffer;         // +0x0c
    std::string indent = "    "; // +0x34..+0x38
};

// Game subclass. VisitEnter(element) writes the indentation for the current
// depth followed by "<name" into the buffer.
class TiXmlPrinterDH : public TiXmlPrinter {
public:
    using TiXmlPrinter::TiXmlPrinter;
    bool VisitEnter(const TiXmlElement& element, const TiXmlAttribute* firstAttribute) override;
};
