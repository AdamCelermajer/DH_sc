// vox::VoxNativeSubDecoder and its PCM variant. Only the methods recovered so far are
// declared; the rest of the native sub-decoder (segments, state, decode) is not ported,
// so the base has no pure virtuals and the classes here are concrete.
#pragma once

#include <cstdint>

#include "vox-audio/vox/decoder.hpp"

namespace vox {

struct SegmentState;      // defined with the native segment code (not ported yet)
struct TransitionRule;    // defined with the native segment code (not ported yet)
class NativeSubDecoderPCMState;  // native_sub_decoder_state.hpp

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

    static void* s_pMixingBuffer;    // _ZN3vox19VoxNativeSubDecoder15s_pMixingBufferE

protected:
    bool has_data_ = false;          // +0x16c
    std::int16_t channels_ = 0;      // +0x0a
    std::int32_t field_0c_ = 0;      // +0x0c (returned as the second track parameter)
    std::int16_t bits_ = 0;          // +0x12 (bits per sample; the byte width is bits >> 3)
};

class VoxNativeSubDecoderPCM : public VoxNativeSubDecoder {
public:
    ~VoxNativeSubDecoderPCM() override;  // D2 0x887b24 and D1 0x887b28 (both empty)

    int Seek(unsigned int sample_offset);                  // 0x887b0c: returns 0
    int GetDecodingBuffer();                               // 0x887b14: returns -1
    void ReleaseDecodingBuffer(int buffer);                // 0x887b1c (empty)
    void SetDecodingBufferToSegmentPosition(SegmentState* segment);  // 0x887b20 (empty)
    int GetBytePositionFromSampleOffset(unsigned int sample_offset);  // 0x887b2c
    // 0x887bb8, 0x887bbc, 0x887bc0: empty bodies (the ARM is a bare return).
    void Reset();
    void SetState(NativeSubDecoderPCMState* state);
    void GetState(NativeSubDecoderPCMState* state);

protected:
    std::int16_t bytes_per_sample_ = 0;  // +0x10
};

}  // namespace vox
