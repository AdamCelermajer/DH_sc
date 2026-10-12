// vox::VoxMSWavSubDecoderPCM: the PCM variant of the WAV sub-decoder interface. Only its
// destructor is reconstructed; Decode, Seek and HasData are not ported, so the class
// stays abstract.
#pragma once

#include "vox-audio/vox/decoder_mswav_cursor.hpp"

namespace vox {

class VoxMSWavSubDecoderPCM : public VoxMSWavSubDecoder {
public:
    ~VoxMSWavSubDecoderPCM() override;  // D1 0x875e24 (empty); D0 0x875ec8 is the deleting variant
};

}  // namespace vox
