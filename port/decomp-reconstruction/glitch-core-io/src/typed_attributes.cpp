#include "glitch-core-io/io/typed_attributes.hpp"

#include <cstdint>
#include <cstring>
#include <cwchar>
#include <string>
#include <vector>

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

// 0x31f30c _ZNK6glitch2io16CStringAttribute7getTypeEv
int CStringAttribute::getType() const { return 2; }
// 0x31f314 _ZNK6glitch2io16CStringAttribute13getTypeStringEv
const wchar_t* CStringAttribute::getTypeString() const { return L"string"; }

// 0x5606d8 _ZNK6glitch2io15CLightAttribute7getTypeEv
int CLightAttribute::getType() const { return 0x1b; }
// 0x5606c4 _ZN6glitch2io15CLightAttribute7getBoolEv
bool CLightAttribute::getBool() const { return light.get() != nullptr; }
// 0x5606d4 _ZN6glitch2io15CLightAttribute9setStringEPKc
void CLightAttribute::setString(const char*) {}

// 0x560690 _ZNK6glitch2io17CTextureAttribute7getTypeEv
int CTextureAttribute::getType() const { return 0x1a; }
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

}  // namespace glitch::io
