// VoxNativeSubDecoder base and the PCM variant. Most PCM methods are stubs in the
// original: they return a fixed value or do nothing, and are reproduced as such.
#include "vox-audio/vox/native_sub_decoder.hpp"

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

}  // namespace vox
