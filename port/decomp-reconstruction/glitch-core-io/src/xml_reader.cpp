#include "glitch-core-io/io/xml_reader.hpp"

#include <cstdlib>

#include "glitch-core-io/core/strings.hpp"

namespace glitch::io {

// ---- CXMLReaderImpl<char> -------------------------------------------------

using CharReader = CXMLReaderImpl<char, glitch::IReferenceCounted>;

// 0x572634 _ZN6glitch2io14CXMLReaderImplIcNS_17IReferenceCountedEE17readToNextElementEv
bool CharReader::readToNextElement() {
    do {
        if (!read()) return false;
    } while (getNodeType() != 1);
    return true;
}

// 0x572678 _ZNK6glitch2io14CXMLReaderImplIcNS_17IReferenceCountedEE11getNodeTypeEv
int CharReader::getNodeType() const { return nodeType; }

// 0x572680 _ZNK6glitch2io14CXMLReaderImplIcNS_17IReferenceCountedEE17getAttributeCountEv
int CharReader::getAttributeCount() const { return static_cast<int>(attributes.size()); }

// 0x5726a8 _ZNK6glitch2io14CXMLReaderImplIcNS_17IReferenceCountedEE16getAttributeNameEi
// Indices are compared unsigned, so a negative index gives nullptr as well.
const char* CharReader::getAttributeName(int index) const {
    const auto i = static_cast<std::size_t>(static_cast<unsigned>(index));
    return i < attributes.size() ? attributes[i].name.c_str() : nullptr;
}

// 0x5726e4 _ZNK6glitch2io14CXMLReaderImplIcNS_17IReferenceCountedEE17getAttributeValueEi
const char* CharReader::getAttributeValue(int index) const {
    const auto i = static_cast<std::size_t>(static_cast<unsigned>(index));
    return i < attributes.size() ? attributes[i].value.c_str() : nullptr;
}

// 0x5742a4 _ZNK6glitch2io14CXMLReaderImplIcNS_17IReferenceCountedEE17getAttributeValueEPKc
const char* CharReader::getAttributeValue(const char* name) const {
    const SAttribute* attribute = getAttributeByName(name);
    return attribute ? attribute->value.c_str() : nullptr;
}

// 0x574288 _ZNK6glitch2io14CXMLReaderImplIcNS_17IReferenceCountedEE21getAttributeValueSafeEPKc
const char* CharReader::getAttributeValueSafe(const char* name) const {
    const SAttribute* attribute = getAttributeByName(name);
    return attribute ? attribute->value.c_str() : defaultValue.c_str();
}

// 0x572720 _ZNK6glitch2io14CXMLReaderImplIcNS_17IReferenceCountedEE22getAttributeValueAsIntEPKc
// Goes through the virtual float accessor (vtable +0x34), then truncates.
int CharReader::getAttributeValueAsInt(const char* name) const {
    return static_cast<int>(getAttributeValueAsFloat(name));
}

// 0x572738 _ZNK6glitch2io14CXMLReaderImplIcNS_17IReferenceCountedEE22getAttributeValueAsIntEi
int CharReader::getAttributeValueAsInt(int index) const {
    return static_cast<int>(getAttributeValueAsFloat(index));
}

// 0x5742b8 _ZNK6glitch2io14CXMLReaderImplIcNS_17IReferenceCountedEE24getAttributeValueAsFloatEPKc
float CharReader::getAttributeValueAsFloat(const char* name) const {
    const SAttribute* attribute = getAttributeByName(name);
    if (!attribute) return 0.f;
    return static_cast<float>(std::strtod(attribute->value.c_str(), nullptr));
}

// 0x574354 _ZNK6glitch2io14CXMLReaderImplIcNS_17IReferenceCountedEE24getAttributeValueAsFloatEi
float CharReader::getAttributeValueAsFloat(int index) const {
    const char* text = getAttributeValue(index);
    if (!text) return 0.f;
    float result = 0.f;
    glitch::core::fast_atof_move(text, result);
    return result;
}

// 0x572750 _ZNK6glitch2io14CXMLReaderImplIcNS_17IReferenceCountedEE11getNodeNameEv
const char* CharReader::getNodeName() const { return nodeName.c_str(); }

// 0x572758 _ZNK6glitch2io14CXMLReaderImplIcNS_17IReferenceCountedEE11getNodeDataEv
// Text nodes keep their data in the same string as the node name.
const char* CharReader::getNodeData() const { return nodeName.c_str(); }

// 0x572760 _ZNK6glitch2io14CXMLReaderImplIcNS_17IReferenceCountedEE14isEmptyElementEv
bool CharReader::isEmptyElement() const { return emptyElement; }

// 0x572768 _ZNK6glitch2io14CXMLReaderImplIcNS_17IReferenceCountedEE15getSourceFormatEv
int CharReader::getSourceFormat() const { return sourceFormat; }

// 0x572770 _ZNK6glitch2io14CXMLReaderImplIcNS_17IReferenceCountedEE15getParserFormatEv
int CharReader::getParserFormat() const { return parserFormat; }

// 0x574164 _ZNK6glitch2io14CXMLReaderImplIcNS_17IReferenceCountedEE18getAttributeByNameEPKc
const CharReader::SAttribute* CharReader::getAttributeByName(const char* name) const {
    if (!name) return nullptr;
    const std::string key(name);
    for (const SAttribute& attribute : attributes) {
        if (attribute.name == key) return &attribute;
    }
    return nullptr;
}

// ---- CXMLReaderImpl<wchar_t> ----------------------------------------------

using WideReader = CXMLReaderImpl<wchar_t, glitch::IReferenceCounted>;

// 0x572778 _ZN6glitch2io14CXMLReaderImplIwNS_17IReferenceCountedEE17readToNextElementEv
bool WideReader::readToNextElement() {
    do {
        if (!read()) return false;
    } while (getNodeType() != 1);
    return true;
}

// 0x5727bc _ZNK6glitch2io14CXMLReaderImplIwNS_17IReferenceCountedEE11getNodeTypeEv
int WideReader::getNodeType() const { return nodeType; }

// 0x5727c4 _ZNK6glitch2io14CXMLReaderImplIwNS_17IReferenceCountedEE17getAttributeCountEv
int WideReader::getAttributeCount() const { return static_cast<int>(attributes.size()); }

// 0x5727f4 _ZNK6glitch2io14CXMLReaderImplIwNS_17IReferenceCountedEE16getAttributeNameEi
const wchar_t* WideReader::getAttributeName(int index) const {
    const auto i = static_cast<std::size_t>(static_cast<unsigned>(index));
    return i < attributes.size() ? attributes[i].name.c_str() : nullptr;
}

// 0x572838 _ZNK6glitch2io14CXMLReaderImplIwNS_17IReferenceCountedEE17getAttributeValueEi
const wchar_t* WideReader::getAttributeValue(int index) const {
    const auto i = static_cast<std::size_t>(static_cast<unsigned>(index));
    return i < attributes.size() ? attributes[i].value.c_str() : nullptr;
}

// 0x573ab0 _ZNK6glitch2io14CXMLReaderImplIwNS_17IReferenceCountedEE17getAttributeValueEPKw
const wchar_t* WideReader::getAttributeValue(const wchar_t* name) const {
    const SAttribute* attribute = getAttributeByName(name);
    return attribute ? attribute->value.c_str() : nullptr;
}

// 0x573a94 _ZNK6glitch2io14CXMLReaderImplIwNS_17IReferenceCountedEE21getAttributeValueSafeEPKw
const wchar_t* WideReader::getAttributeValueSafe(const wchar_t* name) const {
    const SAttribute* attribute = getAttributeByName(name);
    return attribute ? attribute->value.c_str() : defaultValue.c_str();
}

// 0x57287c _ZNK6glitch2io14CXMLReaderImplIwNS_17IReferenceCountedEE22getAttributeValueAsIntEPKw
int WideReader::getAttributeValueAsInt(const wchar_t* name) const {
    return static_cast<int>(getAttributeValueAsFloat(name));
}

// 0x572894 _ZNK6glitch2io14CXMLReaderImplIwNS_17IReferenceCountedEE22getAttributeValueAsIntEi
int WideReader::getAttributeValueAsInt(int index) const {
    return static_cast<int>(getAttributeValueAsFloat(index));
}

// 0x574488 _ZNK6glitch2io14CXMLReaderImplIwNS_17IReferenceCountedEE24getAttributeValueAsFloatEPKw
// PARTIAL: the value is narrowed with stringw2stringc (partial) and read with strtod, as the
// decompile does. A missing attribute gives zero.
float WideReader::getAttributeValueAsFloat(const wchar_t* name) const {
    const SAttribute* attribute = getAttributeByName(name);
    if (!attribute) return 0.f;
    const std::string narrow = glitch::core::stringw2stringc(attribute->value.c_str());
    return static_cast<float>(std::strtod(narrow.c_str(), nullptr));
}

// 0x5743f0 _ZNK6glitch2io14CXMLReaderImplIwNS_17IReferenceCountedEE24getAttributeValueAsFloatEi
// PARTIAL: as the name form. The original parses the narrowed text with fast_atof_move, which is
// partial in the port (see the narrow reader).
float WideReader::getAttributeValueAsFloat(int index) const {
    const wchar_t* text = getAttributeValue(index);
    if (!text) return 0.f;
    const std::string narrow = glitch::core::stringw2stringc(text);
    float result = 0.f;
    glitch::core::fast_atof_move(narrow.c_str(), result);
    return result;
}

// 0x5728ac _ZNK6glitch2io14CXMLReaderImplIwNS_17IReferenceCountedEE11getNodeNameEv
const wchar_t* WideReader::getNodeName() const { return nodeName.c_str(); }

// 0x5728b4 _ZNK6glitch2io14CXMLReaderImplIwNS_17IReferenceCountedEE11getNodeDataEv
const wchar_t* WideReader::getNodeData() const { return nodeName.c_str(); }

// 0x5728bc _ZNK6glitch2io14CXMLReaderImplIwNS_17IReferenceCountedEE14isEmptyElementEv
bool WideReader::isEmptyElement() const { return emptyElement; }

// 0x5728c4 _ZNK6glitch2io14CXMLReaderImplIwNS_17IReferenceCountedEE15getSourceFormatEv
int WideReader::getSourceFormat() const { return sourceFormat; }

// 0x5728cc _ZNK6glitch2io14CXMLReaderImplIwNS_17IReferenceCountedEE15getParserFormatEv
int WideReader::getParserFormat() const { return parserFormat; }

// 0x5739cc _ZNK6glitch2io14CXMLReaderImplIwNS_17IReferenceCountedEE18getAttributeByNameEPKw
const WideReader::SAttribute* WideReader::getAttributeByName(const wchar_t* name) const {
    if (!name) return nullptr;
    const std::wstring key(name);
    for (const SAttribute& attribute : attributes) {
        if (attribute.name == key) return &attribute;
    }
    return nullptr;
}

}  // namespace glitch::io
