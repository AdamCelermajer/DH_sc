#include "vox-audio/vox/decoder.hpp"

namespace vox {

// 0x86fb5c _ZN3vox22DecoderCursorInterface5ResetEv
// Reset calls the virtual Seek with frame 0 (vtable +0x28); the result is dropped.
void DecoderCursorInterface::Reset() {
    (void)Seek(0);
}

// 0x86fb44 _ZN3vox22DecoderCursorInterface9DecodeRefERPvi (partial)
// The original calls virtual slot +0x10 with the count only. The forwarding
// to Decode(dst, count) here is an inferred equivalent; see NOTES.md.
int DecoderCursorInterface::DecodeRef(void*& dst, int count) {
    return Decode(dst, count);
}

}  // namespace vox
