// Typed attribute classes. Most only expose their type code (and, where the
// literal resolved, their type name). Their value accessors are not
// reconstructed yet, except where noted in the .cpp.
#pragma once

#include <string>
#include <vector>

#include "glitch-core-io/io/attributes.hpp"
#include "glitch-core-io/io/numbers_attribute.hpp"

namespace glitch::io {

// CColorAttribute and CColorfAttribute share the same integer view of the colour
// (the ARM bodies are identical). getInt returns the packed colour rotated left by
// one byte (the original stores the bytes in a stack word, then reads it back
// shifted); setInt is its inverse and writes four floats byte/255 into the float list.
class CColorAttribute : public CNumbersAttribute {
public:
    CColorAttribute() = default;
    CColorAttribute(const char* attrName, const glitch::video::SColorf& value, bool f)
        : CNumbersAttribute(attrName, value, f) {}
    int getType() const override;  // 0x31f2ac
    const wchar_t* getTypeString() const override;  // 0x31f2b4
    int getInt() const override;          // 0x31f12c
    float getFloat() const override;      // 0x31f18c
    void setInt(int value) override;      // 0x31f1f0
    void setFloat(float value) override;  // 0x31f284
};

class CColorfAttribute : public CNumbersAttribute {
public:
    CColorfAttribute() = default;
    CColorfAttribute(const char* attrName, const glitch::video::SColorf& value, bool f)
        : CNumbersAttribute(attrName, value, f) {}
    int getType() const override;  // 0x31f114
    const wchar_t* getTypeString() const;  // 0x31f11c
    int getInt() const override;          // 0x31ef94
    float getFloat() const override;      // 0x31eff4
    void setInt(int value) override;      // 0x31f058
    void setFloat(float value) override;  // 0x31f0ec
};

class CVector2DAttribute : public CNumbersAttribute {
public:
    CVector2DAttribute() = default;
    CVector2DAttribute(const char* attrName, const glitch::core::vector2d<float>& value, bool f)
        : CNumbersAttribute(attrName, value, f) {}
    int getType() const override;  // 0x31f2c4
    const wchar_t* getTypeString() const;  // 0x31f2cc
};

class CVector3DAttribute : public CNumbersAttribute {
public:
    CVector3DAttribute() = default;
    CVector3DAttribute(const char* attrName, const glitch::core::vector3d<float>& value, bool f)
        : CNumbersAttribute(attrName, value, f) {}
    int getType() const override;  // 0x31f2dc
    const wchar_t* getTypeString() const;  // 0x31f2e4
};

class CVector4DAttribute : public CNumbersAttribute {
public:
    CVector4DAttribute() = default;
    CVector4DAttribute(const char* attrName, const glitch::core::vector4d<float>& value, bool f)
        : CNumbersAttribute(attrName, value, f) {}
    int getType() const override;  // 0x31f2f4
    const wchar_t* getTypeString() const;  // 0x31f2fc
};

class CVector2DIAttribute : public CNumbersAttribute {
public:
    CVector2DIAttribute() = default;
    CVector2DIAttribute(const char* attrName, const glitch::core::vector2d<std::int32_t>& value, bool f)
        : CNumbersAttribute(attrName, value, f) {}
    int getType() const override;  // 0x560484
    const wchar_t* getTypeString() const;  // 0x56048c
};

class CVector3DIAttribute : public CNumbersAttribute {
public:
    CVector3DIAttribute() = default;
    CVector3DIAttribute(const char* attrName, const glitch::core::vector3d<std::int32_t>& value, bool f)
        : CNumbersAttribute(attrName, value, f) {}
    int getType() const override;  // 0x56049c
    const wchar_t* getTypeString() const;  // 0x5604a4
};

class CVector4DIAttribute : public CNumbersAttribute {
public:
    CVector4DIAttribute() = default;
    CVector4DIAttribute(const char* attrName, const glitch::core::vector4d<std::int32_t>& value, bool f)
        : CNumbersAttribute(attrName, value, f) {}
    int getType() const override;  // 0x5604b4
    const wchar_t* getTypeString() const;  // 0x5604bc
};

class CPosition2DAttribute : public CNumbersAttribute {
public:
    CPosition2DAttribute() = default;
    CPosition2DAttribute(const char* attrName, glitch::core::position2d<std::int32_t> value, bool f)
        : CNumbersAttribute(attrName, value, f) {}
    int getType() const override;  // 0x5604cc
};

// The rect, matrix, quaternion, box, plane, triangle, line and 2D/3D/4D integer
// vector attributes share the CNumbersAttribute layout (their vtables inherit the
// CNumbersAttribute slots; addRect builds a CNumbersAttribute and sets this vtable).
class CRectAttribute : public CNumbersAttribute {
public:
    CRectAttribute() = default;
    CRectAttribute(const char* attrName, const glitch::core::rect<std::int32_t>& value, bool f)
        : CNumbersAttribute(attrName, value, f) {}
    int getType() const override;  // 0x5604e4
};

class CMatrixAttribute : public CNumbersAttribute {
public:
    CMatrixAttribute() = default;
    CMatrixAttribute(const char* attrName, const glitch::core::CMatrix4<float>& value, bool f)
        : CNumbersAttribute(attrName, value, f) {}
    int getType() const override;  // 0x5604fc
    const wchar_t* getTypeString() const override;  // 0x560504
};

class CQuaternionAttribute : public CNumbersAttribute {
public:
    CQuaternionAttribute() = default;
    CQuaternionAttribute(const char* attrName, const glitch::core::quaternion& value, bool f)
        : CNumbersAttribute(attrName, value, f) {}
    int getType() const override;  // 0x560514
    const wchar_t* getTypeString() const;  // 0x56051c
};

class CBBoxAttribute : public CNumbersAttribute {
public:
    CBBoxAttribute() = default;
    CBBoxAttribute(const char* attrName, const glitch::core::aabbox3d<float>& value, bool f)
        : CNumbersAttribute(attrName, value, f) {}
    int getType() const override;  // 0x56052c
};

class CPlaneAttribute : public CNumbersAttribute {
public:
    CPlaneAttribute() = default;
    CPlaneAttribute(const char* attrName, const glitch::core::plane3d<float>& value, bool f)
        : CNumbersAttribute(attrName, value, f) {}
    int getType() const override;  // 0x560544
};

class CTriangleAttribute : public CNumbersAttribute {
public:
    CTriangleAttribute() = default;
    CTriangleAttribute(const char* attrName, const glitch::core::triangle3d<float>& value, bool f)
        : CNumbersAttribute(attrName, value, f) {}
    int getType() const override;  // 0x56055c
    const wchar_t* getTypeString() const;  // 0x560564
};

class CLine2dAttribute : public CNumbersAttribute {
public:
    CLine2dAttribute() = default;
    CLine2dAttribute(const char* attrName, const glitch::core::line2d<float>& value, bool f)
        : CNumbersAttribute(attrName, value, f) {}
    int getType() const override;  // 0x560574
};

class CLine3dAttribute : public CNumbersAttribute {
public:
    CLine3dAttribute() = default;
    CLine3dAttribute(const char* attrName, const glitch::core::line3d<float>& value, bool f)
        : CNumbersAttribute(attrName, value, f) {}
    int getType() const override;  // 0x56058c
};

class CStringWArrayAttribute : public IAttribute {
public:
    std::vector<std::wstring> array;  // +0x24
    std::vector<std::wstring> getArray() const override;  // 0x562f10
    void setArray(const std::vector<std::wstring>& value) override;  // 0x5632a4
    int getType() const override;  // 0x5606f0
};

class CStringAttribute : public IAttribute {
public:
    int getType() const override;  // 0x31f30c
    const wchar_t* getTypeString() const;  // 0x31f314
};

class CLightAttribute : public IAttribute {
public:
    void* light = nullptr;  // +0x24 (boost::intrusive_ptr<CLight> in the original)
    bool getBool() const override;  // 0x5606c4
    void setString(const char*) override;  // 0x5606d4 (no-op)
    int getType() const override;  // 0x5606d8
};

class CTextureAttribute : public IAttribute {
public:
    void* texture = nullptr;  // +0x24 (boost::intrusive_ptr<ITexture> in the original)
    bool getBool() const override;  // 0x560654
    int getType() const override;  // 0x560690
};


}  // namespace glitch::io
