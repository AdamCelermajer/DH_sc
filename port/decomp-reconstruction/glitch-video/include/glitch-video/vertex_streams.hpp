// glitch::video::CVertexStreams: the vertex stream table of a mesh. The
// original stores 16-byte SVertexStream entries in an inline-or-heap array
// with begin at +0x14 and end pointer at +0x10. Here the array is a vector;
// offsets are kept as comments.
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-video/buffer.hpp"
#include "glitch-video/deps/intrusive_ptr.hpp"

namespace glitch::video {

// 16 bytes in the original.
struct SVertexStream {
    boost::intrusive_ptr<IBuffer> buffer;  // +0x00
    uint32_t offset = 0;                   // +0x04, changed by CVertexStreams::addOffset
    uint16_t attribute = 0;                // +0x08, E_VERTEX_ATTRIBUTE index
    uint16_t field0a = 0;                  // +0x0a, copied by setStream; meaning not identified
    uint16_t field0c = 0;                  // +0x0c, copied by setStream; meaning not identified
    uint16_t field0e = 0;                  // +0x0e, copied by setStream; meaning not identified
};

class CVertexStreams {
public:
    // 0x5a0a58: bit mask of the attributes whose stream has a buffer.
    uint32_t getActiveAttributeMask() const;

    // 0x5a0aac / 0x5a0af0: first stream in [first, end) with attribute >= wanted; the
    // end pointer when the attribute is not present.
    const SVertexStream* getStream(uint32_t attribute, const SVertexStream* first, const SVertexStream* end) const;
    SVertexStream* getStream(uint32_t attribute, SVertexStream* first, SVertexStream* end);

    // 0x5a0b34: adds delta to the offset of every stream.
    void addOffset(int32_t delta);

    // 0x5a0b74: true when every attribute in mask (restricted to the attribute
    // mask +0x04) reads from the same buffer.
    bool isHomogeneous(uint32_t mask) const;

    std::vector<SVertexStream> m_streams;  // +0x10..+0x24 (inline array in the original)
    uint32_t m_attributeMask = 0;          // +0x04, attributes present in the table
};

}  // namespace glitch::video
