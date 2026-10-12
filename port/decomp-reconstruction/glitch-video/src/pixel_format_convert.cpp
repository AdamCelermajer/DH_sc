// pixel_format conversion, the table-driven part. See pixel_format_convert.hpp for the scope.
//
// Evidence (libDungeonHunter2.so, ARM, Hex-Rays pseudocode and listing):
//   convert            0x5f95ac  top-level dispatch (lines 756-830 of the pseudocode)
//   convertPacked      0x5f4fe8  switch on getPackedType(dst) | 4 * getPackedType(src); arms 0, 5, 0xA call Impl
//   convertPackedImpl  0x5eeef4 (uint), 0x5f0f60 (ushort), 0x5f2fdc (uchar): G-width test at line 315,
//                      alpha test at 448 and 820, converter constructors at 319, 681, 1031
//   converter ctors    0x5ee790 (LUT for all channels), 0x5ee958 (replicate), 0x5ee5e8 (alpha LUT)
//   LUT tables         0x8e3694 (1-bit), 0x8e3698 (2-bit), 0x8e36a0 (4-bit): 0000 ffff / 0000 5555 aaaa ffff /
//                      0000 1111 ... ffff, that is entry i = i * (0xffff / (2^bits - 1))
#include "glitch-video/pixel_format_convert.hpp"

#include <vector>

namespace glitch::video::pixel_format {

namespace {

using detail::PFDTable;
using detail::SFormatEntry;

// ARM register shifts by 32 or more give zero.
uint32_t shr(uint32_t value, unsigned shift) { return shift >= 32 ? 0 : value >> shift; }
uint32_t shl(uint32_t value, unsigned shift) { return shift >= 32 ? 0 : value << shift; }

bool lutBitsSupported(unsigned bits) { return bits == 1 || bits == 2 || bits == 4; }

// Entry of the 1/2/4-bit table: the index replicated to 16 bits.
uint32_t lutEntry(unsigned bits, uint32_t index) { return index * (0xffffu / ((1u << bits) - 1u)); }

// Pixels are little-endian words of 1 to 4 bytes (the ARM copies with memcpy into a word).
uint32_t loadPixel(const uint8_t* p, unsigned bytes) {
    uint32_t v = 0;
    for (unsigned i = 0; i < bytes; ++i) v |= static_cast<uint32_t>(p[i]) << (8 * i);
    return v;
}

void storePixel(uint8_t* p, unsigned bytes, uint32_t v) {
    for (unsigned i = 0; i < bytes; ++i) p[i] = static_cast<uint8_t>(v >> (8 * i));
}

bool isPixelSized(const SFormatEntry& e) {
    return (e.flags & 8) == 0 && e.bytesPerBlock >= 1 && e.bytesPerBlock <= 4 &&
           static_cast<unsigned>(e.bitsPerPixel) == 8u * e.bytesPerBlock;
}

}  // namespace

// Converter construction, one channel at a time. Widths and shifts are the PFD row bytes
// +0x18..+0x1b (widths) and +0x1c..+0x1f (shifts); masks are the key words +0x04..+0x13.
bool buildPackedPlan(E_PIXEL_FORMAT src, E_PIXEL_FORMAT dst, SPackedPlan& plan) {
    if (src < 0 || src >= detail::kFormatCount || dst < 0 || dst >= detail::kFormatCount) return false;
    const SFormatEntry& s = PFDTable[src];
    const SFormatEntry& d = PFDTable[dst];
    if (!isPixelSized(s) || !isPixelSized(d)) return false;

    const unsigned gs = s.unknown18[1], gd = d.unknown18[1];  // green widths
    const unsigned as = s.unknown18[3], ad = d.unknown18[3];  // alpha widths
    const bool srcHasAlphaFlag = (s.flags & 1) != 0;
    const bool dstHasAlphaFlag = (d.flags & 1) != 0;

    // Dispatch of convertPackedImpl (0x5eeef4): line 315 tests the green widths (gs != 0, gd != 0, gs < gd);
    // line 319 takes the LUT converter when gd > 2 * gs; line 448 tests the alpha widths for the
    // alpha-upscale converters, which are not modelled; line 820 tests them for SPackedConverter<0,2>.
    const bool greenUpscale = gd != 0 && gs != 0 && gs < gd;
    const bool alphaUpscale = ad != 0 && as != 0 && as < ad;

    if (greenUpscale) {
        if (gd > 2 * gs) {
            plan.mode = EPackedMode::kLut;
        } else {
            if (alphaUpscale) return false;  // line 448 branch: SAlphaUpscale / SAlphaIndexed, not modelled
            plan.mode = EPackedMode::kReplicate;
        }
    } else if (alphaUpscale) {
        plan.mode = EPackedMode::kAlphaLut;  // line 820 branch: SPackedConverter<0,2>
    } else {
        plan.mode = EPackedMode::kReplicate;  // plain branch: only reductions and equal widths are modelled
    }

    // Alpha is filled with the destination alpha mask when the destination has the alpha flag and the
    // source has none (ctor 0x5ee958 line: row +0x18 = (dst flags & 1) == 0 || (src flags & 1) ? 0 : -1, then & dst key 3).
    plan.orValue = (dstHasAlphaFlag && !srcHasAlphaFlag) ? d.key[3] : 0;

    for (int c = 0; c < 4; ++c) {
        SPackedChannel& ch = plan.channel[c];
        const unsigned sb = s.unknown18[c];
        const unsigned db = d.unknown18[c];
        const unsigned ss = s.unknown18[4 + c];
        const unsigned ds = d.unknown18[4 + c];
        ch.dstMask = d.key[c];
        ch.srcMask = s.key[c];
        ch.srcBits = static_cast<uint8_t>(sb);
        ch.dstBits = static_cast<uint8_t>(db);
        ch.dstShift = static_cast<uint8_t>(ds);
        ch.lutRightShift = static_cast<uint8_t>(16u - db);

        // Shifts of the first term (ctor loop, 0x5ee958 lines 26-45 of the pseudocode).
        unsigned rs = ss, ls = ds;
        if (db == 0) {
            rs = ss;
            ls = ds;
        } else if (sb == 0) {
            // Source channel absent: the listing shifts the raw word. Only valid when the alpha
            // fill covers the channel (its result is then fully overwritten).
            if (!(c == 3 && plan.orValue == d.key[3] && plan.orValue != 0)) return false;
        } else if (sb > db) {
            rs = ss + sb - db;  // reduction: top db bits of the source field
            ls = ds;
        } else if (db <= 2 * sb) {
            ls = ds + db - sb;  // expansion up to 2x: top aligned
        } else {
            ls = ds;  // more than 2x: only the LUT converters use this, checked below
        }
        ch.rightShift = static_cast<uint8_t>(rs);
        ch.leftShift = static_cast<uint8_t>(ls);
        ch.replicaMask = 0;
        ch.replicaRightShift = 0;

        const bool lutChannel = (plan.mode == EPackedMode::kLut && c < 3) || (plan.mode == EPackedMode::kAlphaLut && c == 3);
        const bool replicateChannel = plan.mode == EPackedMode::kReplicate && c < 3;
        const bool expands = db > sb && sb != 0;

        if (db == 0) continue;
        if (lutChannel) {
            if (!lutBitsSupported(sb) || db > 16) return false;  // null table pointer in the listing
            continue;
        }
        if (replicateChannel) {
            if (expands) {
                if (db > 2 * sb) return false;  // needs the LUT converter, not this one
                const unsigned t = 2 * sb - db;  // replication shift
                ch.replicaMask = ch.srcMask & (ch.srcMask << t);
                ch.replicaRightShift = static_cast<uint8_t>(rs + t);
            }
            // Reductions and equal widths: the second term is zero (replicaMask stays 0).
            continue;
        }
        // Plain shift on RGB (kAlphaLut) or on alpha in kReplicate / kLut.
        if (c < 3 && expands) {
            if (plan.mode == EPackedMode::kAlphaLut && db > 2 * sb) return false;
            if (plan.mode == EPackedMode::kReplicate && !greenUpscale) return false;  // plain branch expansions not modelled
        }
        if (c == 3 && expands && plan.mode == EPackedMode::kReplicate) return false;
    }
    return true;
}

// Per-pixel formula. Listing: the RGB terms of kReplicate are (x & K) >> rs << ls | (x & K2) >> rs2 << ls2;
// kLut RGB terms are the 16-bit table value >> (16 - dst bits) << ls; alpha in kReplicate and kLut and RGB in
// kAlphaLut are the unmasked x >> rs << ls. Every term is ANDed with the dst key, then OR'ed with the fill.
uint32_t convertPixel(const SPackedPlan& plan, uint32_t x) {
    uint32_t out = plan.orValue;
    for (int c = 0; c < 4; ++c) {
        const SPackedChannel& ch = plan.channel[c];
        if (ch.dstMask == 0) continue;
        uint32_t term;
        const bool lut = (plan.mode == EPackedMode::kLut && c < 3) || (plan.mode == EPackedMode::kAlphaLut && c == 3);
        if (lut) {
            const uint32_t index = shr(x & ch.srcMask, ch.rightShift);
            term = shl(shr(lutEntry(ch.srcBits, index), ch.lutRightShift), ch.leftShift);
        } else if (plan.mode == EPackedMode::kReplicate && c < 3) {
            term = shl(shr(x & ch.srcMask, ch.rightShift), ch.leftShift) |
                   shl(shr(x & ch.replicaMask, ch.replicaRightShift), ch.dstShift);
        } else {
            term = shl(shr(x, ch.rightShift), ch.leftShift);
        }
        out |= term & ch.dstMask;
    }
    return out;
}

// 0x5f4fe8 convertPacked.
int convertPacked(E_PIXEL_FORMAT srcFormat, const void* src, uint32_t srcPitch, E_PIXEL_FORMAT dstFormat, void* dst,
                  uint32_t dstPitch, uint32_t width, uint32_t height, bool flip) {
    const int key = getPackedType(dstFormat) | (4 * getPackedType(srcFormat));
    if (key != 0 && key != 5 && key != 0xA) return 0;  // inline arms 1, 2, 4, 6, 8, 9 are not modelled

    SPackedPlan plan;
    if (!buildPackedPlan(srcFormat, dstFormat, plan)) return 0;
    if (width == 0 || height == 0) return 0;  // loop bounds at zero are not checked in the listing

    const SFormatEntry& s = PFDTable[srcFormat];
    const SFormatEntry& d = PFDTable[dstFormat];
    const unsigned sb = s.bytesPerBlock, db = d.bytesPerBlock;

    const uint8_t* in = static_cast<const uint8_t*>(src);
    uint8_t* out = static_cast<uint8_t*>(dst);
    std::vector<uint8_t> copy;
    if (src == dst) {
        // In place: read every source row before any row is written.
        const size_t bytes = static_cast<size_t>(height - 1) * srcPitch + static_cast<size_t>(width) * sb;
        copy.assign(in, in + bytes);
        in = copy.data();
    }

    for (uint32_t r = 0; r < height; ++r) {
        const uint8_t* srow = in + static_cast<size_t>(r) * srcPitch;
        uint8_t* drow = out + static_cast<size_t>(flip ? height - 1 - r : r) * dstPitch;
        for (uint32_t i = 0; i < width; ++i) {
            storePixel(drow + static_cast<size_t>(i) * db, db, convertPixel(plan, loadPixel(srow + static_cast<size_t>(i) * sb, sb)));
        }
    }
    return 1;
}

// 0x5f95ac convert.
int convert(E_PIXEL_FORMAT srcFormat, const void* src, uint32_t srcPitch, E_PIXEL_FORMAT dstFormat, void* dst,
            uint32_t dstPitch, uint32_t width, uint32_t height, bool flip) {
    if (srcPitch == 0) srcPitch = computePitch(srcFormat, width);
    if (dstPitch == 0) dstPitch = computePitch(dstFormat, width);

    if (srcFormat == dstFormat) return 0;  // copy() branch, body not analysed
    if (src == dst) {
        // In place needs equal pitches (the listing logs and returns 0 here).
        if (computePitch(srcFormat, width) != computePitch(dstFormat, width) || srcPitch != dstPitch) return 0;
    }
    const SFormatEntry& d = PFDTable[dstFormat];
    const SFormatEntry& s = PFDTable[srcFormat];
    if ((d.flags & 8) != 0) return 0;  // "Compressed destination format not supported"
    if ((s.flags & 8) != 0) return 0;  // decompress() branch, not modelled

    // Outer test of 0x5f95ac: the luminance flag (0x4) of the destination must be clear, or set on the source.
    if (!((d.flags & 4) == 0 || (s.flags & 4) != 0)) return 0;  // else arm: packed-type switch, not modelled

    const bool enter = s.packedType != d.packedType || (s.flags & 0x40) != 0 || (d.flags & 0x40) != 0 ||
                       ((d.flags & 1) != 0 && (s.flags & 1) == 0);
    if (!enter && srcFormat != 2 && dstFormat != 2) return 0;  // same-type arm: not modelled

    if (((s.flags | d.flags) & 2) != 0) return 0;  // flag-2 switch (float and half formats), not modelled
    if (static_cast<unsigned>(srcFormat - 10) <= 1u) return 0;  // 24-bit special path (formats 10 and 11)

    return convertPacked(srcFormat, src, srcPitch, dstFormat, dst, dstPitch, width, height, flip);
}

}  // namespace glitch::video::pixel_format
