// glitch::core::computeBoundingBox: the box of a strided array of components. The first
// element starts the box, the rest widen it. Instantiated for float (0x6a11bc) and the integer
// component types (signed and unsigned char, short and int); each is ledgered against this
// template.
#pragma once

#include <algorithm>
#include <cstddef>

#include "glitch-core-io/deps/glitch_core.hpp"

namespace glitch::core {

// dim: components per element (more than three is read as three); stride: bytes between
// elements; count: number of elements. With count zero the box is all zero.
//
// PARTIAL: the first element and the zero fill of the unused components follow the
// decompile. The per-component widening of the later elements is inferred from the first
// compare in the listing (the rest of that loop is a tail call that is not visible).
template <class T>
void computeBoundingBox(const T* data, unsigned dim, int stride, unsigned count, aabbox3d<float>& box) {
    float lo[3] = {0.f, 0.f, 0.f};
    float hi[3] = {0.f, 0.f, 0.f};
    if (count != 0) {
        const unsigned dims = dim < 3 ? dim : 3u;
        const auto* bytes = reinterpret_cast<const unsigned char*>(data);
        for (unsigned c = 0; c < dims; ++c) {
            const float v = static_cast<float>(data[c]);
            lo[c] = v;
            hi[c] = v;
        }
        for (unsigned i = 1; i < count; ++i) {
            const auto* element = reinterpret_cast<const T*>(bytes + static_cast<std::ptrdiff_t>(i) * stride);
            for (unsigned c = 0; c < dims; ++c) {
                const float v = static_cast<float>(element[c]);
                lo[c] = std::min(lo[c], v);
                hi[c] = std::max(hi[c], v);
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
