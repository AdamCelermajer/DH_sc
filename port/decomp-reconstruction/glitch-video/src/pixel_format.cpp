#include "glitch-video/pixel_format.hpp"

namespace glitch::video::pixel_format {

namespace detail {
// Placeholder: the .rodata table is not in the repo, so every entry is zero.
SFormatEntry PFDTable[kFormatCount] = {};
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

}  // namespace glitch::video::pixel_format
