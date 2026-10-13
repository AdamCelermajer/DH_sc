// SimpleTypeProperty<T>: a Property whose value is a T inside the owning object (at Property::m_offset). The
// default value sits after the base, at +0x20. One instantiation per value type: bool, int, float, std::string,
// glitch::core::vector3d<float>, glitch::core::dimension2d<float>, Point2D<int>, Point3D<float> and
// std::vector<Point3D<float>>. Text conversion goes through StrToObj and ObjToStr for the scalars and the
// glitch types, and through std::string for std::string.
// Original 32-bit size is 0x20 + sizeof(T), rounded up to 4, which is the operator new size in Clone: 0x24 for
// bool, int and float; 0x28 for Point2D<int> and dimension2d<float>; 0x2c for Point3D<float> and
// vector3d<float>; 0x38 for std::string. Each instantiation has a 32-byte vtable: offset-to-top, typeinfo, then
// the six Property slots, all overridden here.
// Reconstructed from libDungeonHunter2.so pseudocode: the std::string constructor 0x33e380, the default
// constructor 0x394f10, and the per-type slots, for example bool 0x33de64..0x33eed8 and std::string
// 0x33e218..0x33f4c4.
#pragma once

#include <string>

#include "dh2/Property.hpp"

template <typename T>
class SimpleTypeProperty : public Property {
public:
    SimpleTypeProperty();                                               // 0x394f10 (std::string): empty name and empty default
    // 0x33e380 (std::string): Property(name, object, field), then m_defaultValue copied from defaultValue.
    SimpleTypeProperty(void* object, const char* name, T* field, T defaultValue);
    // 0x33e244 (std::string): copies the offset, the name and the default value.
    SimpleTypeProperty& operator=(const SimpleTypeProperty& other);

    // Property slots, in vtable order.
    std::string ToString(void* object) override;                        // slot 0x00 (+0x00): value at object + m_offset, as text
    int FromString(void* object, const char* text) override;            // slot 0x04 (+0x04): parses text into the value
    bool IsDefaultValue(void* object) override;                         // slot 0x08 (+0x08): value equals m_defaultValue
    void SetToDefaultValue(void* object) override;                      // slot 0x0c (+0x0c): value = m_defaultValue
    int SetDefaultValueFromString(const char* text) override;           // slot 0x10 (+0x10): parses text into m_defaultValue
    SimpleTypeProperty* Clone() override;                               // slot 0x14 (+0x14): new object with the same offset, name and default

    // Typed default value. Original offset +0x20. Offsets +0x04 (m_offset) and +0x08 (m_name) belong to Property.
    T m_defaultValue;                                                   // +0x20
};
