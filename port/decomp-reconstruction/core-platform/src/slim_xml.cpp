#include "core-platform/slim_xml.hpp"

#include <cassert>
#include <cstdio>
#include <cstring>

namespace slim {

// 0x844908 slim::XmlBase::XmlBase()
XmlBase::XmlBase() = default;

// 0x844acc slim::XmlBase::~XmlBase()
XmlBase::~XmlBase() = default;

// 0x84494c / 0x844988 slim::XmlNode::XmlNode(NodeType, XmlNode*)
XmlNode::XmlNode(NodeType type, XmlNode* parent) : type_(type), parent_(parent) {}

// 0x844b44 / 0x844c8c slim::XmlNode::~XmlNode()
XmlNode::~XmlNode() {
    clearAttribute();
    clearChild();
}

// 0x844d20 slim::XmlNode::addAttribute(char const*, char const*)
// Partial: the listing shows only the allocation of the 0x30-byte attribute; the
// list insertion and the name and value stores are inferred from the parser.
XmlAttribute* XmlNode::addAttribute(const char* name, const char* value) {
    auto* attribute = new XmlAttribute();
    if (name != nullptr) {
        attribute->name = name;
    }
    if (value != nullptr) {
        attribute->value = value;
    }
    attributes_.push_back(attribute);
    return attribute;
}

// 0x844f44 slim::XmlNode::addChild(char const*, slim::NodeType)
// Only element and comment nodes are created; other types return null, as in the
// original. A null name leaves the name empty for the parser to fill in.
XmlNode* XmlNode::addChild(const char* name, NodeType type) {
    if (type != kElement && type != kComment) {
        return nullptr;
    }
    auto* child = new XmlNode(type, this);
    if (name != nullptr) {
        child->name_ = name;
    }
    children_.push_back(child);
    return child;
}

// 0x8449fc slim::XmlNode::clearAttribute()
void XmlNode::clearAttribute() {
    for (XmlAttribute* attribute : attributes_) {
        delete attribute;
    }
    attributes_.clear();
}

// 0x844bd8 slim::XmlNode::clearChild()
void XmlNode::clearChild() {
    for (XmlNode* child : children_) {
        delete child;
    }
    children_.clear();
}

// 0x8445bc slim::XmlNode::findAttribute(char const*) const
const XmlAttribute* XmlNode::findAttribute(const char* name) const {
    for (const XmlAttribute* attribute : attributes_) {
        if (attribute->name == name) {
            return attribute;
        }
    }
    return nullptr;
}

// 0x844828 slim::XmlNode::findChild(char const*) const
XmlNode* XmlNode::findChild(const char* name) const {
    assert(name != nullptr);
    for (XmlNode* child : children_) {
        if (child->name_ == name) {
            return child;
        }
    }
    return nullptr;
}

// 0x84473c slim::XmlNode::findFirstChild(char const*, NodeIterator&) const
// Leaves the iterator on the matched child, so findNextChild can continue after it.
XmlNode* XmlNode::findFirstChild(const char* name, ChildIterator& it) const {
    assert(name != nullptr);
    it = children_.begin();
    while (it != children_.end()) {
        XmlNode* child = *it;
        if (child->name_ == name) {
            return child;
        }
        ++it;
    }
    return nullptr;
}

// 0x844660 slim::XmlNode::findNextChild(char const*, NodeIterator&) const
// Steps past the current iterator before comparing, as the original does.
XmlNode* XmlNode::findNextChild(const char* name, ChildIterator& it) const {
    assert(name != nullptr);
    if (it == children_.end()) {
        return nullptr;
    }
    for (;;) {
        ++it;
        if (it == children_.end()) {
            return nullptr;
        }
        XmlNode* child = *it;
        if (child->name_ == name) {
            return child;
        }
    }
}

// 0x845268 slim::XmlNode::writeNode(slim::String&, int) const
// Partial: the listing's string literals (the tag punctuation and the newline)
// are not recoverable; plain ASCII tags are used. Attributes are written as
// name="value" in the usual order.
void XmlNode::writeNode(std::string& out, int depth) const {
    if (depth < 0) {
        writeChildNodes(out, depth);
        return;
    }
    out.append(static_cast<std::size_t>(depth), '\t');
    if (type_ == kComment) {
        out += "<!--";
        out += name_;
        out += "-->";
        return;
    }
    out += '<';
    out += name_;
    for (const XmlAttribute* attribute : attributes_) {
        out += ' ';
        out += attribute->name;
        out += "=\"";
        out += attribute->value;
        out += '"';
    }
    if (children_.empty() && value_.empty()) {
        out += "/>";
        return;
    }
    out += '>';
    if (children_.empty()) {
        out += value_;
    } else {
        out += '\n';
        writeChildNodes(out, depth);
        out.append(static_cast<std::size_t>(depth), '\t');
    }
    out += "</";
    out += name_;
    out += '>';
}

// 0x845518 slim::XmlNode::writeChildNodes(slim::String&, int) const
void XmlNode::writeChildNodes(std::string& out, int depth) const {
    for (const XmlNode* child : children_) {
        child->writeNode(out, depth + 1);
    }
}

// 0x8449c4 / 0x8449e0 slim::XmlDocument::XmlDocument()
XmlDocument::XmlDocument() : XmlNode(kDocument, nullptr) {}

// 0x844574 slim::XmlDocument::loadFromFile(char const*)
// The original does not load files: a non-null name returns false, and a null
// name fails the assertion.
bool XmlDocument::loadFromFile(const char* filename) {
    if (filename != nullptr) {
        return false;
    }
    std::fprintf(stderr,
                 "Assertion failed: filename != NULL (bool slim::XmlDocument::loadFromFile(const slim::Char*))\n");
    std::abort();
}

// 0x8451dc slim::XmlDocument::loadFromMemory(char const*, unsigned int)
// Clears the children, then parses unless the data is shorter than three bytes or
// starts with a byte-order mark (UTF-16 either way, or UTF-8), which is rejected.
bool XmlDocument::loadFromMemory(const char* data, unsigned length) {
    clearChild();
    if (length > 2) {
        const auto b0 = static_cast<signed char>(data[0]);
        const auto b1 = static_cast<signed char>(data[1]);
        const auto b2 = static_cast<signed char>(data[2]);
        const bool utf16Be = b0 == -2 && b1 == -1;
        const bool utf16Le = b0 == -1 && b1 == -2;
        const bool utf8Bom = b0 == -17 && b1 == -69 && b2 == -65;
        if (!utf16Be && !utf16Le && !utf8Bom) {
            return parse(data, length);
        }
    }
    return false;
}

// 0x844464 slim::XmlDocument::findLabel(char const*&, unsigned int, char const*&, unsigned int&)
// Finds the next '<'. A comment runs to its "-->"; any other label runs to its '>'.
// Sets labelStart to the character after '<' and labelLen to the label's length,
// and moves the cursor past the label.
bool XmlDocument::findLabel(const char*& cursor, std::size_t length, const char*& labelStart,
                            unsigned& labelLen) {
    const void* lt = std::memchr(cursor, '<', length);
    if (lt == nullptr) {
        return false;
    }
    const char* s = static_cast<const char*>(lt) + 1;
    labelStart = s;
    std::size_t rest = static_cast<std::size_t>(cursor + length - s);
    const bool isComment = rest >= 7 && s[0] == '!' && s[1] == '-' && s[2] == '-';
    if (!isComment) {
        const void* gt = std::memchr(s, '>', rest);
        if (gt == nullptr) {
            return false;
        }
        const char* end = static_cast<const char*>(gt);
        labelLen = static_cast<unsigned>(end - s);
        cursor = end + 1;
        return labelLen != 0;
    }
    rest -= 5;
    const char* p = s + 4;
    while (const void* dash = std::memchr(p, '-', rest)) {
        const char* d = static_cast<const char*>(dash);
        if (d[1] == '-' && d[2] == '>') {
            labelLen = static_cast<unsigned>((d + 2) - s);
            cursor = d + 3;
            return true;
        }
        rest = rest - static_cast<std::size_t>(d - p) - 1;
        p = d + 1;
    }
    return false;
}

// 0x844db8 slim::XmlDocument::parseLabel(slim::XmlNode*, char const*, unsigned int)
// Partial: the listing's two-argument string assignments for the attribute value
// are read as the range from the opening to the closing quote.
void XmlDocument::parseLabel(XmlNode* node, const char* label, unsigned length) {
    const char* end = label + length;
    const char* p = label;
    if (*p == '/' || *p == ' ' || *p == '>') {
        node->name_.clear();
    } else {
        while (true) {
            ++p;
            if (*p == '/' || *p == ' ' || *p == '>') {
                break;
            }
            if (*p == '\0' && p >= end) {
                break;
            }
        }
        node->name_.assign(label, p);
    }
    if (*p != ' ') {
        return;
    }
    char c = ' ';
    p = p;  // the attribute scan starts at the space that ended the name
    const char* scan = p;
    if (scan >= end) {
        return;
    }
    do {
        while (c == ' ') {
            ++scan;
            c = *scan;
        }
        const char* nameStart = scan;
        std::size_t nameLen = 0;
        if (c != '=' && c != '/' && c != '>') {
            const char* q = scan;
            do {
                ++q;
                const char d = *q;
                if (d == '=' || d == ' ' || d == '/') {
                    break;
                }
            } while (*q != '>');
            nameLen = static_cast<std::size_t>(q - scan);
            const void* open = std::memchr(q, '"', static_cast<std::size_t>(end - q));
            if (open == nullptr) {
                return;
            }
            const char* valueStart = static_cast<const char*>(open) + 1;
            const void* close = std::memchr(valueStart, '"', static_cast<std::size_t>(end - valueStart));
            if (close == nullptr) {
                return;
            }
            const char* valueEnd = static_cast<const char*>(close);
            XmlAttribute* attribute = node->addAttribute(nullptr, nullptr);
            attribute->name.assign(nameStart, nameLen);
            attribute->value.assign(valueStart, static_cast<std::size_t>(valueEnd - valueStart));
            scan = valueEnd + 1;
            if (scan >= end) {
                return;
            }
            c = *scan;
            continue;
        }
        // A name with no value (or a stray '=' or '/'): nothing to store.
        return;
    } while (scan < end);
}

// 0x844fdc slim::XmlDocument::parse(char const*, unsigned int)
// Walks the labels, building the tree. A closing tag returns to the parent and
// records the text when the element has no children. Processing instructions
// ('?') are skipped. Returns false on malformed nesting.
bool XmlDocument::parse(const char* data, unsigned length) {
    int open = 0;
    XmlNode* current = this;
    const char* cursor = data;
    const char* const limit = data + length;
    const char* labelStart = nullptr;
    unsigned labelLen = 0;
    while (cursor < limit) {
        const char* textStart = cursor;
        if (!findLabel(cursor, static_cast<std::size_t>(limit - cursor), labelStart, labelLen)) {
            break;
        }
        const char first = *labelStart;
        if (first == '/') {
            if (open == 0) {
                return false;
            }
            if (current->type_ == kElement && current->children_.empty()) {
                current->value_.assign(textStart, static_cast<std::size_t>((labelStart - 1) - textStart));
            }
            --open;
            current = current->parent_;
            continue;
        }
        if (first == '?') {
            continue;
        }
        if (first == '!') {
            if (labelLen < 5) {
                return false;
            }
            XmlNode* comment = current->addChild(nullptr, kComment);
            comment->name_.assign(labelStart + 3, labelLen - 5);
            continue;
        }
        XmlNode* element = current->addChild(nullptr, kElement);
        parseLabel(element, labelStart, labelLen);
        if (labelStart[labelLen - 1] != '/') {
            ++open;
            current = element;
        }
    }
    if (open != 0) {
        return false;
    }
    return true;
}

}  // namespace slim
