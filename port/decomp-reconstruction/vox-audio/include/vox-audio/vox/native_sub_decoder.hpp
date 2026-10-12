// vox::VoxNativeSubDecoder and its PCM variant. Only the methods recovered so far are
// declared; the rest of the native sub-decoder (segments, state, decode) is not ported,
// so the base has no pure virtuals and the classes here are concrete.
#pragma once

#include <cstdint>
#include <vector>

#include "vox-audio/vox/decoder.hpp"

namespace vox {

struct TransitionRule;    // defined with the native segment code (not ported yet)
class NativeSubDecoderPCMState;  // native_sub_decoder_state.hpp

// Stand-ins for the native segment layouts. The original NativeChunks, SegmentState and
// AudioSegments containers are not ported; only the words the recovered sub-decoder code
// reads are named. Offsets are the 32-bit ones from the pseudocode.

// One entry of the chunk table, 24 bytes in the original.
struct NativeChunk {
    std::int32_t byte_offset = 0;   // +0x00: start of the chunk, relative to base_offset_
    std::int32_t byte_size = 0;     // +0x04: size of the chunk in bytes
    std::int32_t frame_count = 0;   // +0x08: frames in the chunk
    std::int32_t reserved_0c[3] = {};  // +0x0c..+0x17: not read by the recovered code
};

// vox::NativeChunks stand-in. The chunk table is the vector at +0x04.
struct NativeChunks {
    std::int32_t unknown_00 = 0;    // +0x00: not read by the recovered code
    std::vector<NativeChunk> chunks;  // +0x04
};

// vox::SegmentState: playback position of one segment (0x44 bytes). Field names come from
// how the recovered code uses each word; their exact meaning is not confirmed.
struct SegmentState {
    std::int32_t chunk_index = 0;     // +0x00: index into the chunk table
    std::int32_t play_mode = 0;       // +0x04: compared with 1 and 3 by the decoders
    std::int32_t byte_offset = 0;     // +0x08: byte position inside the chunk
    std::int32_t sample_position = 0; // +0x0c: frame position inside the chunk
    std::int32_t loop_start = 0;      // +0x10: loop start frame (second entry of the loop list)
    std::int32_t end_frame = 0;       // +0x14: frame where the current loop entry ends
    std::int32_t entry_count = 0;     // +0x18: length of the loop list of the chunk
    std::int32_t entries_left = 0;    // +0x1c: countdown to the next loop-list entry
    std::int32_t restart_at_end = 0;  // +0x20: 1 means the list restarts at its last entry
    std::int32_t state = 0;           // +0x24: 3 and 4 are tested by the decoders
    std::int32_t reserved_28[5] = {}; // +0x28..+0x37: not read by the recovered code
    std::int32_t decoding_slot = 0;   // +0x3c: decoding-buffer slot (IMA ADPCM)
    bool reset_pending = false;       // +0x40: set when the state is 3 (IMA ADPCM)
};

class VoxNativeSubDecoder {
public:
    virtual ~VoxNativeSubDecoder();  // D1 0x8841f8 (empty); D0 0x8847d4 is the deleting variant
    virtual bool HasData();          // 0x884468: returns the flag at +0x16c

    // 0x884438: the track parameters {channels, +0x0c word, bits per sample, 0}.
    TrackParams GetTrackParams();
    // 0x884f04: frees the shared mixing buffer. The original leaves the pointer set.
    static void Clean();
    // 0x885258 (partial): returns the frame count, the byte count divided by the frame
    // size (the mixing body is not ported). The pseudocode signature is void; the ARM
    // tail-calls __aeabi_idiv, so the quotient is returned.
    int MixMultipleSegments(std::int16_t* dst, int bytes);
    // 0x8841fc (partial): frame count for the byte count, as MixMultipleSegments.
    int EmulateMixSegmentInBuffer(int bytes, SegmentState* segment);
    // 0x8847e8 and 0x884ab4 (partial): the 0x41-byte state copies between the segment
    // arrays are not reproduced (the arrays are not laid out in the port).
    void UpdateOldSegmentState(TransitionRule* rule);
    void UpdateDyingSegmentState(TransitionRule* rule);
    // 0x885740 (other category's row): advances the segment list. Declared here because the
    // PCM and IMA ADPCM decode loops call it; not defined in this slice.
    void UpdateSegmentsStates();

    static void* s_pMixingBuffer;    // _ZN3vox19VoxNativeSubDecoder15s_pMixingBufferE

protected:
    StreamCursorInterface* stream_ = nullptr;  // +0x04
    std::int16_t bytes_per_sample_ = 0;        // +0x10 (bytes per frame; the decoders multiply by it)
    std::int32_t base_offset_ = 0;             // +0x14: added to chunk offsets to get the stream position
    NativeChunks* chunks_ = nullptr;           // +0x18
    std::vector<std::vector<std::int32_t>>* loop_lists_ = nullptr;  // +0x2c (12-byte elements)
    SegmentState segments_[3] = {};            // +0x98: segments_[2] (+0x120) is the current segment
    std::int32_t pending_seek_ = 0;            // +0x168: frames to skip after a seek to the current segment
    bool has_data_ = false;          // +0x16c
    std::int16_t channels_ = 0;      // +0x0a
    std::int32_t field_0c_ = 0;      // +0x0c (returned as the second track parameter)
    std::int16_t bits_ = 0;          // +0x12 (bits per sample; the byte width is bits >> 3)
};

class VoxNativeSubDecoderPCM : public VoxNativeSubDecoder {
public:
    ~VoxNativeSubDecoderPCM() override;  // D2 0x887b24 and D1 0x887b28 (both empty)

    int Seek(unsigned int sample_offset);                  // 0x887b0c: returns 0
    // 0x887b38: seeks the stream to a frame of the chunk named by the segment. A negative
    // offset means the segment's loop start. Returns the stream Seek result.
    int Seek(int sample_offset, SegmentState* segment);
    // 0x887bc4: emulates decoding of up to sample_bytes bytes of the segment (no output).
    int EmulateDecodeSegment(int sample_bytes, SegmentState* segment);
    // 0x887e1c: the same for the current segment (segments_[2]) from a byte offset.
    int EmulateDecodeCurrentSegmentWithOffset(int sample_bytes);
    // 0x888050 (partial): decodes into dst, advancing the segment as it goes.
    int DecodeSegment(void* dst, int bytes, SegmentState* segment);
    // 0x8882ac (partial): the same for the current segment, after a pending seek.
    int DecodeCurrentSegmentWithOffset(void* dst, int bytes);
    int GetDecodingBuffer();                               // 0x887b14: returns -1
    void ReleaseDecodingBuffer(int buffer);                // 0x887b1c (empty)
    void SetDecodingBufferToSegmentPosition(SegmentState* segment);  // 0x887b20 (empty)
    int GetBytePositionFromSampleOffset(unsigned int sample_offset);  // 0x887b2c
    // 0x887bb8, 0x887bbc, 0x887bc0: empty bodies (the ARM is a bare return).
    void Reset();
    void SetState(NativeSubDecoderPCMState* state);
    void GetState(NativeSubDecoderPCMState* state);

};

}  // namespace vox
