#include "core-platform/touch_screen_base.hpp"

// 0x33a9a4 TouchScreenBase::_IsQueueEmpty() const
bool TouchScreenBase::_IsQueueEmpty() const { return queueHead_ == queueTail_; }

// 0x33b2ec TouchScreenBase::hasTouch()
// True when any slot is touched and valid, with the original's search order.
bool TouchScreenBase::hasTouch() const {
    for (const Slot& s : slots_) {
        if (s.touched_ && s.valid_) {
            return true;
        }
    }
    return false;
}

// 0x33b3dc TouchScreenBase::getID(long)
// Returns the slot holding touch id, or claims the first free slot for it.
int TouchScreenBase::getID(long id) {
    int firstFree = -1;
    for (int i = 0; i < 8; ++i) {
        if (slots_[i].id_ == id) {
            return i;
        }
        if (!slots_[i].touched_ && firstFree == -1) {
            firstFree = i;
        }
    }
    slots_[firstFree].id_ = static_cast<int>(id);
    return firstFree;
}

// 0x33b4d8 TouchScreenBase::clear()
// Partial: the original logs "TouchScreenBase::clear()" through _DEBUG_OUTPKcz;
// that debug line is omitted here.
void TouchScreenBase::clear() {
    for (Slot& s : slots_) {
        s.touched_ = false;
    }
}

// 0x33b1a4 TouchScreenBase::getTouchPoint(long)
const float* TouchScreenBase::getTouchPoint(long slot) const {
    const Slot& s = slots_[slot];
    if (s.touched_ && s.valid_) {
        return s.point_;
    }
    return nullptr;
}

// 0x33b1e0 TouchScreenBase::getFirstTouchPoint(long)
const float* TouchScreenBase::getFirstTouchPoint(long slot) const {
    const Slot& s = slots_[slot];
    if (s.touched_ && s.valid_) {
        return s.point_;
    }
    return nullptr;
}

// 0x33b2bc TouchScreenBase::getSwipeDirection(long) const
int TouchScreenBase::getSwipeDirection(long slot) const {
    const Slot& s = slots_[slot];
    if (s.touched_ && s.valid_) {
        return s.swipe_;
    }
    return 0;
}
