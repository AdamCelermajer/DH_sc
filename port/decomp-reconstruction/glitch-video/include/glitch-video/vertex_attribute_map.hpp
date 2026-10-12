// glitch::video::CVertexAttributeMap: a 30-byte table that maps each slot to
// the index of a stream in a CVertexStreams table (0xff when unset). Layout
// from the original: +0x00 is zeroed by every constructor and not copied by the
// assignment; +0x04..+0x21 holds the map.
#pragma once

#include <cstdint>

#include "glitch-video/deps/intrusive_ptr.hpp"
#include "glitch-video/vertex_streams.hpp"

namespace glitch::video {

class CVertexAttributeMap {
public:
    static constexpr uint32_t kMapSize = 0x1e;  // 30 bytes at +0x04

    // 0x5a07bc, 0x5a07d8: copy constructor. Copies the map, not the +0x00 word.
    CVertexAttributeMap(const CVertexAttributeMap& other);

    // 0x5a07f4, 0x5a0814: copies kMapSize bytes from data.
    explicit CVertexAttributeMap(const uint8_t* data);

    // 0x5a0834, 0x5a0894: starts with every slot unset, then calls set when the
    // streams pointer is not null.
    CVertexAttributeMap(const boost::intrusive_ptr<const CVertexStreams>& streams, uint32_t count,
                        const uint8_t* pairs, bool sorted);

    // 0x5a0788: copies the map.
    CVertexAttributeMap& operator=(const CVertexAttributeMap& other);

    // 0x5a0708: pairs holds count (slot, attribute) byte pairs. For each pair,
    // finds the first stream whose attribute is at least `attribute` and stores
    // its index in the slot. When sorted is true, the next search starts at the
    // stream just found.
    void set(const boost::intrusive_ptr<const CVertexStreams>& streams, uint32_t count, const uint8_t* pairs,
             bool sorted);

    uint32_t m_unknown00 = 0;             // +0x00, zeroed by the constructors; meaning not identified
    uint8_t m_map[kMapSize] = {};         // +0x04..+0x21, stream index per slot, 0xff when unset
};

}  // namespace glitch::video
