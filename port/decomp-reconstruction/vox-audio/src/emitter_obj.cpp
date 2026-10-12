#include "vox-audio/vox/emitter_obj.hpp"

namespace vox {

// 0x863528 _ZN3vox10EmitterObj15SetGainModifierEf
void EmitterObj::SetGainModifier(float gain) {
    gain_modifier_ = gain;
}

// 0x863530 _ZN3vox10EmitterObj7IsReadyEv
bool EmitterObj::IsReady() {
    return (ready_flag_ ^ 1) != 0;
}

// 0x8653c0 _ZN3vox10EmitterObj11SetUserDataERNS_21EmitterHandleUserDataE
void EmitterObj::SetUserData(const std::uintptr_t& first_word) {
    mutex_.Lock();
    user_data_ = first_word;
    mutex_.Unlock();
}

// 0x8655e0 _ZN3vox10EmitterObj5ResetEv
void EmitterObj::Reset() {
    mutex_.Lock();
    reset_flag_ = 1;
    mutex_.Unlock();
}

// 0x865608 _ZN3vox10EmitterObj7SetLoopEb
// The original stores the flag at +0x8d; GetLoop reads +0x8c. Both are kept.
void EmitterObj::SetLoop(bool loop) {
    mutex_.Lock();
    loop_ = loop ? 1 : 0;
    mutex_.Unlock();
}

// 0x865630 _ZN3vox10EmitterObj7GetLoopEv
bool EmitterObj::GetLoop() {
    mutex_.Lock();
    const bool loop = loop_read_ != 0;
    mutex_.Unlock();
    return loop;
}

// 0x865658 _ZN3vox10EmitterObj8SetStateEi
void EmitterObj::SetState(int state) {
    mutex_.Lock();
    state_set_ = state;
    mutex_.Unlock();
}

// 0x865680 _ZN3vox10EmitterObj8GetStateEv
int EmitterObj::GetState() {
    mutex_.Lock();
    const int state = state_;
    mutex_.Unlock();
    return state;
}

// 0x8656f8 _ZN3vox10EmitterObj7IsGroupEj
bool EmitterObj::IsGroup(std::uint32_t groups) {
    mutex_.Lock();
    const bool in_group = (groups & (std::uint32_t{1} << group_)) != 0;
    mutex_.Unlock();
    return in_group;
}

// 0x865734 _ZN3vox10EmitterObj8SetGroupEi
void EmitterObj::SetGroup(int group) {
    mutex_.Lock();
    group_ = group;
    mutex_.Unlock();
}

// 0x86575c _ZN3vox10EmitterObj8GetGroupEv
int EmitterObj::GetGroup() {
    mutex_.Lock();
    const int group = group_;
    mutex_.Unlock();
    return group;
}

}  // namespace vox
