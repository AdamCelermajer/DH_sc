#include "glitch-video/texture.hpp"

namespace glitch::video {

// 0x5e80e8 _ZNK6glitch5video8ITexture14getSizeInBytesEv
// Cube textures (type bits == 2) use a multiplier of 5 on the 128-byte-aligned
// size of one face, as in the original.
uint32_t ITexture::getSizeInBytes() const {
    uint32_t multiplier = ((m_flags38 & 3) == 2) ? 5u : 0u;
    uint32_t size;
    if ((m_flags3f & 2) != 0) {
        size = static_cast<uint32_t>(m_levelSizes[1] - m_levelSizes[0]);
    } else {
        size = static_cast<uint32_t>(m_levelSizes[m_mipCount3e]);
    }
    uint32_t aligned = (size + 0x7fu) & 0xffffff80u;
    return aligned * multiplier + size;
}

// 0x5fdc84 _ZN6glitch5video8ITexture6unlockEv
void ITexture::unlock() {
    uint8_t count = m_lockCount42;
    if ((count & 0x1f) > 1) {
        m_lockCount42 = static_cast<uint8_t>(((count & 0x1f) - 1) | (count & 0xe0));
        return;
    }
    if ((m_flags3f & 0x20) != 0) {
        // Unlock callback (vtable +0x18) is not modelled.
    }
    m_flags3f = static_cast<uint8_t>(m_flags3f & 0xbf);
    m_lockCount42 = 0;
    m_lockFlags43 = 0;
}

// 0x7d3bb4 _ZN6glitch5video8ITexture7setWrapENS0_15E_TEXTURE_CLAMPE
// S axis: bits 18..20 of +0x38, flag 0x10. T axis: bits 21..23, flag 0x20.
// The R-axis branch is kept as in the original; with a valid clamp value
// the comparison it relies on is false, so only S and T are written in practice.
void ITexture::setWrap(uint32_t wrap) {
    uint32_t v = m_flags38;
    if (wrap != ((v << 11) >> 29)) {
        v = (v & 0xffe3ffffu) | ((wrap & 7u) << 18);
        m_flags38 = v;
        m_dirtyFlags40 = static_cast<uint16_t>(m_dirtyFlags40 | 0x10);
    }
    if (wrap != ((v << 8) >> 29)) {
        uint32_t t = (v & 0xff1fffffu) | ((wrap & 7u) << 21);
        uint16_t flags = m_dirtyFlags40;
        bool bVar5 = wrap != ((t << 8) >> 29);
        m_flags38 = t;
        if (bVar5) {
            t = (v & 0xf81fffffu) | ((wrap & 7u) << 21);
        }
        m_dirtyFlags40 = static_cast<uint16_t>(flags | 0x20);
        if (bVar5) {
            m_dirtyFlags40 = static_cast<uint16_t>(flags | 0x60);
            m_flags38 = t | ((wrap & 7u) << 24);
        }
    }
}

// 0x6ce46c _ZN6glitch5video8ITexture7setWrapENS0_15E_TEXTURE_CLAMPE.clone.1
void ITexture::setWrapClamp() {
    uint32_t v = m_flags38;
    if (((v << 11) >> 29) != 2) {
        v = (v & 0xffe3ffffu) | 0x80000u;
        m_dirtyFlags40 = static_cast<uint16_t>(m_dirtyFlags40 | 0x10);
        m_flags38 = v;
    }
    if (((v << 8) >> 29) != 2) {
        m_flags38 = (v & 0xff1fffffu) | 0x400000u;
        m_dirtyFlags40 = static_cast<uint16_t>(m_dirtyFlags40 | 0x20);
    }
}

}  // namespace glitch::video
