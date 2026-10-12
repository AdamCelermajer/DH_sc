// vox::DecoderMSWavCursor: the Microsoft WAV cursor. It holds a WAV sub-decoder
// (+0x24) and forwards Decode, Seek and HasData to it through the sub-decoder's
// virtual slots (+0x08, +0x0c, +0x10). The sub-decoder's own class is not ported; only
// the slots the cursor calls are declared here.
#pragma once

#include <cstdint>

#include "vox-audio/vox/decoder.hpp"

namespace vox {

// Interface of the WAV sub-decoder (Decode +0x08, Seek +0x0c, HasData +0x10). The loop
// flag the cursor sets lives at +0x28 in the original.
class VoxMSWavSubDecoder {
public:
    virtual ~VoxMSWavSubDecoder() = default;  // 0x875414: empty body in the original
    virtual int Decode(void* dst, int count) = 0;
    virtual int Seek(unsigned int frame) = 0;
    virtual bool HasData() = 0;

    bool loop_ = false;  // +0x28
};

class DecoderMSWavCursor : public DecoderCursorInterface {
public:
    // Port-only constructor: the original constructor is not reconstructed yet.
    explicit DecoderMSWavCursor(VoxMSWavSubDecoder* sub_decoder) : sub_decoder_(sub_decoder) {}
    // D1 0x8709d8 and D2 0x870a48 share this body; D0 0x870a2c is the deleting variant.
    ~DecoderMSWavCursor() override;

    int Decode(void* dst, int count) override;       // 0x87057c: sub-decoder, else 0
    int Seek(unsigned int frame) override;           // 0x8705a8: sub-decoder, else -1
    void SetLoop(bool on) override;                  // 0x8705d4: writes the sub-decoder loop flag
    bool HasData() override;                         // 0x8705e4: sub-decoder, else false

private:
    VoxMSWavSubDecoder* sub_decoder_ = nullptr;      // +0x24
};

}  // namespace vox
