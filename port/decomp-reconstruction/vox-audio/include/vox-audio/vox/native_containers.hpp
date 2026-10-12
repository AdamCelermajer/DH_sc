// Containers of the native decoder: the chunk and segment tables, the state id list, the
// transition rules and the transition parameter records. The original types are vector
// containers (the mangled names NativeChunks, States, AudioSegments, TransitionRules and
// TransitionParams). Word offsets from the 32-bit pseudocode are kept as comments.
#pragma once

#include <cstdint>
#include <vector>

namespace vox {

// One entry of the chunk table, 24 bytes in the original.
struct NativeChunk {
    std::int32_t byte_offset = 0;      // +0x00: start of the chunk, relative to base_offset_
    std::int32_t byte_size = 0;        // +0x04: size of the chunk in bytes
    std::int32_t frame_count = 0;      // +0x08: frames in the chunk
    std::int32_t reserved_0c[3] = {};  // +0x0c..+0x17: not read by the recovered code
};

// One transition rule of the native decoder. 36 bytes in the original.
struct TransitionRule {
    std::int32_t kind_ = 0;      // +0x00: 1 starts a new segment
    std::int32_t type_ = 0;      // +0x04: 0 plain, 6 looped (see IsExtraSegmentNeeded)
    std::int32_t field_08_ = 0;  // +0x08
    std::int32_t flag_0c_ = 0;   // +0x0c: 1 selects the reversed path
    float start_ = 0.0f;         // +0x10
    float end_ = 0.0f;           // +0x14
    float length_ = 0.0f;        // +0x18: length in seconds (scaled by the base field_0c_)
    std::int32_t field_1c_ = 0;  // +0x1c
    std::int32_t field_20_ = 0;  // +0x20
};

// One transition parameter entry, 8 bytes in the original: an int and a byte, pushed by the
// Trsn loop of ParseFile (the vector growth steps by 8 bytes). The int is record word 2 (-1
// when the record is short) and the byte is record byte 12 (0 when short).
struct TransitionParams {
    std::int32_t value_ = -1;  // +0x00
    std::uint8_t flag_ = 0;    // +0x04
};

// vox::NativeChunks: the chunk table, a vector of 24-byte records.
using NativeChunks = std::vector<NativeChunk>;

// vox::AudioSegments: the segment table of the decoder (24-byte records). The segment code
// reads it as the chunk table, so it is the same vector as NativeChunks.
using AudioSegments = NativeChunks;

// vox::States: the state ids, one 32-bit id each.
using States = std::vector<std::int32_t>;

// vox::TransitionRules: the transition rules, 36-byte records.
using TransitionRules = std::vector<TransitionRule>;

// The VoxN header words that the sub-decoder constructors read (the decoder's header block,
// the words at decoder +0x18, +0x24, +0x28 and +0x2c). The format word holds the format tag
// in its low half (17 selects IMA ADPCM, 1 PCM) and the channel count in its high half. The
// sample word holds the bytes per sample in its low half and the bits per sample in its high
// half.
struct NativeHeaderWords {
    std::int32_t base_offset = 0;   // decoder +0x18: base_offset_
    std::int32_t format_word = 0;   // decoder +0x24: format tag (low), channels (high)
    std::int32_t field_0c = 0;      // decoder +0x28: field_0c_
    std::int32_t sample_word = 0;   // decoder +0x2c: bytes per sample (low), bits (high)
};

}  // namespace vox
