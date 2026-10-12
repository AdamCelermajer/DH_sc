// Layouts that more than one glitch category needs: the glitch::core value types and
// the glitch::video colour words. Field order and widths match the original (32-bit ARM),
// and there are no vtables. Members are plain so one definition serves every category.
// Methods stay with the category that owns them. Include this instead of copying a
// stand-in into a category's deps/ folder.
#pragma once

#include <cstdint>

namespace glitch::core {

template <typename T>
struct dimension2d {
    T Width{};   // +0x00
    T Height{};  // +0x04
};

template <typename T>
struct position2d {
    T X{};  // +0x00
    T Y{};  // +0x04
};

template <typename T>
struct vector3d {
    T X{};
    T Y{};
    T Z{};
};

// Four words in the original; copied as a block by the parameter code.
template <typename T>
struct vector4d {
    T X{};
    T Y{};
    T Z{};
    T W{};
};

template <typename T>
struct aabbox3d {
    vector3d<T> MinEdge;
    vector3d<T> MaxEdge;
};

template <typename T>
struct rect {
    position2d<T> UpperLeftCorner;
    position2d<T> LowerRightCorner;
};

}  // namespace glitch::core

namespace glitch::video {

// One packed 32-bit colour word. The original reads alpha from the byte at +3.
struct SColor {
    std::uint32_t color = 0;

    // Defined in glitch-video/src/s_color.cpp (0x540f8c).
    SColor getInterpolated(const SColor& other, float t) const;
};

// Four float channels (r, g, b, a), passed by reference in the original.
struct SColorf {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 0.0f;
};

}  // namespace glitch::video
