#include "glitch-core-io/io/typed_attributes.hpp"

#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <sstream>
#include <string>
#include <vector>

#include "glitch-core-io/core/strings.hpp"

namespace glitch::io {

// 0x31f2ac _ZNK6glitch2io15CColorAttribute7getTypeEv
int CColorAttribute::getType() const { return 5; }

// 0x31f114 _ZNK6glitch2io16CColorfAttribute7getTypeEv
int CColorfAttribute::getType() const { return 6; }
// 0x31f11c _ZNK6glitch2io16CColorfAttribute13getTypeStringEv
const wchar_t* CColorfAttribute::getTypeString() const { return L"colorf"; }

// 0x31f2c4 _ZNK6glitch2io18CVector2DAttribute7getTypeEv
int CVector2DAttribute::getType() const { return 7; }
// 0x31f2cc _ZNK6glitch2io18CVector2DAttribute13getTypeStringEv
const wchar_t* CVector2DAttribute::getTypeString() const { return L"vector2d"; }

// 0x31f2dc _ZNK6glitch2io18CVector3DAttribute7getTypeEv
int CVector3DAttribute::getType() const { return 8; }
// 0x31f2e4 _ZNK6glitch2io18CVector3DAttribute13getTypeStringEv
const wchar_t* CVector3DAttribute::getTypeString() const { return L"vector3d"; }

// 0x31f2f4 _ZNK6glitch2io18CVector4DAttribute7getTypeEv
int CVector4DAttribute::getType() const { return 9; }
// 0x31f2fc _ZNK6glitch2io18CVector4DAttribute13getTypeStringEv
const wchar_t* CVector4DAttribute::getTypeString() const { return L"vector4d"; }

// 0x560484 _ZNK6glitch2io19CVector2DIAttribute7getTypeEv
int CVector2DIAttribute::getType() const { return 0xa; }
// 0x56048c _ZNK6glitch2io19CVector2DIAttribute13getTypeStringEv
const wchar_t* CVector2DIAttribute::getTypeString() const { return L"vector2di"; }

// 0x56049c _ZNK6glitch2io19CVector3DIAttribute7getTypeEv
int CVector3DIAttribute::getType() const { return 0xb; }
// 0x5604a4 _ZNK6glitch2io19CVector3DIAttribute13getTypeStringEv
const wchar_t* CVector3DIAttribute::getTypeString() const { return L"vector3di"; }

// 0x5604b4 _ZNK6glitch2io19CVector4DIAttribute7getTypeEv
int CVector4DIAttribute::getType() const { return 0xc; }
// 0x5604bc _ZNK6glitch2io19CVector4DIAttribute13getTypeStringEv
const wchar_t* CVector4DIAttribute::getTypeString() const { return L"vector4di"; }

// 0x5604cc _ZNK6glitch2io20CPosition2DAttribute7getTypeEv
int CPosition2DAttribute::getType() const { return 0xd; }
// 0x5604d4 _ZNK6glitch2io20CPosition2DAttribute13getTypeStringEv
// Partial: the wide literal is not resolved in strings.csv; the name is inferred from the class.
const wchar_t* CPosition2DAttribute::getTypeString() const { return L"position"; }

// 0x5604e4 _ZNK6glitch2io14CRectAttribute7getTypeEv
int CRectAttribute::getType() const { return 0xe; }

// 0x5604fc _ZNK6glitch2io16CMatrixAttribute7getTypeEv
int CMatrixAttribute::getType() const { return 0xf; }

// 0x560514 _ZNK6glitch2io20CQuaternionAttribute7getTypeEv
int CQuaternionAttribute::getType() const { return 0x10; }
// 0x56051c _ZNK6glitch2io20CQuaternionAttribute13getTypeStringEv
const wchar_t* CQuaternionAttribute::getTypeString() const { return L"quaternion"; }

// 0x56052c _ZNK6glitch2io14CBBoxAttribute7getTypeEv
int CBBoxAttribute::getType() const { return 0x11; }

// 0x560544 _ZNK6glitch2io15CPlaneAttribute7getTypeEv
int CPlaneAttribute::getType() const { return 0x12; }
// 0x56054c _ZNK6glitch2io15CPlaneAttribute13getTypeStringEv
// Partial: the wide literal is not resolved in strings.csv; the name is inferred from the class.
const wchar_t* CPlaneAttribute::getTypeString() const { return L"plane"; }

// 0x56055c _ZNK6glitch2io18CTriangleAttribute7getTypeEv
int CTriangleAttribute::getType() const { return 0x13; }
// 0x560564 _ZNK6glitch2io18CTriangleAttribute13getTypeStringEv
const wchar_t* CTriangleAttribute::getTypeString() const { return L"triangle"; }

// 0x560574 _ZNK6glitch2io16CLine2dAttribute7getTypeEv
int CLine2dAttribute::getType() const { return 0x14; }

// 0x56058c _ZNK6glitch2io16CLine3dAttribute7getTypeEv
int CLine3dAttribute::getType() const { return 0x15; }

// 0x5606f0 _ZNK6glitch2io22CStringWArrayAttribute7getTypeEv
int CStringWArrayAttribute::getType() const { return 0x16; }
// 0x5606f8 _ZNK6glitch2io22CStringWArrayAttribute13getTypeStringEv
// Partial: the wide literal is not resolved in strings.csv; the name is inferred from the class.
const wchar_t* CStringWArrayAttribute::getTypeString() const { return L"stringwarray"; }

// 0x31f30c _ZNK6glitch2io16CStringAttribute7getTypeEv
int CStringAttribute::getType() const { return 2; }
// 0x31f314 _ZNK6glitch2io16CStringAttribute13getTypeStringEv
const wchar_t* CStringAttribute::getTypeString() const { return L"string"; }

// 0x5606d8 _ZNK6glitch2io15CLightAttribute7getTypeEv
int CLightAttribute::getType() const { return 0x1b; }
// 0x5606e0 _ZNK6glitch2io15CLightAttribute13getTypeStringEv
// Partial: the wide literal is not resolved in strings.csv; the name is inferred from the class.
const wchar_t* CLightAttribute::getTypeString() const { return L"light"; }
// 0x5606c4 _ZN6glitch2io15CLightAttribute7getBoolEv
bool CLightAttribute::getBool() const { return light.get() != nullptr; }
// 0x5606d4 _ZN6glitch2io15CLightAttribute9setStringEPKc
void CLightAttribute::setString(const char*) {}

// 0x560690 _ZNK6glitch2io17CTextureAttribute7getTypeEv
int CTextureAttribute::getType() const { return 0x1a; }
// 0x560698 _ZNK6glitch2io17CTextureAttribute13getTypeStringEv
// Partial: the wide literal is not resolved in strings.csv; the name is inferred from the class.
const wchar_t* CTextureAttribute::getTypeString() const { return L"texture"; }
// 0x560654 _ZN6glitch2io17CTextureAttribute7getBoolEv
bool CTextureAttribute::getBool() const { return texture.get() != nullptr; }

// 0x562f10 _ZN6glitch2io22CStringWArrayAttribute8getArrayEv
std::vector<std::wstring> CStringWArrayAttribute::getArray() const { return array; }

// 0x5632a4 _ZN6glitch2io22CStringWArrayAttribute8setArrayE...
void CStringWArrayAttribute::setArray(const std::vector<std::wstring>& value) { array = value; }

}  // namespace glitch::io

namespace glitch::io {

// 0x31f2b4 _ZNK6glitch2io15CColorAttribute13getTypeStringEv
const wchar_t* CColorAttribute::getTypeString() const { return L"color"; }

// 0x560504 _ZNK6glitch2io16CMatrixAttribute13getTypeStringEv
const wchar_t* CMatrixAttribute::getTypeString() const { return L"matrix"; }

}  // namespace glitch::io

// ---- CColorAttribute / CColorfAttribute integer view ----------------------
// Both classes use the same bodies. The original stores the four colour bytes
// into a stack word, reads that word back (so the result is the packed colour
// rotated left by one byte), and for setInt writes four floats byte / 255 into
// the float list at +0x30: element 0 from bits 8..15, element 1 from bits
// 16..23, element 2 from bits 24..31 and element 3 from bits 0..7.

namespace glitch::io {
namespace {

// 0x3b808081 is the single-precision constant the original multiplies by.
float byteScale() {
    const std::uint32_t bits = 0x3b808081u;
    float f;
    std::memcpy(&f, &bits, sizeof f);
    return f;
}

// Rotate left by one byte: the byte order the original's stack round trip produces.
std::uint32_t rotatedColor(std::uint32_t packed) { return (packed << 8) | (packed >> 24); }

int colorInt(const IAttribute& attribute) {
    return static_cast<int>(rotatedColor(attribute.getColor().color));
}

float colorFloat(const IAttribute& attribute) {
    return static_cast<float>(rotatedColor(attribute.getColor().color));
}

// Stores the four bytes of value as unit floats. The original writes without a
// size check; the guard below only matters for a list shorter than four.
void storeColorBytes(std::vector<float>& floats, int value) {
    const auto v = static_cast<std::uint32_t>(value);
    const float parts[4] = {
        static_cast<float>((v >> 8) & 0xffu) * byteScale(),
        static_cast<float>((v >> 16) & 0xffu) * byteScale(),
        static_cast<float>((v >> 24) & 0xffu) * byteScale(),
        static_cast<float>(v & 0xffu) * byteScale(),
    };
    for (std::size_t i = 0; i < 4 && i < floats.size(); ++i) floats[i] = parts[i];
}

}  // namespace

// 0x31f12c _ZN6glitch2io15CColorAttribute6getIntEv
int CColorAttribute::getInt() const { return colorInt(*this); }

// 0x31f18c _ZN6glitch2io15CColorAttribute8getFloatEv
float CColorAttribute::getFloat() const { return colorFloat(*this); }

// 0x31f1f0 _ZN6glitch2io15CColorAttribute6setIntEi
void CColorAttribute::setInt(int value) { storeColorBytes(floatValues, value); }

// 0x31f284 _ZN6glitch2io15CColorAttribute8setFloatEf
void CColorAttribute::setFloat(float value) { setInt(static_cast<int>(value)); }

// 0x31ef94 _ZN6glitch2io16CColorfAttribute6getIntEv
int CColorfAttribute::getInt() const { return colorInt(*this); }

// 0x31eff4 _ZN6glitch2io16CColorfAttribute8getFloatEv
float CColorfAttribute::getFloat() const { return colorFloat(*this); }

// 0x31f058 _ZN6glitch2io16CColorfAttribute6setIntEi
void CColorfAttribute::setInt(int value) { storeColorBytes(floatValues, value); }

// 0x31f0ec _ZN6glitch2io16CColorfAttribute8setFloatEf
void CColorfAttribute::setFloat(float value) { setInt(static_cast<int>(value)); }

// ---- CStringAttribute -----------------------------------------------------

namespace {

// Narrow text of the value. The wide copy is narrowed element by element (the
// original's templated range copy); the narrow copy is used as it is.
std::string narrowText(const CStringAttribute& a) {
    if (!a.isWide) return a.narrow;
    std::string out;
    out.reserve(a.wide.size());
    for (wchar_t c : a.wide) out.push_back(static_cast<char>(c));
    return out;
}

// Wide text of the value. The narrow copy is widened with sign extension (ldrsb).
std::wstring wideText(const CStringAttribute& a) {
    if (a.isWide) return a.wide;
    std::wstring out;
    out.reserve(a.narrow.size());
    for (char c : a.narrow) out.push_back(static_cast<wchar_t>(static_cast<signed char>(c)));
    return out;
}

// strcasecmp(a, b) == 0 for the ASCII subset used by the "true" test.
bool equalsIgnoreCase(const char* a, const char* b) {
    for (; *a != '\0' && *b != '\0'; ++a, ++b) {
        if (std::tolower(static_cast<unsigned char>(*a)) != std::tolower(static_cast<unsigned char>(*b))) {
            return false;
        }
    }
    return *a == *b;
}

}  // namespace

// 0x565c00 _ZN6glitch2io16CStringAttributeC1EPKcPKwb
// The constructor takes the name, keeps the flag, marks the value as wide and stores
// the wide value through setString. A null value is treated as empty (the original
// measures it with wcslen, so null is not a supported input).
CStringAttribute::CStringAttribute(const char* attrName, const wchar_t* value, bool f) {
    name = attrName != nullptr ? attrName : "";
    flag = f;
    isWide = true;
    if (value != nullptr) {
        CStringAttribute::setString(value);
    }
}

// 0x3266a8 (D1): the members are released by their own destructors.

// 0x3260d0 _ZN6glitch2io16CStringAttribute9getStringEv
std::string CStringAttribute::getString() const { return narrowText(*this); }

// 0x325e78 _ZN6glitch2io16CStringAttribute10getStringWEv
std::wstring CStringAttribute::getStringW() const { return wideText(*this); }

// 0x326444 _ZN6glitch2io16CStringAttribute9setStringEPKc
void CStringAttribute::setString(const char* value) {
    if (!isWide) {
        narrow = value != nullptr ? value : "";
        return;
    }
    wide = glitch::core::stringc2stringw(value);
}

// 0x326da4 _ZN6glitch2io16CStringAttribute9setStringEPKw
void CStringAttribute::setString(const wchar_t* value) {
    if (!isWide) {
        narrow = glitch::core::stringw2stringc(value);
        return;
    }
    wide = value != nullptr ? value : L"";
}

// 0x326ec4 _ZN6glitch2io16CStringAttribute6setIntEi
void CStringAttribute::setInt(int value) {
    if (!isWide) {
        narrow = glitch::core::int2stringc(value);
        return;
    }
    wide = glitch::core::int2stringw(value);
}

// 0x3271e4 _ZN6glitch2io16CStringAttribute6getIntEv
int CStringAttribute::getInt() const { return std::atoi(narrowText(*this).c_str()); }

// 0x327290 _ZN6glitch2io16CStringAttribute8getFloatEv
// Parsed with fast_atof_move, which is partial (the scale table is upstream's).
float CStringAttribute::getFloat() const {
    const std::string text = narrowText(*this);
    float result = 0.f;
    glitch::core::fast_atof_move(text.c_str(), result);
    return result;
}

// 0x327798 _ZN6glitch2io16CStringAttribute7getBoolEv
bool CStringAttribute::getBool() const { return equalsIgnoreCase(narrowText(*this).c_str(), "true"); }

// 0x32d3d4 _ZN6glitch2io16CStringAttribute8setFloatEf
// Formatted through an output stream on a double, as the original does. Partial: the
// stream's digit generation is the C++ library's, not the one the original linked.
void CStringAttribute::setFloat(float value) {
    std::ostringstream stream;
    stream << static_cast<double>(value);
    const std::string text = stream.str();
    if (!isWide) {
        narrow = text;
        return;
    }
    wide = glitch::core::stringc2stringw(text.c_str());
}

// ---- CLightAttribute / CTextureAttribute / CBinaryAttribute --------------

// 0x5606a8 _ZN6glitch2io15CLightAttribute8getLightEv
boost::intrusive_ptr<glitch::video::CLight> CLightAttribute::getLight() const { return light; }

// 0x561dd8 _ZN6glitch2io15CLightAttribute9getStringEPc
// The buffer receives "light" (with its terminator) when a light is set, else "".
void CLightAttribute::getString(char* buf) const {
    if (light.get() != nullptr) {
        std::memcpy(buf, "light", 6);
        return;
    }
    buf[0] = '\0';
}

// 0x562ae4 _ZN6glitch2io15CLightAttribute9getStringEv
std::string CLightAttribute::getString() const { return std::string("light"); }

// 0x560638 _ZN6glitch2io17CTextureAttribute10getTextureEv
boost::intrusive_ptr<glitch::video::ITexture> CTextureAttribute::getTexture() const { return texture; }

// 0x560664 _ZN6glitch2io17CTextureAttribute10setTextureERKN5boost13intrusive_ptrINS_5video8ITextureEEE
// The new reference is taken before the old one is dropped (intrusive_ptr assignment).
void CTextureAttribute::setTexture(const boost::intrusive_ptr<glitch::video::ITexture>& value) {
    texture = value;
}

// 0x560620 _ZNK6glitch2io16CBinaryAttribute7getTypeEv
int CBinaryAttribute::getType() const { return 0x19; }
// 0x560628 _ZNK6glitch2io16CBinaryAttribute13getTypeStringEv
// Partial: the wide literal is not resolved in strings.csv; the name is inferred from the class.
const wchar_t* CBinaryAttribute::getTypeString() const { return L"binary"; }

// 0x566ea0 _ZN6glitch2io17CTextureAttributeC1EPKcRKN5boost13intrusive_ptrINS_5video8ITextureEEEPNS6_12IVideoDriverEb
// Partial: as decompiled, the texture argument is not stored (the texture starts null);
// the driver is kept and a reference is taken on it (+0x28).
CTextureAttribute::CTextureAttribute(const char* attrName,
                                     const boost::intrusive_ptr<glitch::video::ITexture>& value,
                                     glitch::video::IVideoDriver* drv, bool f) {
    (void)value;
    name = attrName != nullptr ? attrName : "";
    flag = f;
    driver = drv;
    if (driver != nullptr) ++driver->referenceCounter;
}

// 0x5653c8 _ZN6glitch2io17CTextureAttributeD1Ev
// The driver reference is dropped first; the texture member is released after this body.
CTextureAttribute::~CTextureAttribute() {
    if (driver != nullptr) driver->drop();
}

// 0x5657b0 _ZN6glitch2io22CStringWArrayAttributeC1EPKcSt6vectorIS...
// Partial: as decompiled, the array argument is not stored; the array starts empty.
CStringWArrayAttribute::CStringWArrayAttribute(const char* attrName,
                                               const std::vector<std::wstring>& value, bool f) {
    (void)value;
    name = attrName != nullptr ? attrName : "";
    flag = f;
}

// 0x326f90 _ZN6glitch2io18CVector2DAttribute9getMatrixEv
// The identity matrix with the flag set; the attribute's value is not read.
namespace {

// The identity matrix with the flag set: what the getMatrix bodies that do not read
// their value return.
glitch::core::CMatrix4<float> identityWithFlag() {
    glitch::core::CMatrix4<float> m = detail::identityMatrix();
    m.flag = true;
    return m;
}

}  // namespace

glitch::core::CMatrix4<float> CVector2DAttribute::getMatrix() const { return identityWithFlag(); }

// 0x563c40 _ZN6glitch2io19CVector2DIAttribute9getMatrixEv
glitch::core::CMatrix4<float> CVector2DIAttribute::getMatrix() const { return identityWithFlag(); }

// 0x563d9c _ZN6glitch2io19CVector3DIAttribute9getMatrixEv
glitch::core::CMatrix4<float> CVector3DIAttribute::getMatrix() const { return identityWithFlag(); }

// 0x563c04 _ZN6glitch2io19CVector4DIAttribute9getMatrixEv
glitch::core::CMatrix4<float> CVector4DIAttribute::getMatrix() const { return identityWithFlag(); }

// 0x3273ac _ZN6glitch2io18CVector3DAttribute9getMatrixEv
// A translation matrix: the identity with the three components at M[12..14] and the
// flag clear. The original reads its float buffer without a size check; a short list
// gives zeros here.
glitch::core::CMatrix4<float> CVector3DAttribute::getMatrix() const {
    glitch::core::CMatrix4<float> m = detail::identityMatrix();
    auto component = [this](std::size_t i) { return i < floatValues.size() ? floatValues[i] : 0.f; };
    m.M[12] = component(0);
    m.M[13] = component(1);
    m.M[14] = component(2);
    return m;
}

// 0x5633ec _ZN6glitch2io15CLightAttribute8setLightERKN5boost13intrusive_ptrINS_5video6CLightEEE
void CLightAttribute::setLight(boost::intrusive_ptr<glitch::video::CLight> value) { light = value; }

// 0x56514c _ZN6glitch2io15CLightAttributeD1Ev
// The driver reference is dropped first; the light member is released after this body.
CLightAttribute::~CLightAttribute() {
    if (driver != nullptr) driver->drop();
}

// 0x56377c _ZNK6glitch2io15CLightAttribute10getStringWEv
std::wstring CLightAttribute::getStringW() const { return light ? L"light" : L""; }

}  // namespace glitch::io
