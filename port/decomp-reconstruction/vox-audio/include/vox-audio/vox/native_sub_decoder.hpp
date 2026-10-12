// vox::VoxNativeSubDecoder and its PCM variant. Only the methods recovered so far are
// declared; the rest of the native sub-decoder (segments, state, decode) is not ported,
// so the base has no pure virtuals and the classes here are concrete.
#pragma once

#include <cstdint>

namespace vox {

struct SegmentState;  // defined with the native segment code (not ported yet)

class VoxNativeSubDecoder {
public:
    virtual ~VoxNativeSubDecoder();  // D1 0x8841f8 (empty); D0 0x8847d4 is the deleting variant
    virtual bool HasData();          // 0x884468: returns the flag at +0x16c

protected:
    bool has_data_ = false;          // +0x16c
};

class VoxNativeSubDecoderPCM : public VoxNativeSubDecoder {
public:
    ~VoxNativeSubDecoderPCM() override;  // D2 0x887b24 and D1 0x887b28 (both empty)

    int Seek(unsigned int sample_offset);                  // 0x887b0c: returns 0
    int GetDecodingBuffer();                               // 0x887b14: returns -1
    void ReleaseDecodingBuffer(int buffer);                // 0x887b1c (empty)
    void SetDecodingBufferToSegmentPosition(SegmentState* segment);  // 0x887b20 (empty)
    int GetBytePositionFromSampleOffset(unsigned int sample_offset);  // 0x887b2c

protected:
    std::int16_t bytes_per_sample_ = 0;  // +0x10
};

}  // namespace vox
