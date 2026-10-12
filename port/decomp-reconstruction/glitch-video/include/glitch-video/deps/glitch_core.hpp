// Minimal stand-ins for glitch core types used by the glitch::video pieces.
// Field order and widths match the original; no vtables.
#pragma once

#include <cstdint>

namespace glitch::core {

template <typename T>
struct dimension2d {
    T Width{};
    T Height{};
};

template <typename T>
struct position2d {
    T X{};
    T Y{};
};

template <typename T>
struct rect {
    position2d<T> UpperLeftCorner;
    position2d<T> LowerRightCorner;
};

// Four words in the original; copied as a block by the parameter code.
template <typename T>
struct vector4d {
    T X{};
    T Y{};
    T Z{};
    T W{};
};

}  // namespace glitch::core

namespace glitch::video {

// SColor is one 32-bit packed value in upstream Irrlicht; the original reads
// alpha as the byte at +3, consistent with that layout.
struct SColor {
    uint32_t color = 0;
};

// Float colour, four words (r, g, b, a) in the original.
struct SColorf {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 0.0f;
};

}  // namespace glitch::video
