// Typed attribute classes. Most only expose their type code (and, where the
// literal resolved, their type name). Their value accessors are not
// reconstructed yet, except where noted in the .cpp.
#pragma once

#include <string>
#include <vector>

#include "glitch-core-io/io/attributes.hpp"
#include "glitch-core-io/io/numbers_attribute.hpp"

namespace glitch::io {

class CColorAttribute : public CNumbersAttribute {
public:
    int getType() const override;  // 0x31f2ac
};

class CColorfAttribute : public CNumbersAttribute {
public:
    int getType() const override;  // 0x31f114
    const wchar_t* getTypeString() const;  // 0x31f11c
};

class CVector2DAttribute : public CNumbersAttribute {
public:
    int getType() const override;  // 0x31f2c4
    const wchar_t* getTypeString() const;  // 0x31f2cc
};

class CVector3DAttribute : public CNumbersAttribute {
public:
    int getType() const override;  // 0x31f2dc
    const wchar_t* getTypeString() const;  // 0x31f2e4
};

class CVector4DAttribute : public CNumbersAttribute {
public:
    int getType() const override;  // 0x31f2f4
    const wchar_t* getTypeString() const;  // 0x31f2fc
};

class CVector2DIAttribute : public IAttribute {
public:
    int getType() const override;  // 0x560484
    const wchar_t* getTypeString() const;  // 0x56048c
};

class CVector3DIAttribute : public IAttribute {
public:
    int getType() const override;  // 0x56049c
    const wchar_t* getTypeString() const;  // 0x5604a4
};

class CVector4DIAttribute : public IAttribute {
public:
    int getType() const override;  // 0x5604b4
    const wchar_t* getTypeString() const;  // 0x5604bc
};

class CPosition2DAttribute : public IAttribute {
public:
    int getType() const override;  // 0x5604cc
};

class CRectAttribute : public IAttribute {
public:
    int getType() const override;  // 0x5604e4
};

class CMatrixAttribute : public IAttribute {
public:
    int getType() const override;  // 0x5604fc
};

class CQuaternionAttribute : public IAttribute {
public:
    int getType() const override;  // 0x560514
    const wchar_t* getTypeString() const;  // 0x56051c
};

class CBBoxAttribute : public IAttribute {
public:
    int getType() const override;  // 0x56052c
};

class CPlaneAttribute : public IAttribute {
public:
    int getType() const override;  // 0x560544
};

class CTriangleAttribute : public IAttribute {
public:
    int getType() const override;  // 0x56055c
    const wchar_t* getTypeString() const;  // 0x560564
};

class CLine2dAttribute : public IAttribute {
public:
    int getType() const override;  // 0x560574
};

class CLine3dAttribute : public IAttribute {
public:
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
