// VoxEngineInternal::SetInteractiveMusicState: passes a named music state to the native decoder
// cursor of an emitter whose data object is decoded by the native decoder (type 4).
#include "vox-audio/vox/data_obj.hpp"
#include "vox-audio/vox/decoder.hpp"
#include "vox-audio/vox/decoder_native.hpp"
#include "vox-audio/vox/emitter_obj.hpp"
#include "vox-audio/vox/voxengine_internal.hpp"

namespace vox {

// 0x86764c _ZN3vox17VoxEngineInternal24SetInteractiveMusicStateERNS_13EmitterHandleEPKc
// The decoder is the data object's decoder (+0x3c), or none when the data object's state word
// (+0x50) is not 0. Only a native decoder (GetType 4) receives the state: the emitter's cursor
// (+0x114) gets the name, and the emitter records it for later (SetInteractiveMusicStateChange).
// The original returns the ReleaseReadAccess result.
// Partial: when the data object's state word (+0x50) is not 0, the binary passes a null decoder to its
// GetType slot (a null call). The port skips the call on that path, so no state is passed.
int VoxEngineInternal::SetInteractiveMusicState(EmitterHandle& emitter, const char* state) {
    emitter_access_.GetReadAccess();
    if (EmitterObj* obj = GetEmitterObject(emitter)) {
        DataObj* data = obj->data_;
        DecoderInterface* decoder = nullptr;
        if (data != nullptr && data->state_ == 0) {
            decoder = data->decoder_;
        }
        if (decoder != nullptr && decoder->GetType() == 4) {
            auto* cursor = static_cast<DecoderNativeCursor*>(obj->decoder_cursor_);
            if (cursor != nullptr) {
                cursor->SetInteractiveMusicState(state);
                obj->SetInteractiveMusicStateChange(state);
            }
        }
    }
    return emitter_access_.ReleaseReadAccess();
}

}  // namespace vox
