#include "glitch-video/image.hpp"

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

}  // namespace glitch::video
