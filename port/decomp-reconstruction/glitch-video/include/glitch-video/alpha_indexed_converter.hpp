// glitch::video::pixel_format (anonymous namespace) SAlphaIndexedPackedConverter: converts one
// packed pixel by masking and shifting two channel groups, then looks up an alpha value in a
// 16-bit table (the sRGB upscale table). The original is a template instance; the port keeps the
// one instance that the pixel code uses. Offsets are the ARM layout of the original.
#pragma once

#include <cstdint>

#include "glitch-video/deps/glitch_video_types.hpp"

namespace glitch::video::pixel_format {

struct SAlphaIndexedPackedConverter {
    // 0x5eeae4. Partial: the original reads the masks and shifts from detail::PFDTable, which is
    // not in the repo. The fields keep their zero defaults until that table is supplied.
    SAlphaIndexedPackedConverter(E_PIXEL_FORMAT source, E_PIXEL_FORMAT target);

    // 0x5ededc. Reconstructed from the pseudocode; the masks and shifts come from the constructor.
    uint32_t operator()(uint32_t pixel) const;

    uint32_t m_resultMask[3] = {};          // +0x00, +0x04, +0x08: masks applied to the three channel terms
    uint32_t m_fixedBits = 0;               // +0x0c: masked by m_lookupMask, then OR'd (the alpha term)
    uint32_t m_orValue = 0;                 // +0x18: constant OR'd into the result
    uint32_t m_inputMask[2][3] = {};        // +0x1c..+0x2c (group 0) and +0x28..+0x30 (group 1)
    uint8_t m_rightShift[2][3] = {};        // +0x10..+0x12 (group 0), +0x34..+0x36 (group 1)
    uint8_t m_leftShift[2][3] = {};         // +0x14..+0x16 (group 0), +0x37..+0x39 (group 1)
    uint8_t m_lookupIndexShift = 0;         // +0x13: right shift of the lookup index
    uint8_t m_lookupResultShift = 0;        // +0x3a: right shift of the table value
    uint8_t m_lookupResultLeftShift = 0;    // +0x17: left shift of the table value
    const uint16_t* m_lookupTable = nullptr;  // +0x3c
    uint32_t m_lookupMask = 0;              // +0x40: mask applied to the pixel before the lookup
};

}  // namespace glitch::video::pixel_format
