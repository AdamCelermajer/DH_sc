// VoxEngineInternal emitter lookups and user data: the data object an emitter plays, the
// user-data word of an emitter, and the detach of an emitter from the live or pending table.
#include "vox-audio/vox/data_obj.hpp"
#include "vox-audio/vox/emitter_obj.hpp"
#include "vox-audio/vox/engine_handles.hpp"
#include "vox-audio/vox/voxengine_internal.hpp"

namespace vox {

// 0x868f9c _ZN3vox17VoxEngineInternal7GetDataERNS_13EmitterHandleE
// A missing emitter, or one with no data object, gives the empty data handle.
// Partial: the emitter with a null data pointer (+0x118) releases the emitter read access twice
// in the binary (asm 0x869028 and 0x869030, checked). A missing emitter releases it once. The
// port releases once on both paths, because a second release of an unlocked access is not
// reproduced on purpose.
DataHandle VoxEngineInternal::GetData(EmitterHandle& emitter) {
    emitter_access_.GetReadAccess();
    EmitterObj* obj = GetEmitterObject(emitter);
    DataObj* data = obj != nullptr ? obj->data_ : nullptr;
    DataHandle result = data != nullptr
                            ? DataHandle(data->id, &s_voxEngineInternal, nullptr, 0, 0)
                            : DataHandle(-1, nullptr, nullptr, 0, 0);
    emitter_access_.ReleaseReadAccess();
    return result;
}

// 0x86a5d0 _ZN3vox17VoxEngineInternal19DetachEmitterObjectEx
// Takes the emitter out of the live table without destroying it. Only when the id is not
// there does it look in the pending table, under the pending write access.
EmitterObj* VoxEngineInternal::DetachEmitterObject(std::int64_t id) {
    Handlable* found = emitters_.Detach(id);
    if (found == nullptr) {
        emitter_pending_access_.GetWriteAccess();
        found = emitter_pending_.Detach(id);
        emitter_pending_access_.ReleaseWriteAccess();
    }
    return static_cast<EmitterObj*>(found);
}

// 0x867608 _ZN3vox17VoxEngineInternal11SetUserDataERNS_13EmitterHandleERNS_21EmitterHandleUserDataE
// Partial: the listing returns the int from AccessController::ReleaseReadAccess; the port's void
// signature drops it. The record is read as one word: the listing passes the record to
// EmitterObj::SetUserData (0x8653c0), which loads only its first word (asm checked).
void VoxEngineInternal::SetUserData(EmitterHandle& emitter, EmitterHandleUserData& user_data) {
    emitter_access_.GetReadAccess();
    if (EmitterObj* obj = GetEmitterObject(emitter)) {
        obj->SetUserData(user_data.word);
    }
    emitter_access_.ReleaseReadAccess();
}

}  // namespace vox
