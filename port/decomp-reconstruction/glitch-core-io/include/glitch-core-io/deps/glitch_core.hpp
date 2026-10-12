// Minimal stand-ins for glitch::core and glitch::video types used by the io
// attribute code. Layout matches the original: plain members, no vtable.
#pragma once

#include <cstdint>

namespace glitch {
// Reference-counted base; the counter is at +0x04 in the original.
class IReferenceCounted {
public:
    virtual ~IReferenceCounted() = default;
    // Called by drop() right before the object is deleted (vtable +0x08 in the original).
    virtual void onDelete() {}
    // Decrements the count; on zero calls onDelete() and deletes the object.
    // Returns true when the object was deleted. 0x31d584 _ZNK6glitch17IReferenceCounted4dropEv
    bool drop() const;
    mutable int referenceCounter = 1;  // +0x04
};
}  // namespace glitch

namespace glitch::core {
template <typename T>
struct vector2d {
    T X, Y;
};
template <typename T>
struct vector3d {
    T X, Y, Z;
};
template <typename T>
struct vector4d {
    T X, Y, Z, W;
};
template <typename T>
struct position2d {
    T X, Y;
};
template <typename T>
struct rect {
    T X1, Y1, X2, Y2;  // upper-left and lower-right corners
};
}  // namespace glitch::core

namespace glitch::video {
struct SColor {
    std::uint32_t color;
};
struct SColorf {
    float r, g, b, a;
};
}  // namespace glitch::video
