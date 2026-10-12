// glitch::video::pixel_format conversion: pixel_format::convert (0x5f95ac) and the packed path
// (convertPacked 0x5f4fe8, convertPackedImpl<T,T> 0x5f2fdc / 0x5f0f60 / 0x5eeef4).
//
// The original builds a converter object per format pair from PFDTable rows (see
// pixel_format.hpp for the row layout) and runs one per-pixel formula over every row. This port
// keeps that structure: buildPackedPlan() reproduces the converter constructors, convertPixel()
// reproduces the per-pixel expression, and convertPacked() runs the row loop.
//
// Coverage: only the branches listed in NOTES.md (glitch-video, section "pixel_format convert")
// are modelled. Every other branch returns 0 and writes nothing, so a caller can tell an
// unsupported pair from a converted one.
#pragma once

#include <cstdint>

#include "glitch-video/pixel_format.hpp"

namespace glitch::video::pixel_format {

// Per-channel parameters, built from the PFDTable row of the source and the destination.
// Channel 0 is R (key word 0), 1 is G, 2 is B and 3 is A.
struct SPackedChannel {
    uint32_t dstMask = 0;        // dst key word c (dst row +0x04 + 4c)
    uint32_t srcMask = 0;        // src key word c (src row +0x04 + 4c)
    uint32_t replicaMask = 0;    // srcMask & (srcMask << (2*srcBits - dstBits)), replication term
    uint8_t srcBits = 0;         // src row +0x18 + c
    uint8_t dstBits = 0;         // dst row +0x18 + c
    uint8_t rightShift = 0;      // +0x10 + c in the converter (src shift, or reduction shift)
    uint8_t leftShift = 0;       // +0x14 + c in the converter
    uint8_t replicaRightShift = 0;  // +0x34 + c in the converter: rightShift + (2*srcBits - dstBits)
    uint8_t lutRightShift = 0;   // 16 - dstBits, used after a lookup
    uint8_t dstShift = 0;        // dst row +0x1c + c
};

// The converter type that the listing selects for a pair.
enum class EPackedMode : uint8_t {
    kReplicate = 1,  // SPackedConverter<1,0>: RGB with bit replication, alpha as a plain shift
    kLut = 2,        // SRGBIndexedPackedConverter: RGB through a 1/2/4-bit table, alpha a plain shift
    kAlphaLut = 3,   // SPackedConverter<0,2>: RGB as plain shifts, alpha through a 1/2/4-bit table
};

struct SPackedPlan {
    EPackedMode mode = EPackedMode::kReplicate;
    SPackedChannel channel[4] = {};
    uint32_t orValue = 0;  // dst alpha mask when the destination has alpha and the source does not
};

// Builds the plan for a pair of uncompressed packed formats. Returns false when the listing
// branch for the pair is not modelled (see the coverage note above).
bool buildPackedPlan(E_PIXEL_FORMAT src, E_PIXEL_FORMAT dst, SPackedPlan& plan);

// One pixel: src is the source pixel as a little-endian word (bytes per pixel from the row).
uint32_t convertPixel(const SPackedPlan& plan, uint32_t src);

// 0x5f4fe8 convertPacked: dispatch on getPackedType of both formats, then the row loop. Returns 1
// when converted, 0 when the switch arm is not covered.
int convertPacked(E_PIXEL_FORMAT srcFormat, const void* src, uint32_t srcPitch, E_PIXEL_FORMAT dstFormat,
                  void* dst, uint32_t dstPitch, uint32_t width, uint32_t height, bool flip);

// 0x5f95ac convert: top-level dispatch. Returns 1 when converted, 0 when not converted (the copy,
// compressed, flag-2 and luminance-else branches are not modelled, see NOTES).
int convert(E_PIXEL_FORMAT srcFormat, const void* src, uint32_t srcPitch, E_PIXEL_FORMAT dstFormat, void* dst,
            uint32_t dstPitch, uint32_t width, uint32_t height, bool flip);

}  // namespace glitch::video::pixel_format
