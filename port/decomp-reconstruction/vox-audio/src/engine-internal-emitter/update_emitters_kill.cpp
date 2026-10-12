// VoxEngineInternal frame update of the emitters and the emitter kill. UpdateEmitters
// advances the master and group fades, pushes each group's gain to its emitters, updates
// every emitter, collects the dead ones, and kills them after the read pass.
#include "vox-audio/vox/data_obj.hpp"
#include "vox-audio/vox/emitter_obj.hpp"
#include "vox-audio/vox/gain_fade.hpp"
#include "vox-audio/vox/memory.hpp"
#include "vox-audio/vox/priority_bank.hpp"
#include "vox-audio/vox/voxengine_internal.hpp"

namespace vox {

// 0x86db40 _ZN3vox17VoxEngineInternal13UpdateEmittersEf
// The step is clamped to [0, 0.1] (a NaN step becomes 0). While the engine is suspended
// (suspend_count_ > 0) nothing runs. The dead-emitter list is filled under the live read
// access and drained under the write access, popping from the back as the original does.
void VoxEngineInternal::UpdateEmitters(float dt) {
    mutex_.Lock();
    if (suspend_count_ > 0) {
        mutex_.Unlock();
        return;
    }
    if (dt >= 0.0f) {
        if (dt > 0.1f) {
            dt = 0.1f;
        }
    } else {
        dt = 0.0f;
    }

    Update3D();
    UpdateDSP(dt);

    // Master fade: advance while it has time left, then take its current value.
    if (master_fade_.elapsed < master_fade_.duration) {
        master_fade_.elapsed += dt;
    } else {
        master_fade_.flag = true;
    }
    const float master = CurrentOfFade(master_fade_);
    for (int group = 0; group < 32; ++group) {
        VoxGainFade& fade = group_fades_[group];
        if (fade.elapsed < fade.duration) {
            fade.elapsed += dt;
        } else {
            fade.flag = true;
        }
        group_gains_[group] = master * CurrentOfFade(fade);
    }
    mutex_.Unlock();

    emitter_access_.GetWriteAccess();
    emitter_pending_access_.GetWriteAccess();
    if (emitter_pending_.begin() != emitter_pending_.end()) {
        emitters_.Merge(&emitter_pending_);
        // Merge copies the pending entries into the live table. Detach empties the pending
        // table without deleting the objects, which now belong to emitters_.
        while (emitter_pending_.begin() != emitter_pending_.end()) {
            emitter_pending_.Detach(emitter_pending_.begin()->first);
        }
    }
    emitter_pending_access_.ReleaseWriteAccess();
    emitter_access_.ReleaseWriteAccess();

    emitter_access_.GetReadAccess();
    for (auto& entry : emitters_) {
        auto* emitter = static_cast<EmitterObj*>(entry.second);
        emitter->SetGainModifier(group_gains_[emitter->GetGroup()]);
        emitter->Update(dt);
    }
    for (auto& entry : emitters_) {
        auto* emitter = static_cast<EmitterObj*>(entry.second);
        if (emitter->ShouldDie()) {
            dead_emitters_.push_back(emitter);
        }
    }
    if (priority_bank_ != nullptr) {
        priority_bank_->Update();
    }
    emitter_access_.ReleaseReadAccess();

    if (!dead_emitters_.empty()) {
        emitter_access_.GetWriteAccess();
        while (!dead_emitters_.empty()) {
            EmitterObj* dead = dead_emitters_.back();
            KillEmitter(static_cast<EmitterObj*>(emitters_.Detach(dead->id)));
            dead_emitters_.pop_back();
        }
        emitter_access_.ReleaseWriteAccess();
    }
}

// 0x86e160 _ZN3vox17VoxEngineInternal11KillEmitterEPNS_10EmitterObjE
// Partial. Done: the slot stamp is bumped (the handles to this emitter go stale), the
// priority bank entry is removed, the emitter is unregistered from its data object, and the
// object is destroyed. Not done, because the pieces are not in the port:
//   - the driver call at slot +0x48 that the original makes when the emitter has a DSP
//     source (+0x110), with no argument;
//   - the release of the source object the emitter owns (+0x114, a DataSource with vtable
//     slots +0x14 and +0x20, plus the source's own release at +0x30);
//   - the push of the data object onto the pending-release list (sentinel at +0x6c, its
//     mutex at +0x74) that UpdateSources drains.
// Until those are in, the data objects of killed emitters are not released.
void VoxEngineInternal::KillEmitter(EmitterObj* emitter) {
    if (emitter == nullptr) {
        return;
    }
    ++emitter_stamps_[emitter->stamp_index_];
    if (emitter->in_bank_ && priority_bank_ != nullptr) {
        priority_bank_->RemoveEmitter(emitter->bank_, emitter);
    }
    emitter->CleanUp();

    data_objects_access_.GetReadAccess();
    if (DataObj* data = emitter->data_) {
        data->UnregisterEmitter(emitter->id);
    }
    data_objects_access_.ReleaseReadAccess();

    emitter->~EmitterObj();
    VoxFree(emitter);
}

}  // namespace vox
