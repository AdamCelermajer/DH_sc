#include "vox-audio/vox/groups.hpp"

namespace vox {

// 0x881a74 _ZN3vox12SegmentGroupD2Ev, 0x881a78 _ZN3vox12SegmentGroupD1Ev: empty bodies.
SegmentGroup::~SegmentGroup() = default;

// 0x881a8c _ZN3vox12SegmentGroup8SetStateERS0_
// Copies valid, select mode and the six state words (asm: +0x04, +0x08, +0x0c..+0x20).
void SegmentGroup::SetState(const SegmentGroup& other) {
    valid_ = other.valid_;
    for (int i = 0; i < 6; ++i) {
        words_[i] = other.words_[i];
    }
    select_mode_ = other.select_mode_;
}

// 0x881d30 _ZN3vox15SequentialGroup8GetStateEPNS_20SequentialGroupStateE
void SequentialGroup::GetState(State* state) const {
    state->v[0] = static_cast<std::int32_t>(words_[2]);
    state->v[1] = static_cast<std::int32_t>(words_[3]);
    state->v[2] = index_;
    state->v[3] = static_cast<std::int32_t>(words_[4]);
    state->v[4] = static_cast<std::int32_t>(words_[5]);
    state->v[5] = prev_index_;
}

// 0x881dfc _ZN3vox15SequentialGroup8SetStateEPNS_20SequentialGroupStateE
void SequentialGroup::SetState(const State* state) {
    words_[2] = static_cast<std::uint32_t>(state->v[0]);
    words_[3] = static_cast<std::uint32_t>(state->v[1]);
    index_ = state->v[2];
    words_[4] = static_cast<std::uint32_t>(state->v[3]);
    words_[5] = static_cast<std::uint32_t>(state->v[4]);
    prev_index_ = state->v[5];
}

// 0x881dd8 _ZN3vox15SequentialGroup8SetStateERS0_
void SequentialGroup::SetState(const SequentialGroup& other) {
    SegmentGroup::SetState(other);
    index_ = other.index_;
    prev_index_ = other.prev_index_;
}

// 0x881d64 _ZN3vox15SequentialGroup22PeekAtNextGroupElementENS_13GroupPeekModeE
// Mode kNormal returns -1 when either current state word is 0. Otherwise the
// element at the current index is returned (no bounds check, as the original).
int SequentialGroup::PeekAtNextGroupElement(GroupPeekMode mode) const {
    if (mode == GroupPeekMode::kNormal && (words_[3] == 0 || words_[2] == 0)) {
        return -1;
    }
    return elements_[static_cast<std::size_t>(index_)];
}

// 0x881da0 _ZN3vox15SequentialGroup5ResetEi
// Saves the current state and resets the index. reset_mode 0 also restores the
// second word pair from the reset values, and the saved second word is set to
// the old current value.
void SequentialGroup::Reset(int reset_mode) {
    std::uint32_t old_a = words_[2];
    std::uint32_t old_b = words_[3];
    words_[4] = old_a;
    words_[2] = words_[0];
    prev_index_ = index_;
    index_ = 0;
    if (reset_mode == 0) {
        words_[5] = old_b;
        words_[3] = words_[1];
    }
}

// 0x881e30 _ZN3vox15SequentialGroup18SetToPreviousStateEv
void SequentialGroup::SetToPreviousState() {
    words_[2] = words_[4];
    words_[3] = words_[5];
    index_ = prev_index_;
}

// 0x88387c _ZN3vox15SequentialGroup10AddElementEi
void SequentialGroup::AddElement(int value) { elements_.push_back(value); }

// 0x882888 D1, 0x8828d0 D2, 0x882c68 deleting: the vector is released by its member.
SequentialGroup::~SequentialGroup() = default;

// 0x881c0c _ZN3vox11RandomGroup22PeekAtNextGroupElementENS_13GroupPeekModeE
// Returns the first word of the element at the current index, or -1 when the
// index is negative.
int RandomGroup::PeekAtNextGroupElement(GroupPeekMode mode) const {
    (void)mode;
    if (current_ < 0) {
        return -1;
    }
    return *reinterpret_cast<const int*>(elements_[static_cast<std::size_t>(current_)]);
}

// 0x8836b0 _ZN3vox11RandomGroup10AddElementEPNS_18RandomGroupElementE
// Partial: the asm allocates an 8-byte block and the element insert is not
// reconstructed. The port appends the pointer.
void RandomGroup::AddElement(RandomGroupElement* element) { elements_.push_back(element); }

}  // namespace vox

namespace vox {

// 0x881cc0 _ZN3vox15SequentialGroup23GetGroupElementPositionEv
// Partial: an empty element vector returns -1 here; the original reads element 0 of an
// empty (null) vector in that case.
int SequentialGroup::GetGroupElementPosition() {
    std::uint32_t remaining = words_[3];  // +0x18
    std::uint32_t cycles = words_[2];     // +0x14
    if (remaining == 0 || cycles == 0 || elements_.empty()) {
        return -1;
    }
    int index = index_;                   // +0x30, read before the advance
    int next = index + 1;
    index_ = next;
    int count = static_cast<int>(elements_.size());
    if (count <= next) {
        words_[4] = cycles;               // +0x1c: saved cycle count
        index_ = 0;
        words_[2] = cycles - 1;           // +0x14: one cycle used up
    }
    prev_index_ = index;                  // +0x34
    words_[3] = remaining - 1;            // +0x18
    words_[5] = remaining;                // +0x20: saved remaining count
    return elements_[static_cast<std::size_t>(index)];
}

}  // namespace vox
