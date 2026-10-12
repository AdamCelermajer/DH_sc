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
    virtual glitch::video::SColorf getColorf() const { return {0.f, 0.f, 0.f, 1.f}; }  // +0x14
    virtual glitch::core::vector3d<float> getVector3d() const { return {}; }            // +0x30
    virtual glitch::core::vector4d<float> getVector4d() const { return {}; }            // +0x34
    virtual glitch::core::position2d<std::int32_t> getPosition() const { return {}; }   // +0x38
    virtual glitch::core::rect<std::int32_t> getRect() const { return {}; }             // +0x3c
    virtual glitch::core::vector2d<float> getVector2d() const { return {}; }            // +0x4c
    virtual glitch::core::vector2d<std::int32_t> getVector2di() const { return {}; }    // +0x50
    virtual glitch::core::vector3d<std::int32_t> getVector3di() const { return {}; }    // +0x54
    virtual glitch::core::vector4d<std::int32_t> getVector4di() const { return {}; }    // +0x58
    virtual float getFloat() const { return 0.f; }                        // +0x10
    virtual bool getBool() const { return false; }                        // +0x28
    virtual void getBinary(void*, int) const {}                           // +0x2c
    virtual int getEnum() const { return 0; }                             // +0x80
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
};

}  // namespace glitch::io
