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

    // 0x5a0bfc: flag == false sets kFlagHomogeneous when no two neighbouring
    // streams hold different non-null buffers. flag == true clears it.
    void updateHomogeneityInternal(bool flag);

    // 0x5a0d80: rebuilds m_flags. Sets kFlagHasData on entry, then clears it when
    // a bound buffer has no data pointer. Homogeneity follows the same rule as
    // updateHomogeneityInternal.
    void updateStatesInternal(bool flag);

    // 0x5a0e24: thunk, `b #0x5a0d80`. Forwards to updateStatesInternal.
    void updateStates(bool flag);

    // Bits of m_flags, named by what the code tests them for.
    static constexpr uint16_t kFlagHomogeneous = 0x0001;  // bit 0, every bound stream shares one buffer
    static constexpr uint16_t kFlagHasData = 0x0002;      // bit 1, set on entry, cleared on a data-less buffer

    std::vector<SVertexStream> m_streams;  // +0x10..+0x24 (inline array in the original)
    uint32_t m_attributeMask = 0;          // +0x04, attributes present in the table
    uint16_t m_flags = 0;                  // +0x0e, see kFlag* above
};

}  // namespace glitch::video
