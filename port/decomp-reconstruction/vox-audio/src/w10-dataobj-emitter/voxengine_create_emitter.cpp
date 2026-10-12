// VoxEngineInternal::CreateEmitter, CreateEmitterAsync and UpdateSources. Each creates or
// retires emitters and data objects under the access controllers and the release queue.
// The track words for the driver come from the data object (the cursor words are not in the
// port). A null handle is the handle the original builds with id -1 and no engine.
#include <cstdint>
#include <new>
#include <vector>

#include "vox-audio/vox/data_obj.hpp"
#include "vox-audio/vox/decoder.hpp"
#include "vox-audio/vox/driver.hpp"
#include "vox-audio/vox/emitter_obj.hpp"
#include "vox-audio/vox/engine_handles.hpp"
#include "vox-audio/vox/memory.hpp"
#include "vox-audio/vox/stream.hpp"
#include "vox-audio/vox/voxengine_internal.hpp"

namespace vox {

namespace {

EmitterHandle NullEmitterHandle() { return EmitterHandle(-1, nullptr, nullptr, 0, 0); }

// Chunk bytes for a cursor-fed emitter (the value the original computes before the frame
// rounding): bits * length / 8 when the source and cursor both hand out buffer references,
// otherwise about 150 ms of audio (channels * bits / 8 * 150 * rate / 1000).
std::int64_t ChunkBytes(const DataObj& obj, bool both_allow_reference) {
    const std::int64_t bits = obj.bits_per_sample_;
    if (both_allow_reference) {
        return bits * obj.length_ / 8;
    }
    const std::int64_t frame = static_cast<std::int64_t>(obj.channels_) * bits / 8;
    return frame * 150 * obj.sample_rate_ / 1000;
}

// Rounds a chunk down to whole frames (the original subtracts chunk % frame bytes).
int RoundToFrames(const DataObj& obj, std::int64_t chunk) {
    const std::int64_t frame = static_cast<std::int64_t>(obj.channels_) * obj.bits_per_sample_ / 8;
    if (frame <= 0) {
        return static_cast<int>(chunk);
    }
    return static_cast<int>(chunk - chunk % frame);
}

}  // namespace

// 0x8690a0 _ZN3vox17VoxEngineInternal18CreateEmitterAsyncERNS_10DataHandleEiPv
// Creates the driver source and the emitter from the data object's track words, without a
// decoder cursor (the emitter loads it later through LoadAsync). The emitter is queued on the
// pending container, and the returned handle carries its stamp.
EmitterHandle VoxEngineInternal::CreateEmitterAsync(DataHandle& data, int flags, void* user) {
    data_objects_access_.GetReadAccess();
    DataObj* obj = GetDataObject(data);
    if (obj == nullptr || !obj->IsReady() || obj->stream_ == nullptr || obj->decoder_ == nullptr ||
        obj->channels_ <= 0 || driver_ == nullptr) {
        data_objects_access_.ReleaseReadAccess();
        return NullEmitterHandle();
    }
    const std::int32_t params[4] = {obj->channels_, obj->sample_rate_, obj->bits_per_sample_, obj->length_};
    DriverSourceInterface* source = driver_->CreateDriverSource(params, user, flags);
    if (source == nullptr) {
        data_objects_access_.ReleaseReadAccess();
        return NullEmitterHandle();
    }
    const std::int64_t id = GetFreeEmitterObjectId();
    EmitterObj* emitter = nullptr;
    if (void* mem = VoxAlloc(sizeof(EmitterObj))) {
        emitter = new (mem) EmitterObj(id, flags, obj->priority_bank_id_, source, obj);
    }
    if (emitter == nullptr) {
        driver_->DestroyDriverSource(source);
        data_objects_access_.ReleaseReadAccess();
        return NullEmitterHandle();
    }
    emitter->SetGroup(obj->GetGroup());
    obj->RegisterEmitter(emitter->id);
    data_objects_access_.ReleaseReadAccess();

    emitter->stamp_index_ = static_cast<int>(next_data_slot_);  // +0x14
    EmitterHandle handle(emitter->id, &VoxEngineInternal::s_voxEngineInternal, emitter,
                         data_stamps_[next_data_slot_], next_data_slot_);
    next_data_slot_ = (next_data_slot_ + 1) & 0xFu;
    emitter_pending_access_.GetWriteAccess();
    emitter_pending_.Add(emitter);
    emitter_pending_access_.ReleaseWriteAccess();
    return handle;
}

// 0x86ca08 _ZN3vox17VoxEngineInternal13CreateEmitterERNS_10DataHandleEiPv
// Synchronous create: a stream cursor and a decoder cursor on it, a driver source for the
// track words, and an emitter fed by the cursor with a buffer ring (the 7-argument constructor).
// On any failure both cursors and the source are released again.
EmitterHandle VoxEngineInternal::CreateEmitter(DataHandle& data, int flags, void* user) {
    data_objects_access_.GetReadAccess();
    DataObj* obj = GetDataObject(data);
    if (obj == nullptr || !obj->IsReady() || obj->state_ != 0 || obj->stream_ == nullptr ||
        obj->decoder_ == nullptr) {
        data_objects_access_.ReleaseReadAccess();
        return NullEmitterHandle();
    }
    StreamCursorInterface* stream_cursor = obj->stream_->CreateNewCursor();
    if (stream_cursor == nullptr) {
        data_objects_access_.ReleaseReadAccess();
        return NullEmitterHandle();
    }
    DecoderCursorInterface* cursor = obj->decoder_->CreateNewCursor(stream_cursor);
    if (cursor == nullptr) {
        obj->stream_->DestroyCursor(stream_cursor);
        data_objects_access_.ReleaseReadAccess();
        return NullEmitterHandle();
    }

    DriverSourceInterface* source = nullptr;
    if (obj->channels_ > 0 && driver_ != nullptr) {
        const std::int32_t params[4] = {obj->channels_, obj->sample_rate_, obj->bits_per_sample_, obj->length_};
        source = driver_->CreateDriverSource(params, user, flags);
    }
    if (source != nullptr) {
        const bool both_allow = source->AllowBufferReference() && cursor->AllowBufferReference();
        const std::int64_t chunk = ChunkBytes(*obj, both_allow);
        if (chunk > 0) {
            const int rounded = RoundToFrames(*obj, chunk);
            const std::int64_t id = GetFreeEmitterObjectId();
            EmitterObj* emitter = nullptr;
            if (void* mem = VoxAlloc(sizeof(EmitterObj))) {
                emitter = new (mem) EmitterObj(id, flags, obj->priority_bank_id_, rounded, source, cursor, obj);
            }
            if (emitter != nullptr) {
                emitter->SetGroup(obj->GetGroup());
                obj->RegisterEmitter(emitter->id);
                data_objects_access_.ReleaseReadAccess();

                emitter->stamp_index_ = static_cast<int>(next_data_slot_);  // +0x14
                EmitterHandle handle(emitter->id, &VoxEngineInternal::s_voxEngineInternal, emitter,
                                     data_stamps_[next_data_slot_], next_data_slot_);
                next_data_slot_ = (next_data_slot_ + 1) & 0xFu;
                emitter_pending_access_.GetWriteAccess();
                emitter_pending_.Add(emitter);
                emitter_pending_access_.ReleaseWriteAccess();
                return handle;
            }
        }
    }
    obj->stream_->DestroyCursor(stream_cursor);
    obj->decoder_->DestroyCursor(cursor);
    if (source != nullptr && driver_ != nullptr) {
        driver_->DestroyDriverSource(source);
    }
    data_objects_access_.ReleaseReadAccess();
    return NullEmitterHandle();
}

// 0x86a998 _ZN3vox17VoxEngineInternal13UpdateSourcesEv
// Does nothing while suspended. Otherwise it merges the pending data objects into the live
// table and works through the release queue once: an object that should die is detached and
// released, the others are updated. Access is taken for writing on the data table while an
// object is popped, and for reading while it is updated.
void VoxEngineInternal::UpdateSources() {
    if (suspend_count_ > 0) {
        return;
    }
    data_objects_access_.GetWriteAccess();
    data_access_.GetWriteAccess();
    if (data_pending_.begin() != data_pending_.end()) {
        data_objects_.Merge(&data_pending_);
        std::vector<std::int64_t> pending_ids;
        for (const auto& entry : data_pending_) {
            pending_ids.push_back(entry.first);
        }
        for (std::int64_t id : pending_ids) {
            data_pending_.Detach(id);  // unlinks the entry; the object now lives in data_objects_
        }
    }
    data_access_.ReleaseWriteAccess();
    data_objects_access_.ReleaseWriteAccess();

    data_queue_mutex_.Lock();
    const std::size_t queued = data_queue_.size();
    data_queue_mutex_.Unlock();
    std::size_t done = 0;
    while (done < queued) {
        DataObj* obj = nullptr;
        for (;;) {
            data_objects_access_.GetWriteAccess();
            data_queue_mutex_.Lock();
            if (!data_queue_.empty()) {
                obj = data_queue_.front();
                data_queue_.pop_front();
            }
            data_queue_mutex_.Unlock();
            if (obj == nullptr) {
                data_objects_access_.ReleaseWriteAccess();
                return;
            }
            if (!obj->ShouldDie()) {
                break;
            }
            DetachDataObject(obj->id);
            data_objects_access_.ReleaseWriteAccess();
            ++done;
            ReleaseDatasource(obj);
            if (done == queued) {
                return;
            }
            obj = nullptr;
        }
        obj->queued_ = false;  // +0x4c
        data_objects_access_.ReleaseWriteAccess();
        data_objects_access_.GetReadAccess();
        obj->Update();
        ++done;
        data_objects_access_.ReleaseReadAccess();
    }
}

}  // namespace vox
