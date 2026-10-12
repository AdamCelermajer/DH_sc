// The body of vox::DecoderMPC8::CreateNewCursor (0x8704ac), as a free function.
//
// DecoderMPC8 does not override CreateNewCursor, so it stays abstract in the port. An override
// would make the decoder's vtable reference the Musepack-linked cursor, and decoder_mpc8_test
// (which links only decoder_mpc8.cpp) would then fail to link. The factory (0x8704dc) therefore
// cannot construct a DecoderMPC8 yet.
#include <new>

#include "vox-audio/vox/decoder_mpc8.hpp"
#include "vox-audio/vox/memory.hpp"

namespace vox {

// 0x8704ac _ZN3vox11DecoderMPC815CreateNewCursorEPNS_21StreamCursorInterfaceE
DecoderMPC8Cursor* CreateDecoderMPC8Cursor(DecoderMPC8* decoder, StreamCursorInterface* stream) {
    void* storage = VoxAlloc(sizeof(DecoderMPC8Cursor));
    if (storage == nullptr) {
        return nullptr;
    }
    return new (storage) DecoderMPC8Cursor(decoder, stream);
}

}  // namespace vox
