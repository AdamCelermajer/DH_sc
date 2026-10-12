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

}  // namespace glitch::core

namespace glitch::video {

// SColor is one 32-bit packed value in upstream Irrlicht; the original reads
// alpha as the byte at +3, consistent with that layout.
struct SColor {
    uint32_t color = 0;
};

}  // namespace glitch::video
