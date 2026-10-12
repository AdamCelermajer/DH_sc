// vox::EmitterObj playback path (wave 9): the cursor-to-source pull (ProcessNonNativeData), the
// play cursor read (GetPlayCursor), the 3D parameter push (Update3D) and the debug dump
// (PrintDebug). Source slots are named from the DriverCallbackSourceInterface vtable
// (+0x24 NeedData, +0x28 UploadData, +0x1c GetByteOffset, +0x3c Set3DParameter, +0x50
// PrintDebug). Cursor slots come from the DecoderCursorInterface vtable (+0x14 DecodeRef,
// +0x18 HasData, +0x34 AllowBufferReference).
#include "vox-audio/vox/decoder.hpp"
#include "vox-audio/vox/driver.hpp"
#include "vox-audio/vox/emitter_obj.hpp"

namespace vox {

// 0x864e28 _ZN3vox10EmitterObj10PrintDebugEv
// The original returns the Mutex::Unlock result; the port returns void like the other accessors.
void EmitterObj::PrintDebug() {
    mutex.Lock();
    if (dsp_source_ != nullptr) {
        dsp_source_->PrintDebug();  // source +0x50
    }
    mutex.Unlock();
}

// 0x863318 _ZN3vox10EmitterObj8Update3DEv
// Parameter ids in the order the original pushes them: 0 (int at +0xc0), 1..6 (floats at
// +0xc4..+0xd8), 8 position (+0x9c), 9 velocity (+0xb4), 10 direction (+0xa8). Id 7 is never pushed.
// Each id's dirty byte (+0xe0 + id) is cleared after its push. The port returns early without a
// source, so the dirty bytes stay set until a source exists (the original would call through null).
void EmitterObj::Update3D() {
    if (dsp_source_ == nullptr) {
        return;
    }
    struct Entry {
        int id;
        const void* value;
    };
    const Entry entries[] = {
        {0, &param0_},
        {1, &float_params_[0]},
        {2, &float_params_[1]},
        {3, &float_params_[2]},
        {4, &float_params_[3]},
        {5, &float_params_[4]},
        {6, &float_params_[5]},
        {8, &position_},
        {9, &velocity_},
        {10, &direction_},
    };
    for (const Entry& entry : entries) {
        if (dirty_[entry.id] != 0) {
            dsp_source_->Set3DParameter(entry.id, entry.value);  // source +0x3c
            dirty_[entry.id] = 0;
        }
    }
}

// 0x8654f8 _ZN3vox10EmitterObj13GetPlayCursorEv
// partial: the original returns with the mutex still locked when play_length_ is 0. The port
// unlocks on that path as well, because a second call would otherwise block forever. Like the
// original, it divides the source byte offset (source +0x1c) by play_rate_ after the modulo.
float EmitterObj::GetPlayCursor() {
    mutex.Lock();
    float position = 0.0f;
    if (play_length_ > 0 && dsp_source_ != nullptr) {
        const int offset = dsp_source_->GetByteOffset();
        position = static_cast<float>(offset % play_length_) / static_cast<float>(play_rate_);
    }
    mutex.Unlock();
    return position;
}

// 0x863228 _ZN3vox10EmitterObj20ProcessNonNativeDataEf
// The float argument in the pseudocode is a leftover register: NeedData takes no argument, so dt
// is unused. The port returns 0 when the source, the cursor or the ring is missing, where the
// original would call through null. A zero buffer_count_ also returns 0 instead of dividing by it.
// Flow: the source asks for data (NeedData); the cursor has data (HasData); the ring slot at the
// read index is refilled through AllowBufferReference when empty (a refusal sets state_set_ to
// -1); DecodeRef fills the slot with buffer_chunk_bytes_; a positive count is uploaded to the
// source (UploadData) and the read index advances modulo buffer_count_. The return value is the
// number of wraps (next / count).
int EmitterObj::ProcessNonNativeData(float dt) {
    (void)dt;
    if (dsp_source_ == nullptr || decoder_cursor_ == nullptr || buffer_ring_.empty() ||
        buffer_count_ <= 0) {
        return 0;
    }
    int result = dsp_source_->NeedData();  // source +0x24
    if (result == 0) {
        return result;
    }
    if (!decoder_cursor_->HasData()) {  // cursor +0x18
        return 0;
    }
    if (buffer_ring_[buffer_read_index_] == nullptr) {
        if (!decoder_cursor_->AllowBufferReference()) {  // cursor +0x34
            state_set_ = -1;
            return 0;
        }
    }
    void*& slot = buffer_ring_[buffer_read_index_];
    result = decoder_cursor_->DecodeRef(slot, buffer_chunk_bytes_);  // cursor +0x14
    if (result > 0) {
        dsp_source_->UploadData(buffer_ring_[buffer_read_index_], result);  // source +0x28
        const int next = buffer_read_index_ + 1;
        buffer_read_index_ = next % buffer_count_;
        return next / buffer_count_;
    }
    return result;
}

}  // namespace vox
