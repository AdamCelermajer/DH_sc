// glitch::video::IBuffer: CPU-side buffer used by vertex streams and batch
// baking. The copy, reset, map and unmap paths are reconstructed; the vtable
// slots they call (driver map, unmap callback) are inferred from the call pattern.
#pragma once

#include <cstddef>
#include <cstdint>

#include "glitch-video/deps/glitch_video_types.hpp"

namespace glitch::video {

class IBuffer {
public:
    // Flag bits of m_flags12 (+0x12).
    static constexpr uint8_t kFlagOwnsData = 0x01;      // data is a private copy, freed on replace
    static constexpr uint8_t kFlagHasData = 0x02;       // set when the buffer has a size, data and usage != 4
    static constexpr uint8_t kFlagDirty = 0x04;         // mapped copy must be read back before the next read map
    static constexpr uint8_t kFlagDriverMapped = 0x08;  // driver-side mapping (vtable mapImpl path)
    static constexpr uint8_t kFlagUnmapCallback = 0x20; // unmap callback flagged (vtable +0x18)

    IBuffer() = default;
    virtual ~IBuffer() = default;
    // 0x5a1908 (C2) / 0x5a197c (C1): stores type, usage, size, data and the flag byte. Bit 1 of
    // the flag byte is set when the buffer has a size, data and a usage other than 4.
    IBuffer(E_BUFFER_TYPE type, E_BUFFER_USAGE usage, uint32_t size, void* data, uint8_t flags);
    // 0x5a1c1c: when the buffer has a size and no private copy of its data, copies the current
    // bytes into a new allocation and sets kFlagOwnsData. Returns false when nothing was copied.
    bool copy();
    // 0x5a1cb4: replaces the data pointer and size. Frees the old data when it is privately
    // owned and the pointer changes. keepOwned sets kFlagOwnsData on the new data.
    void reset(uint32_t size, void* data, bool keepOwned);
    // 0x637a68: releases one map reference. The unmap callback (vtable +0x18) is
    // not modelled and is skipped when the buffer is not flagged for it.
    void unmap();
    // 0x5a19f0: maps the buffer for an access mode and returns its pointer, or nullptr.
    uint8_t* map(int32_t access);
    // 0x5a1adc, map(E_BUFFER_READ_MAP_ACCESS). Partial: the unmap callback (vtable +0x18)
    // is not modelled. The original is const; this port is not, because the virtual hooks
    // it calls are non-const and existing tests override them.
    uint8_t* mapRead(int32_t access);
    // vtable +0x14 (index 5): the driver's mapping for the access mode. Base default.
    virtual uint8_t* mapImpl(int32_t access);
    // vtable +0x1c (index 7): the pointer of a counted map with the callback flag set. Base default.
    virtual uint8_t* getMappedData();

    uint32_t m_refCount = 0;     // +0x04, zeroed by the constructor
    uint8_t* m_data = nullptr;   // +0x08
    uint32_t m_size = 0;         // +0x0c
    uint8_t m_type = 0;          // +0x10, E_BUFFER_TYPE
    uint8_t m_usage = 0;         // +0x11, E_BUFFER_USAGE
    uint8_t m_flags12 = 0;     // +0x12, see kFlag* above
    uint8_t m_mapCount13 = 0;  // +0x13, low 5 bits = map count, bits 5-6 = access
};

}  // namespace glitch::video
