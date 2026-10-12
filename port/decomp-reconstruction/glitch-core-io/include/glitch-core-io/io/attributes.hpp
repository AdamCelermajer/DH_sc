// glitch::io::IAttribute (interface subset) and glitch::io::CAttributes
// (indexed attribute container). Reconstructed from libDungeonHunter2.so.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "glitch-core-io/deps/glitch_core.hpp"

namespace glitch::io {

// Interface subset. Virtual slot numbers from the vtable are noted; the base
// implementations of the setters reconstructed so far are empty.
class IAttribute {
public:
    // +0x1c: attribute name (glitch stores it as a C string pointer).
    const char* name = nullptr;

    virtual ~IAttribute() = default;

    virtual int getInt() const { return 0; }                              // +0x0c
    virtual glitch::video::SColorf getColorf() const { return {1.f, 1.f, 1.f, 1.f}; }  // +0x14
    virtual glitch::core::vector3d<float> getVector3d() const { return {}; }            // +0x30
    virtual glitch::core::vector4d<float> getVector4d() const { return {}; }            // +0x34
    virtual glitch::core::position2d<std::int32_t> getPosition() const { return {}; }   // +0x38
    virtual glitch::core::rect<std::int32_t> getRect() const { return {}; }             // +0x3c
    virtual glitch::core::vector2d<float> getVector2d() const { return {}; }            // +0x4c
    virtual glitch::core::vector2d<std::int32_t> getVector2di() const { return {}; }    // +0x50
    virtual glitch::core::vector3d<std::int32_t> getVector3di() const { return {}; }    // +0x54
    virtual glitch::core::vector4d<std::int32_t> getVector4di() const { return {}; }    // +0x58
    virtual glitch::video::SColor getColor() const { return {0xffffffffu}; }  // 0x31d5f8
    virtual float getFloat() const { return 0.f; }                        // +0x10
    virtual bool getBool() const { return false; }                        // +0x28
    virtual void getBinary(void*, int) const {}                           // +0x2c
    virtual int getEnum() const { return 0; }                             // +0x80
    virtual void* getUserPointer() const { return nullptr; }             // 0x31d81c
    virtual void setInt(int) {}                                           // +0x88
    virtual void setFloat(float) {}                                       // +0x8c
    virtual void setString(const char*) {}                                // +0x90
    virtual std::vector<std::wstring> getArray() const { return {}; }      // +0x24
    virtual void setArray(const std::vector<std::wstring>&) {}            // +0x98
    virtual void setColor(glitch::video::SColorf) {}                      // +0x9c
    virtual void setColor(glitch::video::SColor) {}                       // +0xa0
    virtual void setBool(bool) {}                                         // +0xa4
    virtual void setBinary(void*, int) {}                                 // +0xa8
    virtual void setVector2d(glitch::core::vector2d<float>) {}            // +0xac
    virtual void setVector3d(const glitch::core::vector3d<float>&) {}     // +0xb0
    virtual void setVector4d(const glitch::core::vector4d<float>&) {}     // +0xb4
    virtual void setPosition(glitch::core::position2d<std::int32_t>) {}   // +0xb8
    virtual void setVector2di(glitch::core::vector2d<std::int32_t>) {}    // +0xcc
    virtual void setVector3di(const glitch::core::vector3d<std::int32_t>&) {}  // +0xd0
    virtual void setVector4di(const glitch::core::vector4d<std::int32_t>&) {}  // +0xd4
    virtual void setUserPointer(void*) {}                                 // +0xf4
    virtual void setEnum(const char*, const char* const*) {}              // +0xf8
    virtual int getType() const { return 0; }                             // +0x104
    // Base defaults from the original bodies (0x31d690 .. 0x31d7dc).
    virtual glitch::core::quaternion getQuaternion() const { return {0.f, 0.f, 0.f, 1.f}; }  // 0x31d690
    virtual glitch::core::triangle3d<float> getTriangle() const { return {}; }               // 0x31d6ac
    virtual glitch::core::line2d<float> getLine2d() const { return {{0.f, 0.f}, {1.f, 1.f}}; }   // 0x31d724
    virtual glitch::core::line2d<std::int32_t> getLine2di() const { return {{0, 0}, {1, 1}}; }  // 0x31d740
    virtual glitch::core::line3d<float> getLine3d() const { return {{0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}}; }  // 0x31d75c
    virtual glitch::core::line3d<std::int32_t> getLine3di() const { return {{0, 0, 0}, {1, 1, 1}}; }     // 0x31d780
    virtual glitch::core::dimension2d<std::int32_t> getDimension2d() const { return {}; }    // 0x31d7a4
    virtual glitch::core::aabbox3d<float> getBBox() const { return {{-1.f, -1.f, -1.f}, {1.f, 1.f, 1.f}}; }  // 0x31d7b4
    virtual glitch::core::plane3d<float> getPlane() const { return {{0.f, 1.f, 0.f}, -0.f}; }  // 0x31d7dc
    virtual void setQuaternion(const glitch::core::quaternion&) {}                          // 0x31d870
    virtual void setTriangle(const glitch::core::triangle3d<float>&) {}                     // 0x31d888
    virtual void setLine2d(glitch::core::line2d<float>) {}                                  // 0x31d898
    virtual void setLine2d(glitch::core::line2d<std::int32_t>) {}                           // 0x31d89c
    virtual void setLine3d(glitch::core::line3d<float>) {}                                  // 0x31d8a0
    virtual void setLine3d(glitch::core::line3d<std::int32_t>) {}                           // 0x31d8a4
    virtual void setDimension2d(glitch::core::dimension2d<std::int32_t>) {}                 // 0x31d8a8
    virtual void setBBox(glitch::core::aabbox3d<float>) {}                                  // 0x31d8b4
    virtual void setPlane(glitch::core::plane3d<float>) {}                                  // 0x31d8b8
    virtual void setString(const wchar_t*) {}                                               // +0x94
    virtual void setRect(glitch::core::rect<std::int32_t>) {}                               // 0x31d86c
};

class CAttributes {
public:
    // +0x48: the attribute list (begin/end pointers in the original).
    std::vector<IAttribute*> attributes;

    int getAttributeCount() const;                    // 0x5607e4
    const char* getAttributeName(int index) const;    // 0x5607fc
    int getAttributeType(int index) const;            // 0x560820
    bool getBool(int index) const;                    // 0x5608a0
    int getInt(int index) const;                      // 0x5608dc
    float getFloat(int index) const;                  // 0x560918
    int getEnum(int index) const;                     // 0x560d1c
    void getBinaryData(int index, void* data, int size) const;  // 0x560ce4

    glitch::video::SColorf getColorf(int index) const;                         // 0x5609e8
    glitch::core::vector2d<float> getVector2d(int index) const;                // 0x560a44
    glitch::core::vector3d<float> getVector3d(int index) const;                // 0x560a94
    glitch::core::vector4d<float> getVector4d(int index) const;                // 0x560ae8
    glitch::core::vector2d<std::int32_t> getVector2di(int index) const;        // 0x560b40
    glitch::core::vector3d<std::int32_t> getVector3di(int index) const;        // 0x560b90
    glitch::core::vector4d<std::int32_t> getVector4di(int index) const;        // 0x560be4
    glitch::core::position2d<std::int32_t> getPosition2d(int index) const;     // 0x560c3c
    glitch::core::rect<std::int32_t> getRect(int index) const;                 // 0x560c8c
    void setAttribute(int index, glitch::video::SColorf value);                // 0x560f0c

    void setAttribute(int index, const char* value);                       // 0x560db0
    void setAttribute(int index, bool value);                              // 0x560e20
    void setAttribute(int index, int value);                               // 0x560e58
    void setAttribute(int index, float value);                             // 0x560e90
    void setAttribute(int index, glitch::video::SColor value);             // 0x560ec8
    void setAttribute(int index, const glitch::core::vector2d<float>& value);  // 0x560f6c
    void setAttribute(int index, const glitch::core::vector3d<float>& value);  // 0x560fb8
    void setAttribute(int index, const glitch::core::vector4d<float>& value);  // 0x560ff0
    void setAttribute(int index, const glitch::core::vector2d<std::int32_t>& value);  // 0x561028
    void setAttribute(int index, const glitch::core::vector3d<std::int32_t>& value);  // 0x56106c
    void setAttribute(int index, const glitch::core::vector4d<std::int32_t>& value);  // 0x5610a4
    void setAttribute(int index, glitch::core::position2d<std::int32_t> value);       // 0x5610dc
    void setAttribute(int index, void* data, int size);                    // 0x56117c
    void setAttribute(int index, const char* value, const char* const* choices);  // 0x5611b4
    void setAttribute(int index, void* userPointer);                        // 0x56189c
    void setAttribute(int index, const wchar_t* value);                     // 0x560de8
    void setAttribute(int index, glitch::core::rect<std::int32_t> value);  // 0x561120
    void setAttribute(int index, glitch::core::quaternion value);          // 0x5612ac
    void setAttribute(int index, const glitch::core::aabbox3d<float>& value);      // 0x5613b0
    void setAttribute(int index, const glitch::core::plane3d<float>& value);       // 0x5614b0
    void setAttribute(int index, const glitch::core::triangle3d<float>& value);    // 0x5615d4
    void setAttribute(int index, const glitch::core::line2d<float>& value);        // 0x5616e4
    void setAttribute(int index, const glitch::core::line3d<float>& value);        // 0x5617e4
    glitch::video::SColor getColor(int index) const;                           // 0x560954
    glitch::core::quaternion getQuaternion(int index) const;                   // 0x561224
    glitch::core::aabbox3d<float> getBox3d(int index) const;                   // 0x561314
    glitch::core::plane3d<float> getPlane3d(int index) const;                  // 0x561424
    glitch::core::triangle3d<float> getTriangle3d(int index) const;            // 0x561514
    glitch::core::line2d<float> getLine2d(int index) const;                    // 0x561660
    glitch::core::line3d<float> getLine3d(int index) const;                    // 0x561748
    void* getUserPointer(int index) const;                                     // 0x561858
};

}  // namespace glitch::io
