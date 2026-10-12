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
    CNumbersAttribute(const char* attrName, const glitch::core::line2d<float>& value, bool f);           // 0x565ce8
    CNumbersAttribute(const char* attrName, const glitch::core::triangle3d<float>& value, bool f);       // 0x565e94
    CNumbersAttribute(const char* attrName, const glitch::core::plane3d<float>& value, bool f);          // 0x56616c
    CNumbersAttribute(const char* attrName, const glitch::core::aabbox3d<float>& value, bool f);         // 0x566318
    CNumbersAttribute(const char* attrName, const glitch::core::quaternion& value, bool f);              // 0x56653c
    CNumbersAttribute(const char* attrName, const glitch::core::CMatrix4<float>& value, bool f);         // 0x566728
    CNumbersAttribute(const char* attrName, const glitch::core::vector4d<float>& value, bool f);         // 0x5668ac
    CNumbersAttribute(const char* attrName, const glitch::core::vector3d<float>& value, bool f);         // 0x566a84
    CNumbersAttribute(const char* attrName, const glitch::core::position2d<std::int32_t>& value, bool f);  // 0x5674ec
    CNumbersAttribute(const char* attrName, const glitch::core::vector4d<std::int32_t>& value, bool f);  // 0x567620
    CNumbersAttribute(const char* attrName, const glitch::core::vector3d<std::int32_t>& value, bool f);  // 0x5677f8
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
    glitch::core::rect<std::int32_t> getRect() const override;        // 0x31e120
    void setRect(glitch::core::rect<std::int32_t> value);             // 0x3247b8 (named setRect in the original)
    glitch::core::vector2d<float> getVector2d() const override;       // 0x31dbf8
    void setVector2d(glitch::core::vector2d<float> value) override;   // 0x324d04
    void setVector2d(glitch::core::vector2d<std::int32_t> value) override;  // 0x3251d0
    glitch::core::position2d<std::int32_t> getPosition() const override;     // 0x31db5c
    void setPosition(glitch::core::position2d<std::int32_t> value) override; // 0x32490c
    glitch::core::vector3d<float> getVector3d() const override;       // 0x31dcb0
    void setVector3d(const glitch::core::vector3d<float>& value) override;   // 0x324df4
    void setDimension2d(glitch::core::dimension2d<std::int32_t> value) override;  // 0x325830
    glitch::video::SColorf getColorf() const override;                // 0x31defc
    glitch::core::quaternion getQuaternion() const override;          // 0x31e25c
    void setQuaternion(const glitch::core::quaternion& value) override;  // 0x32465c

protected:
    // Element access on the active list (float or int), with the original's conversions:
    // int reads of a float list truncate, float reads of an int list convert.
    float activeFloat(std::size_t i) const;
    std::int32_t activeInt(std::size_t i) const;
    void setActive(std::size_t i, float value);
    void setActive(std::size_t i, std::int32_t value);
    // Zeroes the first count elements of the active list.
    void clearActive();

public:
    const wchar_t* getTypeString() const;              // 0x31ee70

    void setInt(int value) override;                   // 0x31ed50
    void setFloat(float value) override;               // 0x31edc4
    void setBool(bool value) override;                 // 0x31ee38
};

}  // namespace glitch::io
