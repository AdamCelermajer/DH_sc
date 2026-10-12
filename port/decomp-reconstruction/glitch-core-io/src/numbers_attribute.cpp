#include "glitch-core-io/io/numbers_attribute.hpp"

namespace glitch::io {

// 0x31da4c _ZN6glitch2io17CNumbersAttribute6getIntEv
int CNumbersAttribute::getInt() const {
    if (count == 0) return 0;
    if (!isFloatList) return intValues[0];
    return static_cast<int>(floatValues[0]);  // __aeabi_f2iz: truncate toward zero
}

// 0x31da8c _ZN6glitch2io17CNumbersAttribute8getFloatEv
float CNumbersAttribute::getFloat() const {
    if (count == 0) return 0.f;
    if (!isFloatList) return static_cast<float>(intValues[0]);
    return floatValues[0];
}

// 0x31dacc _ZN6glitch2io17CNumbersAttribute7getBoolEv
bool CNumbersAttribute::getBool() const {
    for (std::size_t i = 0; i < count; ++i) {
        if (!isFloatList) {
            if (intValues[i] != 0) return true;
        } else if (floatValues[i] != 0.f) {
            return true;
        }
    }
    return false;
}

// 0x31ee4c _ZN6glitch2io17CNumbersAttribute12isNumberListEv
bool CNumbersAttribute::isNumberList() { return true; }

// 0x31ee54 _ZN6glitch2io17CNumbersAttribute7isFloatEv
bool CNumbersAttribute::isFloat() const { return isFloatList; }

// 0x31ee5c _ZNK6glitch2io17CNumbersAttribute7getTypeEv
int CNumbersAttribute::getType() const { return isFloatList ? 0x17 : 0x18; }

// 0x31ee70 _ZNK6glitch2io17CNumbersAttribute13getTypeStringEv
const wchar_t* CNumbersAttribute::getTypeString() const {
    return isFloatList ? L"floatlist" : L"intlist";
}

// 0x31ed50 _ZN6glitch2io17CNumbersAttribute6setIntEi
// Writes the value into every element, converting to float for float lists.
void CNumbersAttribute::setInt(int value) {
    for (std::size_t i = 0; i < count; ++i) {
        if (!isFloatList) {
            intValues[i] = value;
        } else {
            floatValues[i] = static_cast<float>(value);
        }
    }
}

// 0x31edc4 _ZN6glitch2io17CNumbersAttribute8setFloatEf
// Writes the value into every element, truncating for int lists.
void CNumbersAttribute::setFloat(float value) {
    for (std::size_t i = 0; i < count; ++i) {
        if (!isFloatList) {
            intValues[i] = static_cast<int>(value);
        } else {
            floatValues[i] = value;
        }
    }
}

// 0x31ee38 _ZN6glitch2io17CNumbersAttribute7setBoolEb
// The original tail-calls the virtual setInt (vtable +0x88) with the bool.
void CNumbersAttribute::setBool(bool value) { setInt(value ? 1 : 0); }


// 0x567340 _ZN6glitch2io17CNumbersAttributeC2EPKcNS_4core4rectIiEEb
CNumbersAttribute::CNumbersAttribute(const char* attrName, const glitch::core::rect<std::int32_t>& value, bool f)
    : intValues{value.X1, value.Y1, value.X2, value.Y2}, count(4), isFloatList(false) {
    name = attrName;
    flag = f;
}

// 0x565880 _ZN6glitch2io17CNumbersAttributeC2EPKcRKNS_4core8vector2dIfEEb
CNumbersAttribute::CNumbersAttribute(const char* attrName, const glitch::core::vector2d<float>& value, bool f)
    : floatValues{value.X, value.Y}, count(2), isFloatList(true) {
    name = attrName;
    flag = f;
}

// 0x567968 _ZN6glitch2io17CNumbersAttributeC2EPKcRKNS_4core8vector2dIiEEb
CNumbersAttribute::CNumbersAttribute(const char* attrName, const glitch::core::vector2d<std::int32_t>& value, bool f)
    : intValues{value.X, value.Y}, count(2), isFloatList(false) {
    name = attrName;
    flag = f;
}

// 0x565a54 _ZN6glitch2io17CNumbersAttributeC2EPKcRKNS_5video7SColorfEb
CNumbersAttribute::CNumbersAttribute(const char* attrName, const glitch::video::SColorf& value, bool f)
    : floatValues{value.r, value.g, value.b, value.a}, count(4), isFloatList(true) {
    name = attrName;
    flag = f;
}

// 0x567b3c _ZN6glitch2io17CNumbersAttributeC2EPKcNS_4core6line3dIfEEb
CNumbersAttribute::CNumbersAttribute(const char* attrName, const glitch::core::line3d<float>& value, bool f)
    : floatValues{value.start.X, value.start.Y, value.start.Z, value.end.X, value.end.Y, value.end.Z},
      count(6), isFloatList(true) {
    name = attrName;
    flag = f;
}

}  // namespace glitch::io
