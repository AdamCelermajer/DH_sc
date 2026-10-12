// vox::DecoderStbVorbis accessors. The stb_vorbis decoder itself is upstream code and is
// not retyped here.
#include "vox-audio/vox/decoder_stb_vorbis.hpp"

namespace vox {

// 0x874e54 _ZN3vox16DecoderStbVorbis7GetTypeEv
int DecoderStbVorbis::GetType() const {
    return 2;
}

// 0x874e5c _ZN3vox16DecoderStbVorbis8GetParamEv
std::int32_t* DecoderStbVorbis::GetParam() {
    return nullptr;
}

}  // namespace vox
