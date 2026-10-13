// TiXmlNode (TinyXML 2.5.x), as far as TiXmlPrinterDH::VisitEnter uses it: the child links and ToText().
//
// The original keeps the first child at +0x18 and the last child at +0x1c. The child links here are non-owning. The
// TinyXML parser that fills them is upstream and is not in this repository, so SetChildLinks is a stand-in that the
// tests use to build an element.
#pragma once

class TiXmlText;

class TiXmlNode {
public:
    TiXmlNode() = default;
    virtual ~TiXmlNode() = default;

    // Vtable slot +0x20. TiXmlNode::ToText() const (0x514248) returns null. TiXmlText overrides it.
    virtual const TiXmlText* ToText() const { return nullptr; }

    const TiXmlNode* FirstChild() const { return firstChild_; }  // +0x18
    const TiXmlNode* LastChild() const { return lastChild_; }    // +0x1c

    // Stand-in for the child list. Sets the first and last child links without taking ownership.
    // TinyXML 2.5.x, unmodified upstream in the binary (0x515964, a ledgered `upstream` row; no source file in the tree yet):
    // appends the node as the last child, takes ownership and returns the node.
    TiXmlNode* LinkEndChild(TiXmlNode* node);

    void SetChildLinks(const TiXmlNode* first, const TiXmlNode* last) {
        firstChild_ = first;
        lastChild_ = last;
    }

private:
    const TiXmlNode* firstChild_ = nullptr;
    const TiXmlNode* lastChild_ = nullptr;
};
