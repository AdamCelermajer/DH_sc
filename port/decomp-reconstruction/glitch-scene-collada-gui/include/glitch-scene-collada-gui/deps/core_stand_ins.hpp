// Minimal glitch::core and glitch::video value types used by the GUI accessors.
// Field order matches the original; there are no vtables and no methods.
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
    T X{};
    T Y{};
};

template <typename T>
struct vector3d {
    T X{};
    T Y{};
    T Z{};
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

// One packed 32-bit colour word; the original reads alpha from the byte at +3.
struct SColor {
    std::uint32_t color = 0;
};

// Four float channels (r, g, b, a) as the original passes them by reference.
struct SColorf {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 0.0f;
};

}  // namespace glitch::video
