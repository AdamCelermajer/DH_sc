// vox::RandomGroup state, reset and previous-state members (groups slice).
// words_[2] is the cycle count, words_[3] the remaining count, words_[4] and words_[5] the
// saved pair; words_[0] and words_[1] are the reset values (see groups.hpp).
#include <cstddef>

#include "vox-audio/vox/groups.hpp"
#include "vox-audio/vox/memory.hpp"

namespace vox {
namespace {

RandomGroupNode* AllocRemovedNode(RandomGroupElement* element) {
    auto* node = static_cast<RandomGroupNode*>(VoxAlloc(sizeof(RandomGroupNode)));
    node->next = nullptr;
    node->prev = nullptr;
    node->element = element;
    return node;
}

// Appends node before the sentinel, which is the tail of the ring.
void LinkRemovedAtTail(RandomGroupNode& sentinel, RandomGroupNode* node) {
    RandomGroupNode* tail = sentinel.prev;
    node->next = &sentinel;
    node->prev = tail;
    tail->next = node;
    sentinel.prev = node;
}

void UnlinkRemoved(RandomGroupNode* node) {
    RandomGroupNode* prev = node->prev;
    RandomGroupNode* next = node->next;
    prev->next = next;
    next->prev = prev;
}

void FreeRemovedNodes(RandomGroupNode& sentinel) {
    RandomGroupNode* node = sentinel.next;
    while (node != &sentinel) {
        RandomGroupNode* next = node->next;
        VoxFree(node);
        node = next;
    }
    sentinel.next = &sentinel;
    sentinel.prev = &sentinel;
}

}  // namespace

// 0x881bb0 _ZN3vox11RandomGroup8GetStateEPNS_16RandomGroupStateE
void RandomGroup::GetState(RandomGroupState* state) const {
    state->cycles = static_cast<std::int32_t>(words_[2]);
    state->remaining = static_cast<std::int32_t>(words_[3]);
    state->current = current_;
    state->total_weight = total_weight_;
    state->cycle_remaining = cycle_remaining_;
    state->saved_cycles = static_cast<std::int32_t>(words_[4]);
    state->saved_remaining = static_cast<std::int32_t>(words_[5]);
    state->prev_index = prev_index_;
    state->prev_cycle_remaining = prev_cycle_remaining_;
    state->elements = &elements_;
    state->removed = &removed_;
}

// 0x882f48 _ZN3vox11RandomGroup8SetStateEPNS_16RandomGroupStateE
// The element vector is replaced by the pointers the state refers to, and the removed list
// by a copy of the nodes it refers to (the original frees the old nodes first).
void RandomGroup::SetState(const RandomGroupState* state) {
    elements_.clear();
    words_[2] = static_cast<std::uint32_t>(state->cycles);
    words_[3] = static_cast<std::uint32_t>(state->remaining);
    current_ = state->current;
    total_weight_ = state->total_weight;
    cycle_remaining_ = state->cycle_remaining;
    words_[4] = static_cast<std::uint32_t>(state->saved_cycles);
    words_[5] = static_cast<std::uint32_t>(state->saved_remaining);
    prev_index_ = state->prev_index;
    prev_cycle_remaining_ = state->prev_cycle_remaining;

    if (state->elements != nullptr) {
        const std::vector<RandomGroupElement*>& source = *state->elements;
        const std::size_t source_size = source.size();
        for (std::size_t i = 0; i < source_size; ++i) {
            elements_.push_back(source[i]);
        }
    }

    FreeRemovedNodes(removed_);
    if (state->removed != nullptr) {
        for (const RandomGroupNode* node = state->removed->next; node != state->removed; node = node->next) {
            LinkRemovedAtTail(removed_, AllocRemovedNode(node->element));
        }
    }
}

// 0x8834f0 _ZN3vox11RandomGroup8SetStateERS0_
// Partial. The base copy, the scalar fields and the copy of the first source-size element
// pointers are plain. When this vector is longer than the source, the original pushes the
// value of the first removed element (surplus times) and links one more node at the head. As
// written in the pseudocode that relink does not keep the ring closed when the removed list is
// not empty, and the destructor would not terminate on it; the port keeps the same steps. When
// the removed list is empty the original reads the element-count word after the sentinel as the
// padding pointer; the port uses null there.
void RandomGroup::SetState(const RandomGroup& other) {
    SegmentGroup::SetState(other);

    auto first_removed_element = [this]() -> RandomGroupElement* {
        RandomGroupNode* head = removed_.next;
        return head == &removed_ ? nullptr : head->element;
    };

    const std::size_t source_size = other.elements_.size();
    const std::ptrdiff_t difference =
        static_cast<std::ptrdiff_t>(source_size) - static_cast<std::ptrdiff_t>(elements_.size());
    if (difference > 0) {
        for (std::ptrdiff_t k = 0; k < difference; ++k) {
            elements_.push_back(first_removed_element());
        }
    } else if (difference < 0) {
        RandomGroupElement* last = nullptr;
        for (std::ptrdiff_t k = 0; k < -difference; ++k) {
            last = first_removed_element();
            elements_.push_back(last);
        }
        RandomGroupNode* node = AllocRemovedNode(last);
        RandomGroupNode* head = removed_.next;
        RandomGroupNode* tail = head->prev;
        node->next = head;
        node->prev = tail;
        tail->next = node;
        head->prev = node;
    }
    for (std::size_t i = 0; i < source_size; ++i) {
        elements_[i] = other.elements_[i];
    }

    exclusion_window_ = other.exclusion_window_;
    raw_window_ = other.raw_window_;
    total_weight_ = other.total_weight_;
    cycle_remaining_ = other.cycle_remaining_;
    prev_cycle_remaining_ = other.prev_cycle_remaining_;
    current_ = other.current_;
    prev_index_ = other.prev_index_;
    reinsert_flag_ = other.reinsert_flag_;
}

// 0x883088 _ZN3vox11RandomGroup5ResetEi
// Every removed element goes back to the end of the vector, in list order, and its node is
// freed. The current state moves to the saved fields, the cycle is restarted from the element
// count, and with reset mode 0 the second word pair is restored from the reset values.
int RandomGroup::Reset(int reset_mode) {
    while (removed_.next != &removed_) {
        RandomGroupNode* node = removed_.next;
        RandomGroupElement* element = node->element;
        total_weight_ += element->weight_;
        elements_.push_back(element);
        UnlinkRemoved(node);
        VoxFree(node);
    }

    const std::int32_t old_current = current_;
    const std::int32_t old_cycles = static_cast<std::int32_t>(words_[2]);
    const std::int32_t old_cycle_remaining = cycle_remaining_;
    cycle_remaining_ = element_count_;
    prev_index_ = old_current;
    words_[2] = words_[0];
    words_[4] = static_cast<std::uint32_t>(old_cycles);
    prev_cycle_remaining_ = old_cycle_remaining;

    const int result = GetActiveElementIndex();
    if (reset_mode == 0) {
        words_[5] = words_[3];
        words_[3] = words_[1];
    }
    current_ = result;
    return result;
}

// 0x883404 _ZN3vox11RandomGroup18SetToPreviousStateEv
// Undoes the last GetGroupElementPosition: the saved values come back, and when the window
// removed an element (reinsert flag), the last vector element goes to the removed list and the
// tail of that list returns to the vector.
void RandomGroup::SetToPreviousState() {
    words_[2] = words_[4];
    words_[3] = words_[5];
    current_ = prev_index_;
    cycle_remaining_ = prev_cycle_remaining_;

    if (exclusion_window_ > 0) {
        RandomGroupElement* popped = nullptr;
        if (reinsert_flag_) {
            popped = elements_.back();
            elements_.pop_back();
            total_weight_ -= popped->weight_;
        }

        elements_.push_back(removed_.prev->element);

        RandomGroupNode* tail = removed_.prev;
        total_weight_ += tail->element->weight_;
        UnlinkRemoved(tail);
        VoxFree(tail);

        if (reinsert_flag_) {
            LinkRemovedAtTail(removed_, AllocRemovedNode(popped));
        }
    }
}

}  // namespace vox
