#include "glitch-video/pixel_format.hpp"

namespace glitch::video::pixel_format {

namespace detail {
// Placeholder: the .rodata table is not in the repo, so every entry is zero.
// PFDTable is defined in pixel_format_table.cpp (bytes from the original binary).
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
// (C1 only; the C2 form is not in the ARM listing.) Partial: PFDTable is zero-filled in
// this port, so the divisions yield inf/NaN until the table data is supplied.
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

}  // namespace glitch::video::pixel_format
