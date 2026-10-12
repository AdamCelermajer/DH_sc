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

// glitch::core::CMatrix4<float>: sixteen values, row-major, then one flag byte at +0x40. The
// parameter code copies 0x41 bytes (the 16 values and the flag). A flag set means identity
// (the parameter setters release the heap matrix in that case). sizeof is 68, the original stride.
// Only the layout is carried here; the constructors and operators are in glitch-core-io.
template <typename T>
struct CMatrix4 {
    T M[16];                    // +0x00
    bool flag = false;          // +0x40
};

}  // namespace glitch::core

namespace glitch::video {

// SColor is one 32-bit packed value in upstream Irrlicht; the original reads
// alpha as the byte at +3, consistent with that layout.
struct SColor {
    uint32_t color = 0;

    // 0x540f8c. Channels are bytes 0..3 of color (byte 0 is the low byte, as in
    // memory). Result is the blend t * this + (1 - t) * other, with t clamped to [0, 1].
    SColor getInterpolated(const SColor& other, float t) const;
};

// Float colour, four words (r, g, b, a) in the original.
struct SColorf {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 0.0f;
};

}  // namespace glitch::video
