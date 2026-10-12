// glitch::video::IBuffer: CPU-side buffer used by vertex streams and batch
// baking. Only the copy and unmap paths are reconstructed; the vtable slots
// they call (unmap callback) are not modelled.
#pragma once

#include <cstddef>
#include <cstdint>

namespace glitch::video {

class IBuffer {
public:
    IBuffer() = default;
    // 0x5a1c1c: when the buffer does not own its data and has a size, gives it a
    // private copy of the current bytes.
    void copy();
    // 0x637a68: releases one map reference. The unmap callback (vtable +0x18) is
    // not modelled and is skipped when the buffer is not flagged for it.
    void unmap();

    uint8_t* m_data = nullptr;   // +0x08
    uint32_t m_size = 0;         // +0x0c
    uint8_t m_flags12 = 0;       // +0x12, bit 0 = data not privately owned, bit 5 = unmap callback
    uint8_t m_mapCount13 = 0;    // +0x13, low 5 bits = map count
};

}  // namespace glitch::video
