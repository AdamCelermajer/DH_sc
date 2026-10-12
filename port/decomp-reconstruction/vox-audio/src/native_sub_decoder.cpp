// VoxNativeSubDecoder base and the PCM variant. Most PCM methods are stubs in the
// original: they return a fixed value or do nothing, and are reproduced as such.
#include "vox-audio/vox/native_sub_decoder.hpp"

#include "vox-audio/vox/memory.hpp"

namespace vox {

// 0x8841f8 _ZN3vox19VoxNativeSubDecoderD1Ev
VoxNativeSubDecoder::~VoxNativeSubDecoder() {}

// 0x8847d4 _ZN3vox19VoxNativeSubDecoderD0Ev
// Deleting destructor variant; the compiler emits it for the class above.

// 0x884468 _ZN3vox19VoxNativeSubDecoder7HasDataEv
bool VoxNativeSubDecoder::HasData() {
    return has_data_;
}

// 0x887b24 _ZN3vox22VoxNativeSubDecoderPCMD2Ev and 0x887b28 _ZN3vox22VoxNativeSubDecoderPCMD1Ev
// Both bodies are empty.
VoxNativeSubDecoderPCM::~VoxNativeSubDecoderPCM() {}

// 0x887b0c _ZN3vox22VoxNativeSubDecoderPCM4SeekEj
int VoxNativeSubDecoderPCM::Seek(unsigned int sample_offset) {
    (void)sample_offset;
    return 0;
}

// 0x887b14 _ZN3vox22VoxNativeSubDecoderPCM17GetDecodingBufferEv
int VoxNativeSubDecoderPCM::GetDecodingBuffer() {
    return -1;
}

// 0x887b1c _ZN3vox22VoxNativeSubDecoderPCM21ReleaseDecodingBufferEi
void VoxNativeSubDecoderPCM::ReleaseDecodingBuffer(int buffer) {
    (void)buffer;
}

// 0x887b20 _ZN3vox22VoxNativeSubDecoderPCM34SetDecodingBufferToSegmentPositionEPNS_12SegmentStateE
void VoxNativeSubDecoderPCM::SetDecodingBufferToSegmentPosition(SegmentState* segment) {
    (void)segment;
}

// 0x887b2c _ZN3vox22VoxNativeSubDecoderPCM31GetBytePositionFromSampleOffsetEj
int VoxNativeSubDecoderPCM::GetBytePositionFromSampleOffset(unsigned int sample_offset) {
    return static_cast<int>(bytes_per_sample_) * static_cast<int>(sample_offset);
}

// 0x887bb8 _ZN3vox22VoxNativeSubDecoderPCM5ResetEv (empty)
void VoxNativeSubDecoderPCM::Reset() {}

// 0x887bbc _ZN3vox22VoxNativeSubDecoderPCM8SetStateEPNS_24NativeSubDecoderPCMStateE (empty)
void VoxNativeSubDecoderPCM::SetState(NativeSubDecoderPCMState*) {}

// 0x887bc0 _ZN3vox22VoxNativeSubDecoderPCM8GetStateEPNS_24NativeSubDecoderPCMStateE (empty)
void VoxNativeSubDecoderPCM::GetState(NativeSubDecoderPCMState*) {}

}  // namespace vox

namespace vox {

void* VoxNativeSubDecoder::s_pMixingBuffer = nullptr;

// 0x884438 _ZN3vox19VoxNativeSubDecoder14GetTrackParamsEv
TrackParams VoxNativeSubDecoder::GetTrackParams() {
    TrackParams params;
    params.channels = channels_;
    params.unknown_08 = field_0c_;
    params.bits_per_sample = bits_;
    params.length_frames = 0;
    return params;
}

// 0x884f04 _ZN3vox19VoxNativeSubDecoder5CleanEv
void VoxNativeSubDecoder::Clean() {
    if (s_pMixingBuffer != nullptr) {
        VoxFree(s_pMixingBuffer);
    }
}

// 0x885258 _ZN3vox19VoxNativeSubDecoder19MixMultipleSegmentsEPsi (partial)
int VoxNativeSubDecoder::MixMultipleSegments(std::int16_t* dst, int bytes) {
    (void)dst;
    return bytes / (static_cast<int>(channels_) * (static_cast<int>(bits_) >> 3));
}

// 0x8841fc _ZN3vox19VoxNativeSubDecoder25EmulateMixSegmentInBufferEiPNS_12SegmentStateE (partial)
int VoxNativeSubDecoder::EmulateMixSegmentInBuffer(int bytes, SegmentState* segment) {
    (void)segment;
    return bytes / (static_cast<int>(channels_) * (static_cast<int>(bits_) >> 3));
}

// 0x8847e8 _ZN3vox19VoxNativeSubDecoder21UpdateOldSegmentStateEPNS_14TransitionRuleE (partial)
void VoxNativeSubDecoder::UpdateOldSegmentState(TransitionRule* rule) {
    (void)rule;
}

// 0x884ab4 _ZN3vox19VoxNativeSubDecoder23UpdateDyingSegmentStateEPNS_14TransitionRuleE (partial)
void VoxNativeSubDecoder::UpdateDyingSegmentState(TransitionRule* rule) {
    (void)rule;
}

}  // namespace vox
