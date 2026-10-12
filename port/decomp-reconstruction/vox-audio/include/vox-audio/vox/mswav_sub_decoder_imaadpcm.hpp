// vox::VoxMSWavSubDecoderIMAADPCM: the IMA ADPCM variant of the WAV sub-decoder interface.
// Only the destructor is reconstructed; Decode, Seek, HasData and the constructors are
// not ported, so the class stays abstract.
#pragma once

#include "vox-audio/vox/decoder_mswav_cursor.hpp"

namespace vox {

class VoxMSWavSubDecoderIMAADPCM : public VoxMSWavSubDecoder {
public:
    // D1 0x8754dc and D2 0x875554: the vtable is reset and the block at +0x4c is freed.
    ~VoxMSWavSubDecoderIMAADPCM() override;

private:
    void* decode_block_ = nullptr;  // +0x4c, freed by the destructor (owner not identified)
};

}  // namespace vox
