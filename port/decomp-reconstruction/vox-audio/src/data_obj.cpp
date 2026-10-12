#include "vox-audio/vox/data_obj.hpp"

namespace vox {

// 0x863c24 _ZN3vox7DataObjD1Ev and 0x863cac _ZN3vox7DataObjD0Ev.
// Partial: the original frees the emitter list nodes, then resets the list
// sentinel and destroys the Handlable mutex. The std::list and the member
// mutexes do the same here.
DataObj::~DataObj() = default;

// 0x865878 _ZN3vox7DataObj11GetUserDataEv
std::uintptr_t DataObj::GetUserData() {
    mutex.Lock();
    std::uintptr_t value = user_data_;
    mutex.Unlock();
    return value;
}

// 0x8658a0 _ZN3vox7DataObj11SetUserDataERNS_18DataHandleUserDataE
void DataObj::SetUserData(const DataHandleUserData& user_data) {
    mutex.Lock();
    user_data_ = user_data.word;
    mutex.Unlock();
}

// 0x8658cc _ZN3vox7DataObj7IsReadyEv
// Returns true only for state 0 (the asm computes 1 - state when state <= 1).
bool DataObj::IsReady() {
    state_mutex_.Lock();
    bool ready = state_ == 0;
    state_mutex_.Unlock();
    return ready;
}

// 0x8658fc _ZN3vox7DataObj11GetDurationEv
// Partial: returns 0 when not ready. Otherwise the original reads the duration
// through three external helpers (0x30e2e0, 0x30e964, 0x30ec94) that are not
// reconstructed, so the port returns 0 there.
std::uint32_t DataObj::GetDuration() {
    if (!IsReady()) {
        return 0;
    }
    mutex.Lock();
    std::uint32_t duration = 0;
    mutex.Unlock();
    return duration;
}

// 0x865954 _ZN3vox7DataObj12GetDebugInfoERNS_21DebugChunk_dataSourceE
// Partial: depends on DebugChunk_dataSource, which is not reconstructed.
void DataObj::GetDebugInfo(void* chunk) {
    (void)chunk;
    (void)GetDuration();
}

// 0x8659ec _ZN3vox7DataObj10PrintDebugEv
// Partial: the debug printout itself is not reconstructed.
void DataObj::PrintDebug() { (void)GetDuration(); }

// 0x865a0c _ZN3vox7DataObj7IsGroupEj
// Tests bit group_ of the mask: the asm uses (mask & (1 << group_)) != 0.
bool DataObj::IsGroup(std::uint32_t groups) {
    mutex.Lock();
    bool member = (groups & (std::uint32_t{1} << group_)) != 0;
    mutex.Unlock();
    return member;
}

// 0x865a48 _ZN3vox7DataObj8GetGroupEv
int DataObj::GetGroup() {
    mutex.Lock();
    int group = group_;
    mutex.Unlock();
    return group;
}

// 0x865a70 _ZN3vox7DataObj9ShouldDieEv
// Partial: the asm walks the emitter list and checks need_to_die_ and the
// state, then returns (state == -1). The port returns that state check only;
// the list walk is not reconstructed.
bool DataObj::ShouldDie() {
    mutex.Lock();
    bool result = state_ == -1;
    mutex.Unlock();
    return result;
}

// 0x865aec _ZN3vox7DataObj9NeedToDieEv
// Sets need_to_die_ under the Handlable mutex, then moves the state to 4 under
// the state mutex.
void DataObj::NeedToDie() {
    mutex.Lock();
    need_to_die_ = true;
    mutex.Unlock();
    state_mutex_.Lock();
    state_ = 4;
    state_mutex_.Unlock();
}

// 0x865b30 _ZN3vox7DataObj17UnregisterEmitterEx
// Partial: the asm finds the list node with the id and the matching word at
// +0xc, unlinks it and frees it. The port removes by id only.
void DataObj::UnregisterEmitter(std::int64_t emitter_id, void* emitter) {
    (void)emitter;
    mutex.Lock();
    emitters_.remove_if([emitter_id](const std::pair<std::int64_t, void*>& e) { return e.first == emitter_id; });
    mutex.Unlock();
}

// 0x865ba8 _ZN3vox7DataObj15RegisterEmitterEx
// Partial: the asm allocates a 0x10-byte list node. The port appends the pair.
void DataObj::RegisterEmitter(std::int64_t emitter_id, void* emitter) {
    mutex.Lock();
    emitters_.emplace_back(emitter_id, emitter);
    mutex.Unlock();
}

// 0x866dec _ZN3vox7DataObj6UpdateEv
// Partial: the asm runs a state machine through two external objects (+0x38,
// +0x3c) and sets the state word to 0 or -1. Only the early return is ported:
// nothing happens while the state is 0.
void DataObj::Update() {
    if (state_ == 0) {
        return;
    }
    mutex.Lock();
    mutex.Unlock();
}

}  // namespace vox
