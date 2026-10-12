// glitch::video::ITexture: texture base. Only the state-word helpers are
// reconstructed here (wrap modes, data-dirty bits, lock counter). The layout
// words are kept at their original offsets.
#pragma once

#include <cstdint>

namespace glitch::video {

class ITexture {
public:
    // 0x5e80e8: bytes for the whole texture, including the cube-face multiplier.
    uint32_t getSizeInBytes() const;

    // 0x5fdc84: releases one lock reference. The unlock callback (vtable +0x18)
    // is not modelled, so the branch that calls it only clears the state.
    void unlock();

    // 0x7d3bb4: sets the wrap mode for the S and T axes (and the R axis when the
    // T change is applied). E_TEXTURE_CLAMP values are passed as plain integers.
    void setWrap(uint32_t wrap);

    // 0x6ce46c: the wrap-to-clamp specialisation of setWrap (constant 2).
    void setWrapClamp();

    uint32_t m_refCount = 0;         // +0x04, reference count used by the texture manager
    uint32_t m_width = 0;            // +0x20, mip 0 width
    const int32_t* m_levelSizes = nullptr;  // +0x30, per-mip sizes (or a two-entry range)
    uint32_t m_flags38 = 0;          // +0x38, format, cube and wrap bit fields
    uint8_t m_mipCount3e = 0;        // +0x3e
    uint8_t m_flags3f = 0;           // +0x3f, bit 1 = range sizes, bit 5 = unlock callback, bit 6 = locked
    uint16_t m_dirtyFlags40 = 0;     // +0x40, bit 0x10 = S wrap changed, bit 0x20 = T wrap changed
    uint8_t m_lockCount42 = 0;       // +0x42, low 5 bits
    uint8_t m_lockFlags43 = 0;       // +0x43
};

}  // namespace glitch::video
