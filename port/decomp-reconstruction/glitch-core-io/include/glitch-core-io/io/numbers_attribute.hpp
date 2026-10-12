// glitch::io::CNumbersAttribute: an attribute holding a list of numbers, stored
// either as ints or as floats (the "intlist" / "floatlist" types).
#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "glitch-core-io/io/attributes.hpp"

namespace glitch::io {

class CNumbersAttribute : public IAttribute {
public:
    CNumbersAttribute() = default;
    // The constructors store the components in declaration order (ARM listings of
    // the original). The list kind and element count are set as the original does.
    CNumbersAttribute(const char* attrName, const glitch::core::rect<std::int32_t>& value, bool f);      // 0x567340
    CNumbersAttribute(const char* attrName, const glitch::core::vector2d<float>& value, bool f);         // 0x565880
    CNumbersAttribute(const char* attrName, const glitch::core::vector2d<std::int32_t>& value, bool f);  // 0x567968
    CNumbersAttribute(const char* attrName, const glitch::video::SColorf& value, bool f);                // 0x565a54
    CNumbersAttribute(const char* attrName, const glitch::core::line3d<float>& value, bool f);           // 0x567b3c
    // +0x24: int storage (valid when !isFloatList)
    std::vector<int> intValues;
    // +0x30: float storage (valid when isFloatList)
    std::vector<float> floatValues;
    // +0x3c: element count, equal to the size of the active vector.
    std::size_t count = 0;
    // +0x40: true for a float list.
    bool isFloatList = false;

    // Returns the first element as an int, or 0 when the list is empty.
    int getInt() const override;                       // 0x31da4c
    float getFloat() const override;                   // 0x31da8c
    // True if any element is non-zero (exact compare for float lists).
    bool getBool() const override;                     // 0x31dacc
    static bool isNumberList();                        // 0x31ee4c
    bool isFloat() const;                              // 0x31ee54
    int getType() const override;                      // 0x31ee5c
    const wchar_t* getTypeString() const;              // 0x31ee70

    void setInt(int value) override;                   // 0x31ed50
    void setFloat(float value) override;               // 0x31edc4
    void setBool(bool value) override;                 // 0x31ee38
};

}  // namespace glitch::io
