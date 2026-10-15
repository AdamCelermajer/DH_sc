// pixel_format conversion, the table-driven part. See pixel_format_convert.hpp for the scope.
//
// Evidence (libDungeonHunter2.so, ARM, Hex-Rays pseudocode and listing):
//   convert            0x5f95ac  top-level dispatch (lines 756-830 of the pseudocode)
//   convertPacked      0x5f4fe8  switch on getPackedType(dst) | 4 * getPackedType(src); arms 0, 5, 0xA call Impl
//   convertPackedImpl  0x5eeef4 (uint), 0x5f0f60 (ushort), 0x5f2fdc (uchar): G-width test at line 315,
//                      alpha test at 448 and 820, converter constructors at 319, 681, 1031
//   converter ctors    0x5ee790 (LUT for all channels), 0x5ee958 (replicate), 0x5ee5e8 (alpha LUT)
//   copy               0x5ee40c  the same-format transfer convert dispatches to (anonymous namespace)
//   LUT tables         0x8e3694 (1-bit), 0x8e3698 (2-bit), 0x8e36a0 (4-bit): 0000 ffff / 0000 5555 aaaa ffff /
//                      0000 1111 ... ffff, that is entry i = i * (0xffff / (2^bits - 1))
#include "glitch-video/pixel_format_convert.hpp"

#include <cstddef>
#include <cstring>
#include <vector>

#include "glitch-core-io/core/process_buffer.hpp"
#include "glitch-video/deps/os_logging.hpp"

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

// 0x5ee40c _ZN6glitch5video12pixel_format12_GLOBAL__N_14copyENS0_14E_PIXEL_FORMATEPKvjPvjjjb
// The same-format transfer that convert returns for srcFormat == dstFormat (its copy() call at the top of
// 0x5f95ac). Reconstructed from the listing line by line:
//   * A compressed row (flags bit 8) with flip set logs "flipping compressed pixel format not supported"
//     (level 3) and returns 0 (0x5ee424..0x5ee440).
//   * The row count is the height divided by the row's block height (+0x25); the byte count per row is
//     computePitch(format, width) (0x5ee44c..0x5ee45c).
//   * src == dst: a different pitch logs "Bad conversion parameters" / "in-place conversion with different
//     pitch" and returns 0; an equal pitch without flip returns 1 without touching anything; with flip the
//     rows are swapped from the outside in through one row-sized block of the process buffer heap, with
//     the heap-excess flag saved, set and restored around the allocation (0x5ee468..0x5ee510).
//   * src != dst: flip writes the destination rows backwards (the destination pointer advances to the last
//     row and the step is -dstPitch); when the row pitch equals both pitches one memcpy covers the whole
//     block, otherwise one memcpy per row (0x5ee514..0x5ee5e8).
// Deviations (both host-necessitated, neither observable):
//   * A block height of 0 (only the "no row" entry, format 39) divides by zero in the listing; the ARM
//     runtime helper returns 0 there, so this port copies no rows instead of trapping.
//   * The listing adjusts the flip destination pointer by dstPitch * (rows - 1) before it tests the row
//     count; at rows == 0 that is a wrapped pointer the listing never dereferences, so this port skips the
//     adjustment and the (already skipped) loop.
//   * The in-place flip would memcpy through a null process-buffer block; this port restores the heap-excess
//     flag and returns 0 there instead of faulting.
int copy(E_PIXEL_FORMAT format, const void* src, uint32_t srcPitch, void* dst, uint32_t dstPitch, uint32_t width,
         uint32_t height, bool flip) {
    const SFormatEntry& row = PFDTable[format];
    if ((row.flags & 8) != 0 && flip) {
        os::Printer::log("flipping compressed pixel format not supported", os::ELL_ERROR);
        return 0;
    }

    const uint32_t pitch = computePitch(format, width);
    const uint32_t rows = row.blockHeight == 0 ? 0 : height / row.blockHeight;

    const auto* from = static_cast<const uint8_t*>(src);
    auto* to = static_cast<uint8_t*>(dst);

    if (from == to) {
        if (srcPitch != dstPitch) {
            os::Printer::log("Bad conversion parameters", "in-place conversion with different pitch", os::ELL_ERROR);
            return 0;
        }
        if (!flip) return 1;

        // Reverse the rows in place, one row at a time, through a single row-sized heap block.
        const bool previousExcess = core::isProcessBufferHeapExcessEnabled();
        core::setProcessBufferHeapExcessEnabled(true);
        uint8_t* temp = reinterpret_cast<uint8_t*>(core::allocProcessBuffer(static_cast<int>(pitch)));
        if (temp == nullptr) {
            core::setProcessBufferHeapExcessEnabled(previousExcess);
            return 0;
        }
        uint8_t* upper = to;
        if (rows != 0) {
            upper += static_cast<std::ptrdiff_t>(rows - 1) * srcPitch;
            uint8_t* lower = to;
            while (upper >= lower) {
                std::memcpy(temp, upper, pitch);
                std::memcpy(upper, lower, pitch);
                upper -= srcPitch;
                uint8_t* const first = lower;
                lower += srcPitch;
                std::memcpy(first, temp, pitch);
            }
        }
        core::releaseProcessBuffer(temp);
        core::setProcessBufferHeapExcessEnabled(previousExcess);
        return 1;
    }

    if (flip) {
        if (rows != 0) to += static_cast<std::ptrdiff_t>(rows - 1) * dstPitch;
        const std::ptrdiff_t step = -static_cast<std::ptrdiff_t>(dstPitch);
        for (uint32_t r = 0; r < rows; ++r) {
            std::memcpy(to, from, pitch);
            from += srcPitch;
            to += step;
        }
        return 1;
    }

    if (pitch == dstPitch && pitch == srcPitch) {
        std::memcpy(to, from, static_cast<size_t>(rows) * pitch);
        return 1;
    }
    for (uint32_t r = 0; r < rows; ++r) {
        std::memcpy(to, from, pitch);
        from += srcPitch;
        to += dstPitch;
    }
    return 1;
}

// 0x5edca4 _ZN6glitch5video12pixel_format12_GLOBAL__N_135SPackedRGBtoLuminanceAlphaConverterC1ENS0_14E_PIXEL_FORMATE
// The converter of the "destination wants luminance" arm of convert (0x5f9670..0x5f9950). Built from the
// SOURCE row: the three weights 0.3 / 0.59 / 0.11 divided by (key[i] >> shift[i]), the three channel masks
// and shifts, then the alpha mask +0x10, the alpha shift +0x1b, the scale 255 / (alphaMask >> alphaShift) at
// +0x20 and the OR value at +0x24, which is (alphaMask >> alphaShift) when the row has no alpha flag and 0
// when it has. Layout confirmed instruction by instruction against the constructor listing (the running
// +1 byte cursor at +0x18 reads the channel shifts, so the two byte arrays interleave).
struct SLuminanceConverter {
    float weight[3] = {};          // +0x00
    uint32_t channelMask[3] = {};  // +0x0c
    uint8_t channelShift[3] = {};  // +0x18
    uint8_t alphaShift = 0;        // +0x1b
    uint32_t alphaMask = 0;        // +0x1c
    float alphaScale = 0.0f;       // +0x20
    uint32_t alphaMaxBits = 0;     // +0x24

    explicit SLuminanceConverter(E_PIXEL_FORMAT src) {
        const SFormatEntry& row = PFDTable[src];
        static const float kWeights[3] = {0.3f, 0.59f, 0.11f};
        for (int i = 0; i < 3; ++i) {
            const uint32_t mask = row.key[i];
            const uint8_t shift = row.unknown18[4 + i];
            weight[i] = kWeights[i] / static_cast<float>(shr(mask, shift));
            channelMask[i] = mask;
            channelShift[i] = shift;
        }
        alphaMask = row.key[3];
        alphaShift = row.unknown18[7];
        const uint32_t maxAlpha = shr(alphaMask, alphaShift);
        alphaScale = 255.0f / static_cast<float>(maxAlpha);
        alphaMaxBits = (row.flags & 1) != 0 ? 0 : maxAlpha;
    }

    // The accumulated channel value as the listing's expression builds it: the (x & mask) >> shift terms are
    // summed as floats and the CALLER applies the destination scale and truncates once.
    float luminance(uint32_t pixel) const {
        float sum = 0.0f;
        for (int i = 0; i < 3; ++i) {
            sum += static_cast<float>(shr(pixel & channelMask[i], channelShift[i])) * weight[i];
        }
        return sum;
    }

    // (int)((float)(alphaMaxBits | ((x & alphaMask) >> alphaShift)) * alphaScale), the second byte of the
    // luminance/alpha output form.
    uint32_t alpha(uint32_t pixel) const {
        const uint32_t combined = alphaMaxBits | shr(pixel & alphaMask, alphaShift);
        return static_cast<uint32_t>(static_cast<float>(combined) * alphaScale);
    }
};

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
    std::vector<uint8_t> inPlace;
    if (src == dst) {
        // In place: read every source row before any row is written.
        const size_t bytes = static_cast<size_t>(height - 1) * srcPitch + static_cast<size_t>(width) * sb;
        inPlace.assign(in, in + bytes);
        in = inPlace.data();
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
//
// The branch structure of the listing, in its order: both pitches default to computePitch of their own
// side and width (0x5f95c8/0x5f95dc), an equal format pair returns the anonymous-namespace copy()
// (0x5f95ec), src == dst with different format pitches logs and returns 0 (0x5f9610), a compressed
// destination logs and returns 0, a compressed source goes to decompress() (0x5f9648), the luminance
// flag test (0x5f9654) opens either the luminance arm (0x5f9670..0x5f9950) or the packed dispatch
// (0x5f96b4), and LABEL_17 (0x5f96d4) refuses the float flag pair (flag bit 2) and routes formats 10/11
// to 0x5f9ebc. What this port still returns 0 for: the decompress() call, the same-type switch of the
// packed dispatch (0x5f9778, reached with matching packed types and neither format 2), the 0x5f9ebc arm
// and the inline packed arms convertPacked refuses (keys 1, 2, 4, 6, 8, 9).
int convert(E_PIXEL_FORMAT srcFormat, const void* src, uint32_t srcPitch, E_PIXEL_FORMAT dstFormat, void* dst,
            uint32_t dstPitch, uint32_t width, uint32_t height, bool flip) {
    if (srcPitch == 0) srcPitch = computePitch(srcFormat, width);
    if (dstPitch == 0) dstPitch = computePitch(dstFormat, width);

    // 0x5f95ec: the same-format path is copy(), not a conversion.
    if (srcFormat == dstFormat) return copy(srcFormat, src, srcPitch, dst, dstPitch, width, height, flip);
    if (src == dst) {
        // In place needs equal pitches (the listing logs and returns 0 here).
        if (computePitch(srcFormat, width) != computePitch(dstFormat, width) || srcPitch != dstPitch) return 0;
    }
    const SFormatEntry& d = PFDTable[dstFormat];
    const SFormatEntry& s = PFDTable[srcFormat];
    if ((d.flags & 8) != 0) return 0;  // "Compressed destination format not supported"
    if ((s.flags & 8) != 0) return 0;  // decompress() branch, not modelled

    // 0x5f9654: the luminance flag (0x4) of the destination is clear, or set on the source, before the
    // packed dispatch runs. The opposite case - the destination wants luminance and the source has none -
    // builds it from the source channels (0x5f9694).
    if ((d.flags & 4) != 0 && (s.flags & 4) == 0) {
        // The switch key is 4 * (destination byte +0x17) + (source byte +0x17). The reachable cases, given
        // the shipped table (only rows 0 and 1 carry flag 0x4, both with +0x17 == 1):
        //   0: a 16-bit source, one byte per destination pixel
        //   1: a 16-bit source, a 16-bit destination value scaled by 65535
        //   4: a 32-bit source, one byte per destination pixel
        //   5: a 32-bit source, a 16-bit destination value
        //   8: a 16-bit source, two output bytes (luminance and alpha)
        //   9: a 32-bit source, a 16-bit destination value
        // The default case (2, 3, 6 and anything else) logs "conversion to luminance from float format not
        // implemented yet" and returns 0, which is what those arms do in the listing.
        const int lumCase = 4 * d.field17 + s.field17 - 4;
        if (lumCase != 0 && lumCase != 1 && lumCase != 4 && lumCase != 5 && lumCase != 8 && lumCase != 9) {
            os::Printer::log("conversion to luminance from float format not implemented yet", os::ELL_ERROR);
            return 0;
        }
        if (width == 0 || height == 0) return 0;  // the listing tests the column and row counts
        // The per-pixel source step is the source row's +0x15. The destination step comes from the SOURCE
        // row's alpha flag: the luminance/alpha pairs write two bytes per pixel, the luminance-only pairs
        // one (and the 16-bit forms a word).
        const unsigned srcStride = s.bytesPerBlock;
        const unsigned dstStride = (s.flags & 1) != 0 ? 2u : 1u;
        const SLuminanceConverter lum(srcFormat);
        const uint8_t* in = static_cast<const uint8_t*>(src);
        uint8_t* out = static_cast<uint8_t*>(dst);
        for (uint32_t r = 0; r < height; ++r) {
            const uint8_t* srow = in + static_cast<size_t>(r) * srcPitch;
            uint8_t* drow = out + static_cast<size_t>(flip ? height - 1 - r : r) * dstPitch;
            for (uint32_t i = 0; i < width; ++i) {
                uint8_t* const pixel = drow + static_cast<size_t>(i) * dstStride;
                switch (lumCase) {
                    case 0:
                    case 4: {
                        // One byte: the weighted sum scaled by 255, truncated once. Case 4 reads a 32-bit
                        // source word, case 0 a 16-bit one.
                        const uint32_t word =
                            loadPixel(srow + static_cast<size_t>(i) * srcStride, lumCase == 4 ? 4 : 2);
                        storePixel(pixel, 1, static_cast<uint32_t>(lum.luminance(word) * 255.0f) & 0xFFu);
                        break;
                    }
                    case 1:
                    case 5:
                    case 9: {
                        // A 16-bit destination value, scaled by 65535.
                        const unsigned bytes = lumCase == 1 ? 2 : (lumCase == 5 ? 2 : 4);
                        const uint32_t word = loadPixel(srow + static_cast<size_t>(i) * srcStride, bytes);
                        storePixel(pixel, 2, static_cast<uint32_t>(lum.luminance(word) * 65535.0f));
                        break;
                    }
                    case 8: {
                        const uint32_t word = loadPixel(srow + static_cast<size_t>(i) * srcStride, 2);
                        pixel[0] = static_cast<uint8_t>(lum.luminance(word) * 255.0f);
                        pixel[1] = static_cast<uint8_t>(lum.alpha(word));
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        return 1;
    }

    const bool enter = s.packedType != d.packedType || (s.flags & 0x40) != 0 || (d.flags & 0x40) != 0 ||
                       ((d.flags & 1) != 0 && (s.flags & 1) == 0);
    if (!enter && srcFormat != 2 && dstFormat != 2) return 0;  // same-type arm: not modelled

    if (((s.flags | d.flags) & 2) != 0) return 0;  // flag-2 switch (float and half formats), not modelled
    if (static_cast<unsigned>(srcFormat - 10) <= 1u) return 0;  // 0x5f9ebc arm, not modelled

    return convertPacked(srcFormat, src, srcPitch, dstFormat, dst, dstPitch, width, height, flip);
}

}  // namespace glitch::video::pixel_format
