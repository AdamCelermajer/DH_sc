#include "vox-audio/vox/data_obj.hpp"

#include <cstring>

#include "vox-audio/vox/decoder.hpp"
#include "vox-audio/vox/stream.hpp"

namespace vox {

// 0x863c24 _ZN3vox7DataObjD1Ev. The original frees the emitter list nodes, resets the
// sentinel and destroys the state mutex and the Handlable mutex. The std::list and the
// member mutexes do the same here.
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
// Returns the bit pattern of a float: length / sample rate, read under the Handlable
// mutex, or 0.0f when the object is not ready (state != 0).
std::uint32_t DataObj::GetDuration() {
    float seconds = 0.0f;
    if (IsReady()) {
        mutex.Lock();
        seconds = static_cast<float>(static_cast<std::uint32_t>(length_)) /
                  static_cast<float>(sample_rate_);
        mutex.Unlock();
    }
    std::uint32_t bits = 0;
    std::memcpy(&bits, &seconds, sizeof(bits));
    return bits;
}

// 0x865954 _ZN3vox7DataObj12GetDebugInfoERNS_21DebugChunk_dataSourceE
// Fills the chunk under the Handlable mutex. The stream and decoder types come from
// their GetType virtuals (stream +0x1c, decoder +0x18). The duration is taken before
// the lock, as the original does.
void DataObj::GetDebugInfo(DebugChunk_dataSource& chunk) {
    const std::uint32_t duration_bits = GetDuration();
    mutex.Lock();
    chunk.id = id;
    chunk.stream_type = stream_->GetType();
    chunk.decoder_type = decoder_->GetType();
    chunk.priority_bank_id = priority_bank_id_;
    chunk.sample_rate = sample_rate_;
    chunk.channels = channels_;
    chunk.bits_per_sample = bits_per_sample_;
    std::memcpy(&chunk.duration, &duration_bits, sizeof(chunk.duration));
    chunk.group = group_;
    chunk.ref_count = ref_count;
    mutex.Unlock();
}

// 0x8659ec _ZN3vox7DataObj10PrintDebugEv
// The binary calls GetDuration and takes and releases the mutex. It prints nothing,
// so the port keeps the same calls and the result is discarded.
void DataObj::PrintDebug() {
    (void)GetDuration();
    mutex.Lock();
    mutex.Unlock();
}

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
// Under the Handlable mutex: true when no emitter is registered and the reference
// count is 0; otherwise true when need_to_die_ is set; otherwise true only in state -1.
// The original walks the emitter list to its sentinel before the flag tests. The walk
// only follows the links, so the port does not repeat it.
bool DataObj::ShouldDie() {
    mutex.Lock();
    bool result;
    if (emitters_.empty() && ref_count == 0) {
        result = true;
    } else if (!need_to_die_) {
        result = state_ == -1;
    } else {
        result = true;
    }
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
// Under the Handlable mutex, removes the first node whose id matches and frees it.
void DataObj::UnregisterEmitter(std::int64_t emitter_id) {
    mutex.Lock();
    for (auto it = emitters_.begin(); it != emitters_.end(); ++it) {
        if (*it == emitter_id) {
            emitters_.erase(it);
            break;
        }
    }
    mutex.Unlock();
}

// 0x865ba8 _ZN3vox7DataObj15RegisterEmitterEx
// Under the Handlable mutex, appends a node with the id at the tail.
void DataObj::RegisterEmitter(std::int64_t emitter_id) {
    mutex.Lock();
    emitters_.push_back(emitter_id);
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
