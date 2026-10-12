#include "net-session-online/multiplayer_state_machine.hpp"

#include <cstdlib>

namespace {
// Unlinks an entry from the list and frees it. The count is reduced by the caller.
}  // namespace

// 0x4a00d8 _ZN23MultiplayerStateMachine16initStateMachineEv
void MultiplayerStateMachine::initStateMachine() {
    head_ = nullptr;
    state_ = -1;
    tail_ = &head_;
    count_ = 0;
}

// 0x4a00f8 _ZN23MultiplayerStateMachine29replaceStateEntryCallbackFuncEiPFiPvE
void MultiplayerStateMachine::replaceStateEntryCallbackFunc(std::int32_t stateId, StateCallback callback) {
    for (Entry* e = head_; e != nullptr; e = e->next) {
        if (e->stateId == stateId) {
            e->callback = callback;
            return;
        }
    }
}

// 0x4a012c _ZN23MultiplayerStateMachine9skipStateEi
void MultiplayerStateMachine::skipState(std::int32_t stateId) {
    for (Entry* e = head_; e != nullptr; e = e->next) {
        if (e->stateId == stateId) {
            e->skip = true;
            return;
        }
    }
}

// 0x4a0164 _ZN23MultiplayerStateMachine15runStateMachineEv
// The state is read again for each entry, so a callback that changes it affects the
// entries after it.
void MultiplayerStateMachine::runStateMachine() {
    if (count_ == 0 || state_ == -1 || head_ == nullptr) {
        return;
    }
    for (Entry* e = head_; e != nullptr; e = e->next) {
        if (state_ == e->stateId && !e->skip) {
            const std::int32_t next = e->callback(nullptr);
            if (next >= 0) {
                state_ = next;
            }
        }
    }
}

// 0x4a01dc _ZN23MultiplayerStateMachine16removeStateEntryEi
void MultiplayerStateMachine::removeStateEntry(std::int32_t stateId) {
    for (Entry* e = head_; e != nullptr; e = e->next) {
        if (e->stateId == stateId) {
            // The link that held e now holds e->next. The next entry's link follows.
            if (e->next == nullptr) {
                tail_ = e->link;
            } else {
                e->next->link = e->link;
            }
            *e->link = e->next;
            delete e;
            --count_;
            return;
        }
    }
}

// 0x4a0254 _ZN23MultiplayerStateMachine19destroyStateMachineEv
// Partial: the original does not reset the count or the state here. They are kept
// unchanged, as in the original.
void MultiplayerStateMachine::destroyStateMachine() {
    while (head_ != nullptr) {
        Entry* e = head_;
        if (e->next == nullptr) {
            tail_ = e->link;
        } else {
            e->next->link = e->link;
        }
        *e->link = e->next;
        delete e;
    }
}

// 0x4a02a0 _ZN23MultiplayerStateMachine13addStateEntryEiPFiPvEb
void MultiplayerStateMachine::addStateEntry(std::int32_t stateId, StateCallback callback, bool skip) {
    Entry* e = new Entry{skip, stateId, callback, nullptr, tail_};
    *tail_ = e;
    tail_ = &e->next;
    ++count_;
}
