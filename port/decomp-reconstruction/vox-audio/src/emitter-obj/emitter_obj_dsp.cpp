// vox::EmitterObj DSP name hand-off and cleanup. SetDSPParameter copies the name into the
// pending slot (+0xf0); UpdateDSP (emitter_obj_state.cpp) moves it into the slot in use (+0xec).
#include <cstring>

#include "vox-audio/vox/emitter_obj.hpp"
#include "vox-audio/vox/memory.hpp"

namespace vox {

// 0x866acc _ZN3vox10EmitterObj15SetDSPParameterEiPv
// partial: the original returns the unlock result (see SetGain). Only parameter 0 is handled and
// its value is a string. A non-empty string replaces the pending copy; the previous pending copy
// is freed first. The original calls strlen on any value, so a null value is not guarded there;
// the port treats null as an empty string.
void EmitterObj::SetDSPParameter(int param, void* value) {
    mutex.Lock();
    if (param == 0) {
        const char* name = static_cast<const char*>(value);
        const std::size_t length = name != nullptr ? std::strlen(name) : 0;
        if (length > 0) {
            VoxFree(dsp_pending_);
            dsp_pending_ = VoxAlloc(length + 1);
            if (dsp_pending_ != nullptr) {
                std::strcpy(static_cast<char*>(dsp_pending_), name);
            }
        }
    }
    mutex.Unlock();
}

// 0x863954 _ZN3vox10EmitterObj7CleanUpEv
// partial: the original also walks the buffer ring at +0xf4 (a vector of buffers, read index
// +0x100, count +0x104) and frees the buffers the decoder cursor does not reuse. The port's
// EmitterObj has no ring, so only the two DSP name buffers (+0xec in use, +0xf0 pending) are freed.
// The port clears both pointers after the free; the original leaves them set.
void EmitterObj::CleanUp() {
    VoxFree(dsp_buffer_);
    dsp_buffer_ = nullptr;
    VoxFree(dsp_pending_);
    dsp_pending_ = nullptr;
}

}  // namespace vox
