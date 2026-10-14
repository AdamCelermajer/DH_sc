#include "glitch-core-io/io/scalar_attributes.hpp"

#include <cwchar>
#include <string>

#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>

#include "glitch-core-io/core/strings.hpp"

namespace glitch::io {

// ---- CBoolAttribute -------------------------------------------------------

// 0x31d8f8 _ZN6glitch2io14CBoolAttribute6getIntEv
int CBoolAttribute::getInt() const { return value ? 1 : 0; }

// 0x31d900 _ZN6glitch2io14CBoolAttribute8getFloatEv
float CBoolAttribute::getFloat() const { return value ? 1.f : 0.f; }

// 0x31d914 _ZN6glitch2io14CBoolAttribute7getBoolEv
bool CBoolAttribute::getBool() const { return value; }

// 0x31d91c _ZN6glitch2io14CBoolAttribute6setIntEi
void CBoolAttribute::setInt(int v) { value = v != 0; }

// 0x31d92c _ZN6glitch2io14CBoolAttribute8setFloatEf
// NaN compares unequal to 0 and so stores true, as in the original.
void CBoolAttribute::setFloat(float v) { value = !(v == 0.f); }

// 0x31d958 _ZN6glitch2io14CBoolAttribute7setBoolEb
void CBoolAttribute::setBool(bool v) { value = v; }

// 0x31d960 _ZNK6glitch2io14CBoolAttribute7getTypeEv
int CBoolAttribute::getType() const { return 3; }

// 0x323220 _ZN6glitch2io14CBoolAttribute9setStringEPKc
// Only the exact text "true" sets the attribute; any other string sets false.
void CBoolAttribute::setString(const char* text) { value = std::strcmp(text, "true") == 0; }

// ---- CIntAttribute --------------------------------------------------------

// 0x31d978 _ZN6glitch2io13CIntAttribute6getIntEv
int CIntAttribute::getInt() const { return value; }

// 0x31d980 _ZN6glitch2io13CIntAttribute8getFloatEv
float CIntAttribute::getFloat() const { return static_cast<float>(value); }

// 0x31d990 _ZN6glitch2io13CIntAttribute7getBoolEv
bool CIntAttribute::getBool() const { return value != 0; }

// 0x31d9a0 _ZN6glitch2io13CIntAttribute6setIntEi
void CIntAttribute::setInt(int v) { value = v; }

// 0x31d9a8 _ZN6glitch2io13CIntAttribute8setFloatEf
void CIntAttribute::setFloat(float v) { value = static_cast<int>(v); }

// 0x31d9c0 _ZNK6glitch2io13CIntAttribute7getTypeEv
int CIntAttribute::getType() const { return 0; }

// 0x320908 _ZN6glitch2io13CIntAttribute9setStringEPKc
void CIntAttribute::setString(const char* text) { value = std::atoi(text); }

// ---- CFloatAttribute ------------------------------------------------------

// 0x31d9d8 _ZN6glitch2io15CFloatAttribute6getIntEv
int CFloatAttribute::getInt() const { return static_cast<int>(value); }

// 0x31d9e8 _ZN6glitch2io15CFloatAttribute8getFloatEv
float CFloatAttribute::getFloat() const { return value; }

// 0x31d9f0 _ZN6glitch2io15CFloatAttribute7getBoolEv
bool CFloatAttribute::getBool() const { return !(value == 0.f); }

// 0x31da14 _ZN6glitch2io15CFloatAttribute6setIntEi
void CFloatAttribute::setInt(int v) { value = static_cast<float>(v); }

// 0x31da2c _ZN6glitch2io15CFloatAttribute8setFloatEf
void CFloatAttribute::setFloat(float v) { value = v; }

// 0x31da34 _ZNK6glitch2io15CFloatAttribute7getTypeEv
int CFloatAttribute::getType() const { return 1; }

// 0x31da3c _ZNK6glitch2io15CFloatAttribute13getTypeStringEv
const wchar_t* CFloatAttribute::getTypeString() const { return L"float"; }

// 0x322d98 _ZN6glitch2io15CFloatAttribute9setStringEPKc
// fast_atof_move into a local, stored at +0x24 (value). A null text leaves the value unchanged here;
// the original passes it straight to the parser, which faults on it. Parsing is fast_atof_move,
// whose own row is still partial.
void CFloatAttribute::setString(const char* text) {
    if (text == nullptr) return;
    float parsed = 0.f;
    glitch::core::fast_atof_move(text, parsed);
    value = parsed;
}

// 0x32d6fc _ZN6glitch2io15CFloatAttribute10getStringWEv
// Partial: formatted through an output stream on a double, as CStringAttribute::setFloat
// is; the library's digit generation is not the original's.
std::wstring CFloatAttribute::getStringW() const {
    std::ostringstream stream;
    stream << static_cast<double>(value);
    return glitch::core::stringc2stringw(stream.str().c_str());
}

// ---- CEnumAttribute -------------------------------------------------------

// 0x5605a4 _ZN6glitch2io14CEnumAttribute8getFloatEv
float CEnumAttribute::getFloat() const { return static_cast<float>(getInt()); }

// 0x5605bc _ZN6glitch2io14CEnumAttribute7getBoolEv
bool CEnumAttribute::getBool() const { return getInt() != 0; }

// 0x5605d8 _ZN6glitch2io14CEnumAttribute8setFloatEf
void CEnumAttribute::setFloat(float v) { setInt(static_cast<int>(v)); }

// 0x560600 _ZN6glitch2io14CEnumAttribute7getEnumEv
int CEnumAttribute::getEnum() const { return enumValue; }

// 0x560608 _ZNK6glitch2io14CEnumAttribute7getTypeEv
int CEnumAttribute::getType() const { return 4; }

// ---- CUserPointerAttribute ------------------------------------------------
// The original is 32-bit: the pointer is truncated to int for getInt and
// printed as 0x%x. The 64-bit port keeps the same text format with the full
// pointer value, so getInt truncates and getString/setString use %llx.

// 0x560708 _ZN6glitch2io21CUserPointerAttribute6getIntEv
int CUserPointerAttribute::getInt() const {
    return static_cast<int>(reinterpret_cast<std::intptr_t>(value));
}

// 0x560710 _ZN6glitch2io21CUserPointerAttribute7getBoolEv
bool CUserPointerAttribute::getBool() const { return value != nullptr; }

// 0x560720 _ZNK6glitch2io21CUserPointerAttribute7getTypeEv
int CUserPointerAttribute::getType() const { return 0x1c; }

// 0x560728 _ZN6glitch2io21CUserPointerAttribute14setUserPointerEPv
void CUserPointerAttribute::setUserPointer(void* p) { value = p; }

// 0x560730 _ZN6glitch2io21CUserPointerAttribute14getUserPointerEv
void* CUserPointerAttribute::getUserPointer() const { return value; }

// 0x560738 _ZNK6glitch2io21CUserPointerAttribute13getTypeStringEv
const wchar_t* CUserPointerAttribute::getTypeString() const { return L"userPointer"; }

// 0x561a70 _ZN6glitch2io21CUserPointerAttribute9getStringEPc
void CUserPointerAttribute::getString(char* out) const {
    std::sprintf(out, "0x%llx",
                 static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(value)));
}

// 0x561a88 _ZN6glitch2io21CUserPointerAttribute9setStringEPKc
void CUserPointerAttribute::setString(const char* text) {
    unsigned long long raw = 0;
    std::sscanf(text, "0x%llx", &raw);
    value = reinterpret_cast<void*>(static_cast<std::uintptr_t>(raw));
}


// 0x56716c _ZN6glitch2io14CBoolAttributeC1EPKcbb
CBoolAttribute::CBoolAttribute(const char* attrName, bool v, bool f) : value(v) {
    name = attrName;
    flag = f;
}

// 0x5672a4 _ZN6glitch2io13CIntAttributeC1EPKcib
CIntAttribute::CIntAttribute(const char* attrName, int v, bool f) : value(v) {
    name = attrName;
    flag = f;
}

// 0x567208 _ZN6glitch2io15CFloatAttributeC1EPKcfb
CFloatAttribute::CFloatAttribute(const char* attrName, float v, bool f) : value(v) {
    name = attrName;
    flag = f;
}

// 0x566bf4 _ZN6glitch2io21CUserPointerAttributeC1EPKcPvb
CUserPointerAttribute::CUserPointerAttribute(const char* attrName, void* p, bool f) : value(p) {
    name = attrName;
    flag = f;
}

}  // namespace glitch::io

namespace glitch::io {

// 0x325f30 _ZN6glitch2io14CBoolAttribute10getStringWEv
std::wstring CBoolAttribute::getStringW() const { return value ? L"true" : L"false"; }

// 0x325fdc _ZN6glitch2io13CIntAttribute10getStringWEv
// Decimal text, as int2stringw produces it.
std::wstring CIntAttribute::getStringW() const { return std::to_wstring(value); }

}  // namespace glitch::io

namespace glitch::io {

// 0x56706c _ZN6glitch2io14CEnumAttributeC1EPKcS3_PKS3_b
// PARTIAL: the decompiled constructor builds the literal table and the value string, which are
// not in this layout; only the base (name and flag) is set here.
CEnumAttribute::CEnumAttribute(const char* attrName, const char* value, const char* const* choices, bool f)
    : CIntAttribute(attrName, 0, f) {
    (void)value;
    (void)choices;
}

// 0x31d968 _ZNK6glitch2io14CBoolAttribute13getTypeStringEv
// The wide literal is read from the IDA listing (UTF-32LE, cross-referenced only from this function).
const wchar_t* CBoolAttribute::getTypeString() const { return L"bool"; }

// 0x31d9c8 _ZNK6glitch2io13CIntAttribute13getTypeStringEv
// The wide literal is read from the IDA listing (UTF-32LE, cross-referenced only from this function).
const wchar_t* CIntAttribute::getTypeString() const { return L"int"; }

// 0x560610 _ZNK6glitch2io14CEnumAttribute13getTypeStringEv
// The wide literal is read from the IDA listing (UTF-32LE, cross-referenced only from this function).
const wchar_t* CEnumAttribute::getTypeString() const { return L"enum"; }

// 0x562d0c _ZNK6glitch2io14CEnumAttribute9getStringEv
// Copies the narrow text at +0x24 (see the header).
std::string CEnumAttribute::getString() const { return text; }

// 0x567144 _ZN6glitch2io14CEnumAttribute9setStringEPKc
// The original takes the length of the value and assigns the text, so it has no null case;
// a null value is taken as the empty string here.
void CEnumAttribute::setString(const char* value) { text = value != nullptr ? value : ""; }

}  // namespace glitch::io
