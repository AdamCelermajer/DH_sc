#include "glitch-video/pixel_format.hpp"

#include <cstring>

namespace glitch::video::pixel_format {

namespace detail {
// PFDTable is defined in pixel_format_table.cpp: the original's 40 rows of 0x28 bytes, read from
// ELF 0x8e36c0.
}  // namespace detail

namespace {

using detail::PFDTable;
using detail::SFormatEntry;

// ARM register shifts by 32 or more give zero; C++ shifts by that much are undefined.
uint32_t lsrOrZero(uint32_t value, uint32_t shift) { return shift >= 32 ? 0 : value >> shift; }

}  // namespace

// 0x5ed954 _ZN6glitch5video12pixel_format13getPackedTypeENS0_14E_PIXEL_FORMATE
char getPackedType(E_PIXEL_FORMAT format) {
    const SFormatEntry& e = PFDTable[format];
    if ((e.flags & 0x40) != 0) return e.packedType;
    if (e.field17 == 1) return e.packedType;
    if (e.packedType == 0) {
        if (e.field17 < 3) return 1;
    } else if (e.packedType != 1 || e.field17 != 2) {
        return -1;
    }
    return 2;
}

// 0x5eda70 _ZN6glitch5video12pixel_format9getFormatEjjjj
int getFormat(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    for (int i = 0; i < detail::kFormatCount; ++i) {
        const SFormatEntry& e = PFDTable[i];
        if (e.key[0] == a && e.key[1] == b && e.key[2] == c && e.key[3] == d) return i;
    }
    return detail::kFormatCount - 1;  // the original returns 0x27 when no row matches
}

// 0x5edaec _ZN6glitch5video12pixel_format12computePitchENS0_14E_PIXEL_FORMATEj
uint32_t computePitch(E_PIXEL_FORMAT format, uint32_t width) {
    const SFormatEntry& e = PFDTable[format];
    const uint32_t blockWidth = e.blockWidth;
    if (blockWidth > 1) {
        const uint32_t blocksPerRow = (blockWidth - 1 + width) / blockWidth;
        return static_cast<uint32_t>(e.bytesPerBlock) * blocksPerRow;
    }
    return (static_cast<uint32_t>(e.bitsPerPixel) * width) >> 3;
}

// 0x5edb48 _ZN6glitch5video12pixel_format18computeSizeInBytesENS0_14E_PIXEL_FORMATEjj
uint32_t computeSizeInBytes(E_PIXEL_FORMAT format, uint32_t width, uint32_t height) {
    const SFormatEntry& e = PFDTable[format];
    const uint32_t pitch = computePitch(format, width);
    uint32_t size;
    if (e.blockHeight < 2) {
        size = height * pitch;
    } else {
        const uint32_t blockRows = (e.blockHeight - 1 + height) / e.blockHeight;
        size = blockRows * pitch;
    }
    if (size < e.minSize) size = e.minSize;
    return size;
}

// 0x5edbb8 _ZN6glitch5video12pixel_format18computeSizeInBytesENS0_14E_PIXEL_FORMATEjjj
int computeSizeInBytes(E_PIXEL_FORMAT format, uint32_t width, uint32_t height, uint32_t depth) {
    return static_cast<int>(depth * computeSizeInBytes(format, width, height));
}

// 0x5edbcc _ZN6glitch5video12pixel_format24computeMipmapSizeInBytesENS0_14E_PIXEL_FORMATEjjhb
// Returned value: see the header note on the void pseudocode.
uint32_t computeMipmapSizeInBytes(E_PIXEL_FORMAT format, uint32_t width, uint32_t height, uint8_t level, bool flag) {
    if (!flag) {
        width = lsrOrZero(width, level);
        if (width == 0) width = 1;
    }
    height = lsrOrZero(height, level);
    if (height == 0) height = 1;
    return computeSizeInBytes(format, width, height);
}

// 0x5edbec _ZN6glitch5video12pixel_format24computeMipmapSizeInBytesENS0_14E_PIXEL_FORMATEjjjhb
uint32_t computeMipmapSizeInBytes(E_PIXEL_FORMAT format, uint32_t width, uint32_t height, uint32_t depth,
                                  uint8_t level, bool flag) {
    if (!flag) {
        width = lsrOrZero(width, level);
        if (width == 0) width = 1;
    }
    height = lsrOrZero(height, level);
    if (height == 0) height = 1;
    depth = lsrOrZero(depth, level);
    if (depth == 0) depth = 1;
    return static_cast<uint32_t>(computeSizeInBytes(format, width, height, depth));
}

// 0x5edc18 _ZN6glitch5video12pixel_format18computeSizeInBytesENS0_14E_PIXEL_FORMATEjjjhb
int computeSizeInBytes(E_PIXEL_FORMAT format, uint32_t width, uint32_t height, uint32_t depth, uint8_t levels,
                       bool flag) {
    int total = 0;
    while (levels != 0) {
        --levels;
        total += static_cast<int>(computeMipmapSizeInBytes(format, width, height, depth, levels, flag));
    }
    return total;
}

// 0x5edc80 _ZN6glitch5video12pixel_format18computeSizeInBytesENS0_14E_PIXEL_FORMATEjjhb
int computeSizeInBytes(E_PIXEL_FORMAT format, uint32_t width, uint32_t height, uint8_t levels, bool flag) {
    return computeSizeInBytes(format, width, height, 1u, levels, flag);
}

namespace {

// Packed RGB to luminance/alpha converter. Built from one PFDTable row: three
// channel weights (0.3, 0.59, 0.11), the channel masks and shifts of the row,
// and the alpha mask, shift and scale. Layout offsets are the original's.
struct SPackedRGBtoLuminanceAlphaConverter {
    explicit SPackedRGBtoLuminanceAlphaConverter(E_PIXEL_FORMAT format);

    float weight[3] = {};          // +0x00
    uint32_t channelMask[3] = {};  // +0x0c
    uint8_t channelShift[3] = {};  // +0x18
    uint8_t alphaShift = 0;        // +0x1b
    uint32_t alphaMask = 0;        // +0x1c
    float alphaScale = 0.0f;       // +0x20, 255 / (alphaMask >> alphaShift)
    uint32_t alphaMaxBits = 0;     // +0x24, 0 when the row flags bit 0 is set
};

}  // namespace

// 0x5edca4 _ZN6glitch5video12pixel_format12_GLOBAL__N_135SPackedRGBtoLuminanceAlphaConverterC1ENS0_14E_PIXEL_FORMATE
// (C1 only; the C2 form is not in the ARM listing.) Reconstructed against the listing: the three
// channel weights 0.3/0.59/0.11 divided by (key[i] >> shift[i]), the masks and shifts stored at
// +0x0c/+0x18, then alphaShift +0x1b, alphaMask +0x1c, alphaScale +0x20 and alphaMaxBits +0x24
// (zeroed when the row flags bit 0 is set). The table it reads is the binary's.
SPackedRGBtoLuminanceAlphaConverter::SPackedRGBtoLuminanceAlphaConverter(E_PIXEL_FORMAT format) {
    const SFormatEntry& row = PFDTable[format];
    // Row bytes not named in SFormatEntry (+0x1c..+0x1e are the channel shifts, +0x1f the
    // alpha shift). unknown18 starts at +0x18, so the shifts are unknown18[4..6] and [7].
    // Luma weights 0.3, 0.59, 0.11 (float bit patterns 0x3e99999a, 0x3f170a3d, 0x3de147ae).
    static constexpr float kWeights[3] = {0.3f, 0.59f, 0.11f};
    for (int i = 0; i < 3; ++i) {
        const uint32_t mask = row.key[i];            // row words +0x04, +0x08, +0x0c
        const uint8_t shift = row.unknown18[4 + i];  // row bytes +0x1c, +0x1d, +0x1e
        weight[i] = kWeights[i] / static_cast<float>(lsrOrZero(mask, shift));
        channelMask[i] = mask;
        channelShift[i] = shift;
    }
    alphaShift = row.unknown18[7];                   // row byte +0x1f
    alphaMask = row.key[3];                          // row word +0x10
    alphaScale = 255.0f / static_cast<float>(lsrOrZero(alphaMask, alphaShift));
    alphaMaxBits = (row.flags & 1) != 0 ? 0 : lsrOrZero(alphaMask, alphaShift);
}

// 0x5ed9d0 _ZN6glitch5video12pixel_format27computeRelativeSwizzleTableENS0_14E_PIXEL_FORMATES2_Ph
uint8_t* computeRelativeSwizzleTable(E_PIXEL_FORMAT from, E_PIXEL_FORMAT to, uint8_t* table) {
    const SFormatEntry& src = PFDTable[from];
    const SFormatEntry& dst = PFDTable[to];
    // Byte 0x20 of the destination row indexes the table; the source row supplies the value.
    table[dst.unknown18[8]] = src.unknown18[8];
    if ((dst.flags & 4) == 0) {
        table[dst.unknown18[9]] = src.unknown18[9];    // +0x21
        table[dst.unknown18[10]] = src.unknown18[10];  // +0x22
    }
    if ((src.flags & 1) != 0 && (dst.flags & 1) != 0) {
        table[dst.unknown18[11]] = src.unknown18[11];  // +0x23
    }
    return table;
}

// 0x5edd9c _ZN6glitch5video12pixel_format9swapBytesENS0_14E_PIXEL_FORMATEPKvjbPv
int swapBytes(E_PIXEL_FORMAT format, const void* src, uint32_t size, bool flag, void* dst) {
    const SFormatEntry& row = PFDTable[format];
    if ((row.flags & 8) != 0) return 0;
    if (flag) return 0;
    const uint8_t packedType = static_cast<uint8_t>(row.packedType);  // +0x14
    if (packedType == 0) return 0;

    const auto* in = static_cast<const uint8_t*>(src);
    auto* out = static_cast<uint8_t*>(dst);
    const uint32_t words = size >> 2;
    if (packedType == 1 || packedType == 3) {
        // Each word: the two bytes of each 16-bit half are swapped (result bytes 1,0,3,2).
        for (uint32_t i = 0; i < words; ++i) {
            uint32_t w;
            std::memcpy(&w, in + 4 * i, 4);
            const uint32_t r = ((w & 0xFF00FFFFu) >> 8) | ((w & 0x00FF00FFu) << 8);
            std::memcpy(out + 4 * i, &r, 4);
        }
        if ((size & 3) != 0) {
            // Tail (partial word): the original writes two bytes, low from in[4n+1], high from in[4n].
            out[4 * words] = in[4 * words + 1];
            out[4 * words + 1] = in[4 * words];
        }
        return 1;
    }
    // Other packed types: each whole word is reversed (bytes 3,2,1,0). A tail is not touched.
    for (uint32_t i = 0; i < words; ++i) {
        out[4 * i + 0] = in[4 * i + 3];
        out[4 * i + 1] = in[4 * i + 2];
        out[4 * i + 2] = in[4 * i + 1];
        out[4 * i + 3] = in[4 * i + 0];
    }
    return 1;
}

}  // namespace glitch::video::pixel_format
