// glitch::video::IBuffer: CPU-side buffer used by vertex streams and batch
// baking. Only the copy and unmap paths are reconstructed; the vtable slots
// they call (unmap callback) are not modelled.
#pragma once

#include <cstddef>
#include <cstdint>

#include "glitch-video/deps/glitch_video_types.hpp"

namespace glitch::video {

class IBuffer {
public:
    IBuffer() = default;
    virtual ~IBuffer() = default;
    // 0x5a1908 (C2) / 0x5a197c (C1): stores type, usage, size, data and the flag byte. Bit 1 of
    // the flag byte is set when the buffer has a size, data and a usage other than 4.
    IBuffer(E_BUFFER_TYPE type, E_BUFFER_USAGE usage, uint32_t size, void* data, uint8_t flags);
    // 0x5a1c1c: when the buffer does not own its data and has a size, gives it a
    // private copy of the current bytes.
    void copy();
    // 0x637a68: releases one map reference. The unmap callback (vtable +0x18) is
    // not modelled and is skipped when the buffer is not flagged for it.
    void unmap();
    // 0x5a19f0: maps the buffer for an access mode and returns its pointer, or nullptr.
    // The E_BUFFER_READ_MAP_ACCESS overload (0x5a1adc) is not ported: that enum is an
    // int32 alias here, so it would collide with this overload.
    uint8_t* map(int32_t access);
    // vtable +0x14 (index 5): the driver's mapping for the access mode. Base default.
    virtual uint8_t* mapImpl(int32_t access);
    // vtable +0x1c (index 7): the pointer of a counted map with the callback flag set. Base default.
    virtual uint8_t* getMappedData();

    uint32_t m_refCount = 0;     // +0x04, zeroed by the constructor
    uint8_t* m_data = nullptr;   // +0x08
    uint32_t m_size = 0;         // +0x0c
    uint8_t m_type = 0;          // +0x10, E_BUFFER_TYPE
    uint8_t m_usage = 0;         // +0x11, E_BUFFER_USAGE
    uint8_t m_flags12 = 0;       // +0x12, bit 0 = data not privately owned, bit 5 = unmap callback
    uint8_t m_mapCount13 = 0;    // +0x13, low 5 bits = map count
};

}  // namespace glitch::video
