// glitch::video::pixel_format conversion: pixel_format::convert (0x5f95ac) and the packed path
// (convertPacked 0x5f4fe8, convertPackedImpl<T,T> 0x5f2fdc / 0x5f0f60 / 0x5eeef4).
//
// The original builds a converter object per format pair from PFDTable rows (see
// pixel_format.hpp for the row layout) and runs one per-pixel formula over every row. This port
// keeps that structure: buildPackedPlan() reproduces the converter constructors, convertPixel()
// reproduces the per-pixel expression, and convertPacked() runs the row loop.
//
// Coverage: the branches listed in NOTES.md (glitch-video, section "pixel_format convert") are
// modelled. Every other branch returns 0 and writes nothing, so a caller can tell an unsupported
// pair from a converted one. Still unmodelled: the same-type arm of convert (0x5f9670..0x5f9950),
// the packed dispatch that the "12 packed types include neither format 2" arm reaches, the
// decompress() call and the 24-bit arm (source field17 1 or 4 with the destination's +0x17 in
// {2,3,4}).
#pragma once

#include <cstdint>

#include "glitch-video/pixel_format.hpp"

namespace glitch::video::pixel_format {

// Per-channel operation that the converter of the pair applies to this channel.
enum class EChannelOp : uint8_t {
    kNone = 0,        // the destination channel is absent (dst width or mask 0)
    kMaskedShift = 1, // (x & srcMask) >> a << b, plus the replication term when the destination is wider
    kLut = 2,         // a 1/2/4-bit table lookup (SRGBIndexedPackedConverter, alpha of <0,2>)
    kPlainShift = 3,  // x >> a << b, no source mask (alpha of <1,0>/<2,0>, RGB of <0,2>)
    kAlphaUpscale = 4,  // alpha of SAlphaUpscalePackedConverter: masked shift plus the replica term
    kAlphaUpscaleLut = 5,  // alpha of SAlphaIndexedPackedConverter: a lookup plus a plain shift
};

// Per-channel parameters, built from the PFDTable row of the source and the destination.
// Channel 0 is R (key word 0), 1 is G, 2 is B and 3 is A.
struct SPackedChannel {
    uint32_t dstMask = 0;        // dst key word c (dst row +0x04 + 4c)
    uint32_t srcMask = 0;        // src key word c (src row +0x04 + 4c)
    uint32_t replicaMask = 0;    // srcMask & (srcMask << (2*srcBits - dstBits)), replication term
    uint8_t srcBits = 0;         // src row +0x18 + c
    uint8_t dstBits = 0;         // dst row +0x18 + c
    uint8_t rightShift = 0;      // +0x10 + c in the converter: the shift of the first term
    uint8_t leftShift = 0;       // +0x14 + c in the converter: the shift of the first term
    uint8_t replicaRightShift = 0;  // +0x34 + c in the converter: rightShift - (dstBits - srcBits)
    uint8_t lutRightShift = 0;   // 16 - dstBits, used after a lookup
    uint8_t lutIndexShift = 0;   // src row +0x1c + c: the shift applied before the lookup index
    uint8_t dstShift = 0;        // dst row +0x1c + c
    EChannelOp op = EChannelOp::kNone;
};

// The converter type that the listing selects for a pair. kAlphaUpscale and kAlphaIndexed are the
// SAlphaUpscale / SAlphaIndexed wrappers, which replace the alpha channel of the underlying
// converter; every converter whose destination has no alpha reduces to its RGB form.
enum class EPackedMode : uint8_t {
    kReplicate = 1,   // SPackedConverter<1,0>: RGB with bit replication, alpha as a plain shift
    kLut = 2,         // SRGBIndexedPackedConverter: RGB through a 1/2/4-bit table, alpha a plain shift
    kAlphaLut = 4,    // SPackedConverter<0,2>: RGB as plain shifts, alpha through a 1/2/4-bit table
    kAlphaUpscale = 5,     // SAlphaUpscalePackedConverter: plain-shift RGB, replicated alpha
    kAlphaIndexed = 6,     // SAlphaIndexedPackedConverter: plain-shift RGB, LUT alpha
};

struct SPackedPlan {
    EPackedMode mode = EPackedMode::kReplicate;
    SPackedChannel channel[4] = {};
    uint32_t orValue = 0;  // dst alpha mask when the destination has alpha and the source does not
};

// Builds the plan for a pair of uncompressed packed formats. `key` is convertPacked's switch key
// (getPackedType(dst) | 4 * getPackedType(src)); the key selects the converter template for the
// pair, and with it the per-channel operation. Returns false when the listing branch for the pair
// is not modelled (see the coverage note above).
bool buildPackedPlan(E_PIXEL_FORMAT src, E_PIXEL_FORMAT dst, int key, SPackedPlan& plan);

// One pixel: src is the source pixel as a little-endian word (bytes per pixel from the row).
uint32_t convertPixel(const SPackedPlan& plan, uint32_t src);

// 0x5f4fe8 convertPacked: dispatch on getPackedType of both formats, then the row loop. Returns 1
// when converted, 0 when the switch arm is not covered.
int convertPacked(E_PIXEL_FORMAT srcFormat, const void* src, uint32_t srcPitch, E_PIXEL_FORMAT dstFormat,
                  void* dst, uint32_t dstPitch, uint32_t width, uint32_t height, bool flip);

// 0x5f95ac convert: top-level dispatch. Returns 1 when converted, 0 when not converted. The
// srcFormat == dstFormat branch is the original's copy() (0x5ee40c, reconstructed in the .cpp);
// the packed arms, the luminance arms and the flag-2 refusal are modelled. Still not modelled:
// the same-type arm, the packed dispatch both-packed-type pairs reach, decompress() and the 24-bit
// arm, see the coverage note above.
int convert(E_PIXEL_FORMAT srcFormat, const void* src, uint32_t srcPitch, E_PIXEL_FORMAT dstFormat, void* dst,
            uint32_t dstPitch, uint32_t width, uint32_t height, bool flip);

}  // namespace glitch::video::pixel_format
