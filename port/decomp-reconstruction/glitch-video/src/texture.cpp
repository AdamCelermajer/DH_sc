#include "glitch-video/texture.hpp"

#include <cstring>

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

// 0x5fdf74 _ZN6glitch5video8ITexture7setDataEPvbb
// Flag names: owned = a3 (bit 0 of +0x3f, freed on replace), rangeSizes = a4 (bit 1 of +0x3f).
void ITexture::setData(void* data, bool owned, bool rangeSizes) {
    bool refreshDirty = false;  // original v10: the pointer changed to a non-null value
    bool noData = false;        // original jump to LABEL_19
    if (data == m_data) {
        noData = (data == nullptr);
    } else {
        if (m_data != nullptr && (m_flags3f & 1) != 0) {
            delete[] m_data;
        }
        m_data = static_cast<uint8_t*>(data);
        noData = (data == nullptr);
        refreshDirty = !noData;
    }

    if (noData) {
        // LABEL_19: no data. Bit 0 of +0x3f is always set here; the dirty word loses bit 0x2,
        // and bit 0x1 only when +0x3f bit 0x8 is set.
        const uint8_t flags = static_cast<uint8_t>(m_flags3f | 1);
        m_flags3f = flags;
        if ((flags & 8) != 0) {
            m_dirtyFlags40 = static_cast<uint16_t>(m_dirtyFlags40 & ~1u);
        }
        m_dirtyFlags40 = static_cast<uint16_t>(m_dirtyFlags40 & 0xfffdu);
        if (m_mipCount3e > 1 && rangeSizes) {
            m_flags3f = static_cast<uint8_t>(m_flags3f | 2);
        } else {
            m_flags3f = static_cast<uint8_t>(m_flags3f & ~2);
        }
        return;
    }

    // LABEL_7: a non-null pointer.
    uint8_t flags = owned ? static_cast<uint8_t>(m_flags3f | 1) : static_cast<uint8_t>(m_flags3f & 0xfe);
    const uint32_t mipCount = m_mipCount3e;
    if (mipCount > 1 && rangeSizes) {
        if ((flags & 2) == 0) {
            // The dirty bitmap follows the mipCount + 1 per-mip sizes.
            uint32_t* bitmap = reinterpret_cast<uint32_t*>(m_levelSizes) + mipCount + 1;
            std::memset(bitmap, 0, 4 * ((mipCount + 31) >> 5));
        }
        flags = static_cast<uint8_t>(flags | 2);
    } else {
        flags = static_cast<uint8_t>(flags & ~2);
    }
    m_flags3f = flags;
    if (refreshDirty) {
        setDataDirty(false);
    }
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

namespace glitch::video {

// 0x5fdc0c _ZNK6glitch5video8ITexture5unmapEv
// Partial: the unlock callback (vtable +0x18, taken when bit 0x20 of +0x3f is set) is not
// modelled, so that branch only clears the state.
void ITexture::unmap() const {
    uint8_t count = m_lockCount42;
    if ((count & 0x1f) > 1) {
        m_lockCount42 = static_cast<uint8_t>(((count & 0x1f) - 1) | (count & 0xe0));
        return;
    }
    m_lockCount42 = 0;
    m_flags3f = static_cast<uint8_t>(m_flags3f & 0xbf);
    m_lockFlags43 = 0;
}

// 0x5fdae8 _ZNK6glitch5video8ITexture12setDataDirtyEb
// The bitmap is the word array that follows the per-mip sizes: index mip + 1 onwards.
void ITexture::setDataDirty(bool flag) const {
    const uint32_t mipCount = m_mipCount3e;
    const uint32_t faces = (m_flags38 & 3) == 2 ? 6u : 1u;
    uint32_t* bitmap = reinterpret_cast<uint32_t*>(m_levelSizes) + mipCount + 1;
    if ((m_flags3f & 2) == 0) {
        if (m_data != nullptr || flag) {
            uint32_t words = (mipCount * faces + 0x1fu) >> 5;
            m_dirtyFlags40 = static_cast<uint16_t>(m_dirtyFlags40 | 1);
            for (uint32_t i = 0; i < words; ++i) {
                bitmap[i] = 0xffffffffu;
            }
        }
        return;
    }
    if (m_data != nullptr || flag) {
        m_dirtyFlags40 = static_cast<uint16_t>(m_dirtyFlags40 | 1);
        uint32_t position = 0;
        for (uint32_t face = 0; face < faces; ++face) {
            bitmap[position >> 5] |= 1u << (position & 0x1fu);
            position += mipCount;
        }
    }
}

// 0x5ff26c _ZN6glitch5video8ITexture4copyEv
// Partial, see texture.hpp. Sizes follow getSizeInBytes: one face plus five 128-byte-aligned faces for cubes.
uint32_t ITexture::copy() {
    // Bit 0 of +0x3f means the data is owned (freed by setData), as in the asm at 0x5ff26c
    // (TST R3,#1 then the non-zero check that returns 0). A copy is refused only when owned data exists.
    const bool ownsData = (m_flags3f & 1) != 0;
    if (ownsData && m_data != nullptr) {
        return 0;
    }
    const uint8_t* source = m_data;
    const uint32_t total = getSizeInBytes();
    uint8_t* dest = new uint8_t[total];
    m_data = dest;
    if (source != nullptr) {
        std::memcpy(dest, source, total);
    }
    return 1;
}

}  // namespace glitch::video
