// vox::EmitterObj state-change callback slot, the reported status, and the DSP
// source hand-off. Each locked accessor takes the Handlable mutex (+0x18).
#include "vox-audio/vox/emitter_obj.hpp"

#include "vox-audio/vox/driver.hpp"
#include "vox-audio/vox/memory.hpp"

namespace vox {

// 0x869a38 _ZN3vox10EmitterObjD2Ev (same body as D1 0x8699c0)
// 0x8699c0 _ZN3vox10EmitterObjD1Ev
// Releases the buffer at +0xf4 when one is set; the Handlable base and its mutex follow.
EmitterObj::~EmitterObj() {
    if (owned_buffer_ != nullptr) {
        VoxFree(owned_buffer_);
    }
}

// 0x869a1c _ZN3vox10EmitterObjD0Ev
// Deleting destructor variant; the compiler emits it for the class above.

// 0x864ce0 _ZN3vox10EmitterObj28RegisterStateChangedCallbackEPFvRNS_13EmitterHandleEPvNS_18EmitterExternStateEES3_
void EmitterObj::RegisterStateChangedCallback(EmitterStateCallback callback, void* user_data) {
    mutex.Lock();
    state_callback_ = callback;
    state_callback_user_ = user_data;
    mutex.Unlock();
}

// 0x864cb4 _ZN3vox10EmitterObj30UnregisterStateChangedCallbackEv
void EmitterObj::UnregisterStateChangedCallback() {
    mutex.Lock();
    state_callback_user_ = nullptr;
    state_callback_ = nullptr;
    mutex.Unlock();
}

// 0x864bec _ZN3vox10EmitterObj30NeedToSendStateChangedCallbackERPFvRNS_13EmitterHandleEPvNS_18EmitterExternStateEERS3_RS4_
// When a callback is pending (+0x98) and one is registered, it is handed out once,
// the pending byte is cleared, and the status comes from the state word (+0x90):
// 1 -> 1, 2 -> 2, 0 or 3 -> 4, anything else -> 0.
bool EmitterObj::NeedToSendStateChangedCallback(EmitterStateCallback& callback, void*& user_data,
                                                EmitterExternState& status) {
    mutex.Lock();
    bool pending = false;
    if (callback_pending_ && state_callback_ != nullptr) {
        callback = state_callback_;
        user_data = state_callback_user_;
        callback_pending_ = false;
        int code = 0;
        switch (state_) {
            case 0:
            case 3:
                code = 4;
                break;
            case 1:
                code = 1;
                break;
            case 2:
                code = 2;
                break;
            default:
                code = 0;
                break;
        }
        status = static_cast<EmitterExternState>(code);
        pending = true;
    }
    mutex.Unlock();
    return pending;
}

// 0x864b5c _ZN3vox10EmitterObj9GetStatusEv
// State 1 reports 2 when the flag at +0x6c is set, otherwise 0x11 or 0x21 by the
// sub-state word (+0x94). State 2 reports 2, states 0 and 3 report 4, others 0.
int EmitterObj::GetStatus() {
    mutex.Lock();
    int status = 0;
    switch (state_) {
        case 1:
            if (resume_fade_.flag) {
                status = 2;
            } else {
                status = (state_set_ == 1) ? 0x11 : 0x21;
            }
            break;
        case 2:
            status = 2;
            break;
        case 0:
        case 3:
            status = 4;
            break;
        default:
            status = 0;
            break;
    }
    mutex.Unlock();
    return status;
}

// 0x8638e4 _ZN3vox10EmitterObj9UpdateDSPEf
// partial: when a pending DSP name is set (+0xf0), the name in use (+0xec) is freed and replaced
// by it. The original then calls a one-argument source slot (+0x40) that the port's
// DriverSourceInterface does not model, so that call is skipped. The source Update(dt) (+0x44)
// runs in both cases, and not at all when there is no source.
void EmitterObj::UpdateDSP(float dt) {
    if (dsp_pending_ != nullptr) {
        VoxFree(dsp_buffer_);
        dsp_buffer_ = dsp_pending_;
        dsp_pending_ = nullptr;
        if (dsp_source_ == nullptr) {
            return;
        }
    }
    if (dsp_source_ != nullptr) {
        dsp_source_->Update(dt);
    }
}

}  // namespace vox
