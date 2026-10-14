#include "glitch-video/image.hpp"

#include <cstring>

#include "glitch-video/pixel_format.hpp"

namespace glitch::video {

namespace {

// Halves a mip dimension, clamping at 1. Non-positive sizes are returned
// unchanged (the original tests the signed value before shifting).
uint32_t mipDimension(uint32_t size, uint32_t mipLevel) {
    if (static_cast<int32_t>(size) <= 0) {
        return size;
    }
    uint32_t shifted = size >> mipLevel;
    return shifted == 0 ? 1u : shifted;
}

}  // namespace

// 0x601988 _ZN6glitch5video6CImage8initDataEb
// Computes the pitch and the data size, allocates the pixel data when there is none, and (with allocateMips)
// builds the mip chain: one allocation per level, with the level pointer array terminated by a null entry.
//
// Read field by field off the binary: the pitch is computePitch(m_format, m_width) (the two words it passes are
// +0x20 and +0x10), the size is pitch * m_height unless one was already supplied, and the data block is only
// allocated when m_data is still null - which is what lets a caller install its own buffer first and have
// initData skip the allocation.
//
// The level count is the number of halvings of (width, height) down to 1, and the array is sized 4 * (levels+1)
// bytes; the original picks 4 for a 1x1 image. Each level is allocated as height * bytesPerBlock * width (the
// PFDTable byte at +0x15, not the pitch) and zeroed with a byte that advances by 15 per level; that fill byte is
// cosmetic and the port zeroes instead.
void CImage::initData(bool allocateMips) {
    const uint32_t pitch = pixel_format::computePitch(m_format, m_width);
    m_pitch = pitch;
    if (m_dataSize == 0) {
        m_dataSize = pitch * m_height;
    }
    if (m_data == nullptr) {
        m_data = new uint8_t[m_dataSize];
    }
    if (!allocateMips || m_hasMipmaps == 0 || m_mipData != nullptr) {
        return;
    }

    uint32_t width = m_width;
    uint32_t height = m_height;
    const bool singleLevel = width == 1 && height == 1;
    if (!singleLevel) {
        uint32_t count = 1;
        uint32_t probeW = width;
        uint32_t probeH = height;
        while (true) {
            if (probeW > 1) probeW >>= 1;
            if (probeH > 1) probeH >>= 1;
            ++count;
            if (probeW == 1 && probeH == 1) break;
        }
        m_mipLevels = count - 1;
    }
    const uint32_t slots = singleLevel ? 1 : m_mipLevels + 1;
    m_mipData = new void*[slots];

    if (singleLevel) {
        m_mipData[0] = nullptr;
        return;
    }
    uint32_t index = 0;
    while (true) {
        if (width > 1) width >>= 1;
        if (height > 1) height >>= 1;
        const uint32_t bytes = height * static_cast<uint32_t>(pixel_format::detail::PFDTable[m_format].bytesPerBlock) * width;
        uint8_t* level = new uint8_t[bytes];
        std::memset(level, 0, bytes);
        m_mipData[index] = level;
        ++index;
        if (width == 1 && height == 1) break;
    }
    m_mipData[index] = nullptr;
}

// 0x602670 _ZN6glitch5video6CImageC1ENS0_14E_PIXEL_FORMATERKNS_4core11dimension2dIiEEPvbb
// The (format, size, data, copy, own) form, read field by field off the binary. The size and the format go to
// +0x10/+0x14 and +0x20, m_hasMipmaps (+0x28) is clear and m_ownsData (+0x29) takes the own flag.
//
// PORT DEVIATION (deliberate, host-idiomatic): when own is clear the caller's block is kept and used directly
// (the pitch and size are computed but nothing is allocated); when own is set the block is allocated and the
// caller's bytes are copied into it. The original instead always runs initData - which allocates - and then, on
// the copy path only, overwrites m_data and m_mipData with the caller's pointers. In the original those caller
// pointers are allocations from its own allocator that it has already installed; a host caller typically owns a
// plain buffer, and overwriting the pointer that way makes the image write somewhere the caller is not looking.
// The deviation is exactly the own-clear case and is recorded here rather than hidden.
//
// Also not reproduced: on the copy path the original copies each mip level's bytes into the freshly allocated
// chain (the loop after initData). This row therefore stays `partial` and the note names that gap.
CImage::CImage(E_PIXEL_FORMAT format, const core::dimension2d<int32_t>& size, void* data, bool copy, bool own) {
    (void)copy;
    m_data = nullptr;
    m_mipData = nullptr;
    m_width = static_cast<uint32_t>(size.Width);
    m_height = static_cast<uint32_t>(size.Height);
    m_pitch = 0;
    m_dataSize = 0;
    m_format = format;
    m_mipLevels = 0;
    m_hasMipmaps = 0;
    m_ownsData = own ? 1 : 0;

    m_pitch = pixel_format::computePitch(m_format, m_width);
    m_dataSize = m_pitch * m_height;
    if (own) {
        initData(true);
        if (data != nullptr && m_data != nullptr && m_dataSize != 0) {
            std::memcpy(m_data, data, m_dataSize);
        }
    } else {
        m_data = static_cast<uint8_t*>(data);
    }
}

// 0x5ff638 _ZNK6glitch5video6CImage12getDimensionEj
core::dimension2d<uint32_t> CImage::getDimension(uint32_t mipLevel) const {
    core::dimension2d<uint32_t> result;
    result.Width = mipDimension(m_width, mipLevel);
    result.Height = mipDimension(m_height, mipLevel);
    return result;
}

// 0x5ff808 _ZNK6glitch5video6CImage8getPixelEjj
// Outside the image, and for a format other than 2, 5, 8, 10, 12 and 13, the answer is 0. The data is read row by row (width is the
// row length in pixels). Format 2 (8-bit): the byte becomes the top byte of the colour. Formats 5 (16-bit 5-6-5) and 8 (1-5-5-5) widen
// the channels by replicating the high bits; format 10 (24-bit) reads three bytes and sets the top byte to 255; format 12 (32-bit) rotates
// the word by one byte; format 13 (32-bit) swaps its first and third bytes and keeps the others.
SColor CImage::getPixel(uint32_t x, uint32_t y) const {
    SColor result;
    if (m_width <= x || m_height <= y) {
        return result;
    }
    uint32_t b0 = 0;  // v6 (colour byte 0)
    uint32_t b1 = 0;  // v5
    uint32_t b2 = 0;  // v4
    uint32_t b3 = 0;  // a3
    switch (m_format) {
        case 2:
            b3 = m_data[m_width * y + x];
            break;
        case 5:
        case 8: {
            uint32_t value = *reinterpret_cast<const uint16_t*>(m_data + 2 * (x + m_width * y));
            uint32_t low;
            uint32_t green;
            if (m_format == 5) {
                low = (static_cast<uint8_t>((value >> 13) | (static_cast<uint16_t>(value & 0xF800u) >> 8)) << 8) | 0xFFu;
                green = (static_cast<int32_t>(value & 0x7E0u) >> 2) | (static_cast<uint16_t>(value & 0x7E0u) >> 8);
            } else {
                const uint32_t alpha = (value & 0x8000u) != 0 ? 0xFFu : 0u;
                low = alpha | (((static_cast<int32_t>(value & 0x7C00u) >> 7) | (static_cast<uint16_t>(value & 0x7C00u) >> 12)) << 8);
                green = (static_cast<int32_t>(value & 0x3E0u) >> 2) | (static_cast<uint16_t>(value & 0x3E0u) >> 8);
            }
            const uint32_t packed = (low & 0xFFFFu) | ((static_cast<uint8_t>(green) << 16) & 0xFFFFFFu) |
                                    ((((value >> 2) & 7u) | static_cast<uint8_t>(8u * value)) << 24);
            b2 = packed >> 24;
            b3 = packed & 0xFFu;
            b0 = (packed >> 8) & 0xFFu;
            b1 = (packed >> 16) & 0xFFu;
            break;
        }
        case 10: {
            const uint8_t* pixel = m_data + 3 * (x + m_width * y);
            b3 = 0xFF;
            b0 = pixel[0];
            b1 = pixel[1];
            b2 = pixel[2];
            break;
        }
        case 12: {
            const uint32_t value = *reinterpret_cast<const uint32_t*>(m_data + 4 * (x + m_width * y));
            b2 = value >> 24;
            b3 = value & 0xFFu;
            b0 = (value >> 8) & 0xFFu;
            b1 = (value >> 16) & 0xFFu;
            break;
        }
        case 13: {
            const uint32_t value = *reinterpret_cast<const uint32_t*>(m_data + 4 * (x + m_width * y));
            b2 = value & 0xFFu;
            b3 = value >> 24;
            b0 = (value >> 16) & 0xFFu;
            b1 = (value >> 8) & 0xFFu;
            break;
        }
        default:
            break;
    }
    result.color = (b0 & 0xFFu) | ((b1 & 0xFFu) << 8) | (((b2 & 0xFFu) << 16) & 0xFFFFFFu) | ((b3 & 0xFFu) << 24);
    return result;
}

// 0x5ff9b4 _ZN6glitch5video6CImage4fillERKNS_6SColorE
// Packs the colour for the format and writes the resulting word over the whole data block, eight words at a
// time and then the remaining (m_dataSize >> 2) & 7 words - the loop shape the original has (it walks the block
// as int* and advances by 8 words per pass). A format outside 5, 8, 12, 13 and 14 writes nothing.
//
// The two 16-bit cases replicate their packed halfword into both halves of the word: R5G6B5 (PFDTable row 5,
// masks 0xf800/0x07e0/0x001f, shifts 11/5/0) for 5, and A1R5G5B5 (row 8, 0x7c00/0x03e0/0x001f plus 0x8000,
// shifts 10/5/0/15) for 8. The three 32-bit cases feed four 8-bit channels in the byte order each row stores;
// the names are the row masks, so 12 is 0x0000ff00/0x00ff0000/0xff000000/0x000000ff.
void CImage::fill(const SColor& color) {
    const uint8_t c0 = color.component(0);
    const uint8_t c1 = color.component(1);
    const uint8_t c2 = color.component(2);
    const uint8_t c3 = color.component(3);

    uint32_t packed = 0;
    switch (m_format) {
        case 5: {  // R5G6B5: halfword (R5 << 11) | (G6 << 5) | B5, replicated
            const uint32_t half = (static_cast<uint32_t>(c0 >> 3) << 11) |
                                  (static_cast<uint32_t>(c1 >> 2) << 5) |
                                  static_cast<uint32_t>(c2 >> 3);
            packed = half | (half << 16);
            break;
        }
        case 8: {  // A1R5G5B5: halfword (A1 << 15) | (R5 << 10) | (G5 << 5) | B5, replicated
            const uint32_t half = (static_cast<uint32_t>(c3 >> 7) << 15) |
                                  (static_cast<uint32_t>(c0 >> 3) << 10) |
                                  (static_cast<uint32_t>(c1 >> 3) << 5) |
                                  static_cast<uint32_t>(c2 >> 3);
            packed = half | (half << 16);
            break;
        }
        case 12:  // byte order r, g, b, a
            packed = static_cast<uint32_t>(c3) | (static_cast<uint32_t>(c0) << 8) |
                     (static_cast<uint32_t>(c1) << 16) | (static_cast<uint32_t>(c2) << 24);
            break;
        case 13:  // byte order a, r, g, b
            packed = static_cast<uint32_t>(c2) | (static_cast<uint32_t>(c3) << 8) |
                     (static_cast<uint32_t>(c0) << 16) | (static_cast<uint32_t>(c1) << 24);
            break;
        case 14:  // byte order g, b, a, r
            packed = static_cast<uint32_t>(c1) | (static_cast<uint32_t>(c2) << 8) |
                     (static_cast<uint32_t>(c3) << 16) | (static_cast<uint32_t>(c0) << 24);
            break;
        default:
            return;
    }

    uint32_t* words = reinterpret_cast<uint32_t*>(m_data);
    const uint32_t wholeBlocks = m_dataSize >> 5;  // (m_dataSize >> 2) / 8
    for (uint32_t block = 0; block < wholeBlocks; ++block) {
        for (int i = 0; i < 8; ++i) {
            words[i] = packed;
        }
        words += 8;
    }
    const uint32_t tail = (m_dataSize >> 2) & 7u;
    for (uint32_t i = 0; i < tail; ++i) {
        words[i] = packed;
    }
}

}  // namespace glitch::video
