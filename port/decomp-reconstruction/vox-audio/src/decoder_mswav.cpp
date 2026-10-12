// vox::DecoderMSWav: accessors, the cursor factory-free constructor and DestroyCursor.
#include "vox-audio/vox/decoder_mswav.hpp"

#include "vox-audio/vox/memory.hpp"

namespace vox {

// 0x870548 _ZN3vox12DecoderMSWavC1Ev (C2 0x870514 is the same body)
DecoderMSWav::DecoderMSWav() = default;

// 0x870ffc _ZN3vox12DecoderMSWavD1Ev (D2 0x870f7c has the same body; the deleting variant
// 0x87107c is emitted by the compiler from this destructor).
// Partial: the DataNode tree at +0x3c is not ported, so the original's DataNode::DropNodes
// call (0x870e38) on the child at +0x08 is skipped. Only the block at +0x3c is released.
DecoderMSWav::~DecoderMSWav() {
    flag_40_ = true;
    if (data_nodes_ != nullptr) {
        VoxFree(data_nodes_);
    }
    data_nodes_ = nullptr;
}

// 0x870504 _ZN3vox12DecoderMSWav7GetTypeEv
int DecoderMSWav::GetType() const {
    return 1;
}

// 0x87050c _ZN3vox12DecoderMSWav8GetParamEv
std::int32_t* DecoderMSWav::GetParam() {
    return nullptr;
}

// 0x870a9c _ZN3vox12DecoderMSWav13DestroyCursorEPNS_22DecoderCursorInterfaceE
void DecoderMSWav::DestroyCursor(DecoderCursorInterface* cursor) {
    if (cursor == nullptr) {
        return;
    }
    cursor->~DecoderCursorInterface();
    VoxFree(cursor);
}

}  // namespace vox
