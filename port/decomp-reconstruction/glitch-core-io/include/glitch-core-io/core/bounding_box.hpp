// glitch::core::computeBoundingBox: the box of a strided array of components. The first
// element starts the box, the rest widen it. Instantiated for float (0x6a11bc) and the integer
// component types (signed and unsigned char, short, unsigned short, int and unsigned int). Each
// instance writes an aabbox3d of its own component type and compares with that type, as the
// Hex-Rays bodies show (aabbox3d<int>&, aabbox3d<signed char>&, ...); each is ledgered against
// this template.
#pragma once

#include <cstddef>
#include <cstring>

#include "glitch-core-io/deps/glitch_core.hpp"

namespace glitch::core {

// dim: components per element (more than three is read as three); stride: bytes between
// elements; count: number of elements. With count zero the box is all zero, and components
// past dim are zero. The loop compares each component with the box's current minimum and
// maximum (strict compares, so NaN never widens a float box).
template <class T>
void computeBoundingBox(const T* data, unsigned dim, int stride, unsigned count, aabbox3d<T>& box) {
    T lo[3] = {T{}, T{}, T{}};
    T hi[3] = {T{}, T{}, T{}};
    if (count != 0) {
        const unsigned dims = dim < 3 ? dim : 3u;
        const auto* bytes = reinterpret_cast<const unsigned char*>(data);
        for (unsigned c = 0; c < dims; ++c) {
            lo[c] = data[c];
            hi[c] = data[c];
        }
        for (unsigned i = 1; i < count; ++i) {
            const unsigned char* element = bytes + static_cast<std::ptrdiff_t>(i) * stride;
            for (unsigned c = 0; c < dims; ++c) {
                T v;
                std::memcpy(&v, element + c * sizeof(T), sizeof(T));
                if (v < lo[c]) lo[c] = v;
                if (hi[c] < v) hi[c] = v;
            }
        }
    }
    box.MinEdge.X = lo[0];
    box.MinEdge.Y = lo[1];
    box.MinEdge.Z = lo[2];
    box.MaxEdge.X = hi[0];
    box.MaxEdge.Y = hi[1];
    box.MaxEdge.Z = hi[2];
}

}  // namespace glitch::core
