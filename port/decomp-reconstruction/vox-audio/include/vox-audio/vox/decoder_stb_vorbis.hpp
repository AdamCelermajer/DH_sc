// vox::DecoderStbVorbis: the Ogg Vorbis decoder wrapper. Only the accessors are
// reconstructed here. The decoding is done by stb_vorbis (upstream, public domain),
// which is not retyped, and the cursor is not ported, so the class stays abstract.
#pragma once

#include <cstdint>

#include "vox-audio/vox/decoder.hpp"

namespace vox {

class DecoderStbVorbis : public DecoderInterface {
public:
    int GetType() const override;       // 0x874e54: returns 2
    std::int32_t* GetParam() override;  // 0x874e5c: returns null
};

}  // namespace vox
