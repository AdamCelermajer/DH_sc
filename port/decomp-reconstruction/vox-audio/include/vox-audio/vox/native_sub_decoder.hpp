// vox::VoxNativeSubDecoder and its PCM variant. The base class holds the native segment
// state: the chunk table, three segment records, the playlist elements that the segment
// code reads, and the counters that the state copies in SetState and GetState carry.
// Offsets are the 32-bit ARM offsets from the pseudocode, kept as comments. Several virtual
// slots are not named yet, so the methods that dispatch through them are partial.
#pragma once

#include <cstdint>
#include <vector>

#include "vox-audio/vox/decoder.hpp"
#include "vox-audio/vox/native_playlist.hpp"

namespace vox {

class NativeSubDecoderPCMState;  // native_sub_decoder_state.hpp
class NativeSubDecoderState;     // native_sub_decoder_state.hpp

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
    std::int32_t remaining_frames = 0;  // +0x28: frames left to mix (segment code)
    std::int32_t field_2c = 0;        // +0x2c
    std::int32_t ramp_frames = 0;     // +0x30: frames left in the gain ramp
    std::int32_t ramp_step = 0;       // +0x34: gain change per frame
    std::int32_t gain = 0;            // +0x38: Q15 gain
    std::int32_t decoding_slot = 0;   // +0x3c: decoding-buffer slot (IMA ADPCM)
    bool reset_pending = false;       // +0x40: set when the state is 3 (IMA ADPCM)

    // The record StopSegment leaves and the state constructor starts from: index -1,
    // state 2, the flags at 1, the rest 0, decoding slot -1.
    void SetStopped();
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

    // 0x8842e8 (partial): the two decode calls through virtual slots 0x30 and 0x34 are
    // not named, so their frame counts are taken as 0.
    int EmulateMixMultipleSegments(int bytes);
    // 0x8843b8: which life state the next dying segment has (1 or 2).
    int GetNextDyingSegmentLifeState();
    // 0x884470: whether an extra segment is needed for the rule (nullptr for none).
    bool IsExtraSegmentNeeded(TransitionRule* rule);
    // 0x8844e8 (partial): updates the frame counters and the gain of the segment. The
    // sample mixing loops and the return value are not reproduced.
    int MixSegmentInBuffer(std::int16_t* src, int bytes, SegmentState* segment);
    // 0x884734 (partial): the call through virtual slot 0x24 for mode 17 is not made.
    void StopSegment(SegmentState* segment);
    // 0x884bc4 (partial): the no-element branch is reproduced; the rule branch needs the
    // virtual slot results and the frame ramp, which are not ported.
    int UpdateCurrentSegmentState(TransitionRule* rule, bool extra);
    // 0x884f3c (partial): copies every field except the playlist manager, whose SetState
    // is not ported.
    void SetState(NativeSubDecoderState* state);
    // 0x88506c (partial): the reverse of SetState, with the same missing manager call.
    void GetState(NativeSubDecoderState* state);
    // 0x88519c: resets the playlists and the segment records to their start values.
    void Reset();
    // 0x885538: swaps the second and third segment records and sets their kinds.
    void SwapOldAndCurrentSegments();
    // 0x8855e8 (partial): element lookup through NativePlaylistsManager is not called here;
    // the current element is cleared.
    void ApplyTransitionRule(TransitionRule* rule);
    // 0x885740 (partial): the history shift and the no-element branch run; the playlist
    // element lookup and the transition rule branch are not called (see ApplyTransitionRule).
    void UpdateSegmentsStates();
    // 0x885984 (partial): the transition interpretation needs the TransitionRules table
    // and the playlist peek, neither ported.
    void InterpretTransitionRule(int index);
    // 0x885a3c (partial): stores the index and dispatches on the stored element; the list
    // lookups that fill the element and the state word are not ported.
    void SetState(int index);
    // 0x885ac0 (partial): the stop, mode 17 and counter logic is reproduced; the decode
    // call through slot 0x34 is not.
    int EmulateDecode(int bytes);
    // 0x885c1c (partial): the stop logic and the mix for two or more segments are
    // reproduced; the single-segment decode through slot 0x18 is not.
    int Decode(void* dst, int bytes);

    static void* s_pMixingBuffer;    // _ZN3vox19VoxNativeSubDecoder15s_pMixingBufferE

protected:
    StreamCursorInterface* stream_ = nullptr;  // +0x04
    std::uint16_t mode_ = 0;                   // +0x08: 17 selects the extra path in StopSegment
    std::int16_t bytes_per_sample_ = 0;        // +0x10 (bytes per frame; the decoders multiply by it)
    std::int32_t base_offset_ = 0;             // +0x14: added to chunk offsets to get the stream position
    NativeChunks* chunks_ = nullptr;           // +0x18
    void* transition_rules_ = nullptr;         // +0x1c (TransitionRules*, not ported)
    void* states_ = nullptr;                   // +0x20 (States*, not ported)
    void* field_24_ = nullptr;                 // +0x24
    void* field_28_ = nullptr;                 // +0x28
    std::vector<std::vector<std::int32_t>>* loop_lists_ = nullptr;  // +0x2c (12-byte elements)
    NativePlaylistsManager* playlists_ = nullptr;  // +0x30
    std::int32_t field_34_ = -1;               // +0x34
    std::int32_t field_38_ = -1;               // +0x38
    std::int32_t field_3c_ = -1;               // +0x3c
    std::int32_t field_40_ = -1;               // +0x40
    std::int32_t field_44_ = -1;               // +0x44
    std::int32_t field_48_ = -1;               // +0x48
    std::int32_t reset_pending_ = 0;           // +0x4c (a byte in the original; Reset sets 1)
    PlaylistElement prev_prev_element_;        // +0x50
    PlaylistElement prev_element_;             // +0x64
    PlaylistElement current_element_;          // +0x78
    std::int32_t field_8c_ = -1;               // +0x8c
    std::int32_t field_90_ = -1;               // +0x90
    std::int32_t active_segments_ = 0;         // +0x94: count of playing segments
    SegmentState segments_[3] = {};            // +0x98: segments_[0] (A), +0xdc (B), segments_[2] (+0x120, current)
    std::int32_t next_event_ = -1;             // +0x164
    std::int32_t pending_seek_ = 0;            // +0x168: frames to skip after a seek to the current segment
    bool has_data_ = false;                    // +0x16c
    std::int16_t channels_ = 0;                // +0x0a
    std::int32_t field_0c_ = 0;                // +0x0c (returned as the second track parameter)
    std::int16_t bits_ = 0;                    // +0x12 (bits per sample; the byte width is bits >> 3)
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
