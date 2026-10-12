// glitch::video::CPrimitiveStream: primitive type and element count of a draw.
// Only the two fields read by getPrimitiveCount are laid out here.
#pragma once

#include <cstdint>

#include "glitch-video/deps/glitch_video_types.hpp"

namespace glitch::video {

class CPrimitiveStream {
public:
    // 0x5a0300: primitives for the stream's element count, by primitive type.
    // Types 1..8 use the upstream E_PRIMITIVE_TYPE order: line strip, line loop,
    // lines, triangle strip, triangle fan, triangles, quad strip, quads.
    uint32_t getPrimitiveCount() const;

    uint8_t unknown00[8] = {};     // +0x00..+0x07
    uint32_t m_count = 0;          // +0x08, element count
    uint8_t unknown0c[0x0a] = {};  // +0x0c..+0x15
    uint16_t m_primitiveType = 0;  // +0x16, E_PRIMITIVE_TYPE
};

}  // namespace glitch::video
