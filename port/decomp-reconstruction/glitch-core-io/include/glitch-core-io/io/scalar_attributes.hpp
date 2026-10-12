// Single-value attribute subclasses of glitch::io::IAttribute: bool, int,
// float, enum and user pointer. Member offsets are from the original object.
#pragma once

#include "glitch-core-io/io/attributes.hpp"

namespace glitch::io {

class CBoolAttribute : public IAttribute {
public:
    bool value = false;  // +0x21

    int getInt() const override;          // 0x31d8f8
    float getFloat() const override;      // 0x31d900
    bool getBool() const override;        // 0x31d914
    void setInt(int v) override;          // 0x31d91c
    void setFloat(float v) override;      // 0x31d92c
    void setBool(bool v) override;        // 0x31d958
    int getType() const override;         // 0x31d960 (returns 3)
    void setString(const char* text) override;  // 0x323220 ("true" only)
};

class CIntAttribute : public IAttribute {
public:
    int value = 0;  // +0x24

    int getInt() const override;          // 0x31d978
    float getFloat() const override;      // 0x31d980
    bool getBool() const override;        // 0x31d990
    void setInt(int v) override;          // 0x31d9a0
    void setFloat(float v) override;      // 0x31d9a8
    int getType() const override;         // 0x31d9c0 (returns 0)
    void setString(const char* text) override;  // 0x320908 (atoi)
};

class CFloatAttribute : public IAttribute {
public:
    float value = 0.f;  // +0x24

    int getInt() const override;          // 0x31d9d8
    float getFloat() const override;      // 0x31d9e8
    bool getBool() const override;        // 0x31d9f0
    void setInt(int v) override;          // 0x31da14
    void setFloat(float v) override;      // 0x31da2c
    int getType() const override;         // 0x31da34 (returns 1)
    const wchar_t* getTypeString() const; // 0x31da3c
};

// Enum attributes are an int attribute plus the selected index at +0x38.
// The +0x24 value is inherited from CIntAttribute (inferred from the vtable
// calls in the original, which go through getInt/setInt).
class CEnumAttribute : public CIntAttribute {
public:
    int enumValue = 0;  // +0x38

    float getFloat() const override;      // 0x5605a4
    bool getBool() const override;        // 0x5605bc
    void setFloat(float v) override;      // 0x5605d8
    int getEnum() const override;         // 0x560600
    int getType() const override;         // 0x560608 (returns 4)
};

class CUserPointerAttribute : public IAttribute {
public:
    void* value = nullptr;  // +0x24

    int getInt() const override;          // 0x560708
    bool getBool() const override;        // 0x560710
    int getType() const override;         // 0x560720 (returns 0x1c)
    void setUserPointer(void* p) override;  // 0x560728
    void* getUserPointer() const;         // 0x560730
    const wchar_t* getTypeString() const; // 0x560738
    void getString(char* out) const;      // 0x561a70
    void setString(const char* text);     // 0x561a88
};

}  // namespace glitch::io
