// VoxNativeSubDecoderIMAADPCM::DecodeBlock (partial). Its vtable slot is reached through the
// IMA segment decoders, so the symbol must exist for the host link.
#include "vox-audio/vox/native_sub_decoder_imaadpcm.hpp"

namespace vox {

// 0x887068 _ZN3vox27VoxNativeSubDecoderIMAADPCM11DecodeBlockEPvPNS_12SegmentStateE (partial)
// Not reproduced: the stream position (Tell, Seek and Read through the stream vtable at
// +0x18, +0x10 and +0x1c), the per-channel decoding buffers at +0x19c and the nibble loop
// with the step and index tables. The nibble algorithm is the one in ImaAdpcmDecodeBlock
// (ima_adpcm.cpp). Until the stream part is ported this returns 0 frames decoded.
int VoxNativeSubDecoderIMAADPCM::DecodeBlock(void* buffer, SegmentState* segment) {
    (void)buffer;
    (void)segment;
    return 0;
}

}  // namespace vox
