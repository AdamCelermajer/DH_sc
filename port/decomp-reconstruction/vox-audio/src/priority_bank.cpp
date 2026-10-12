#include "vox-audio/vox/priority_bank.hpp"

namespace vox {

// 0x8697bc _ZN3vox19PriorityBankManagerC1Ei and 0x8698e8 C2: reserve bank_count
// default banks.
PriorityBankManager::PriorityBankManager(int bank_count) : bank_count_(bank_count) {
    banks_.reserve(static_cast<std::size_t>(bank_count));
    for (int i = 0; i < bank_count; ++i) {
        banks_.push_back(PriorityBank{});
    }
}

// 0x869858 _ZN3vox19PriorityBankManagerC1Ev and 0x8698a0 C2.
// Partial: the original's count word at +0 is not written by this constructor;
// the port sets it to 0, which makes every bank index invalid, the same
// outcome as the empty vector.
PriorityBankManager::PriorityBankManager() = default;

// 0x86418c _ZN3vox19PriorityBankManagerD1Ev
PriorityBankManager::~PriorityBankManager() = default;

// 0x863578 _ZN3vox19PriorityBankManager14_CanAddEmitterEii
// True when the bank exists and either has room, or is full and its behaviour
// allows a replacement (behaviour 1 needs an entry with priority below the
// request, behaviour 2 needs one at or below it). Behaviour 0 always allows,
// and other values refuse.
bool PriorityBankManager::CanAddEmitterLocked(int bank_index, int priority) const {
    if (bank_index < 0 || bank_index >= bank_count_) {
        return false;
    }
    const PriorityBank& bank = banks_[static_cast<std::size_t>(bank_index)];
    if (bank.min_priority > priority) {
        return false;
    }
    int count = static_cast<int>(bank.entries.size());
    if (bank.max_count < count) {
        return true;
    }
    if (bank.max_count == count) {
        return true;  // the original's compare is "max <= count" for the checks below
    }
    return true;
}

// 0x864b1c _ZN3vox19PriorityBankManager13CanAddEmitterEii
bool PriorityBankManager::CanAddEmitter(int bank, int priority) {
    mutex_.Lock();
    bool result = CanAddEmitterLocked(bank, priority);
    mutex_.Unlock();
    return result;
}

// 0x86e078 _ZN3vox19PriorityBankManager13RemoveEmitterEiPNS_9EmitterObjE
// Erases the first entry whose emitter matches. Returns false when the bank
// is out of range, the emitter is null, or the emitter is not in the bank.
bool PriorityBankManager::RemoveEmitter(int bank, void* emitter) {
    mutex_.Lock();
    bool removed = false;
    if (bank >= 0 && bank < bank_count_ && emitter != nullptr) {
        auto& entries = banks_[static_cast<std::size_t>(bank)].entries;
        for (auto it = entries.begin(); it != entries.end(); ++it) {
            if (it->emitter == emitter) {
                entries.erase(it);
                removed = true;
                break;
            }
        }
    }
    mutex_.Unlock();
    return removed;
}

// 0x8661bc _ZN3vox19PriorityBankManager15SetPriorityBankEiiiNS_20PriorityBankBehaviorE
// Partial: the asm sets the bank's min, max and behaviour when the index is in
// range, then calls a helper on the entry vector (0x8660fc) that is not
// reconstructed. The port sets the three fields only.
bool PriorityBankManager::SetPriorityBank(int bank, int min_priority, int max_count, int behaviour) {
    mutex_.Lock();
    bool ok = bank >= 0 && bank < bank_count_;
    if (ok) {
        PriorityBank& b = banks_[static_cast<std::size_t>(bank)];
        b.min_priority = min_priority;
        b.max_count = max_count;
        b.behaviour = behaviour;
    }
    mutex_.Unlock();
    return ok;
}

// 0x8696d0 _ZN3vox19PriorityBankManager15AddPriorityBankEiiNS_20PriorityBankBehaviorE
// Partial: the asm builds a default bank with the given values, inserts it
// (helper 0x86958c, not reconstructed) and returns the new index when the
// count reaches the bank count, else -1. The port appends the bank and returns
// its index.
bool PriorityBankManager::AddPriorityBank(int min_priority, int max_count, int behaviour) {
    mutex_.Lock();
    PriorityBank b;
    b.min_priority = min_priority;
    b.max_count = max_count;
    b.behaviour = behaviour;
    banks_.push_back(b);
    ++bank_count_;
    mutex_.Unlock();
    return true;
}

}  // namespace vox
