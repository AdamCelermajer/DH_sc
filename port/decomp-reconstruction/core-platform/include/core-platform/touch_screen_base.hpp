// TouchScreenBase: eight-slot touch tracker. Each slot is 0x30 bytes in the original;
// the field roles below are inferred from the accessors.
#pragma once

class TouchScreenBase {
public:
    struct Slot {
        float point_[2] = {0, 0};  // position (the original returns the address at +0xc)
        bool valid_ = false;       // +0x20
        bool touched_ = false;     // +0x28
        int swipe_ = 0;            // +0x24
        int id_ = 0;               // +0x2c
        int state_ = 0;            // +0x30 (queue state, 2 = ended)
    };

    bool hasTouch() const;                 // 0x33b2ec
    int getID(long id);                    // 0x33b3dc
    void clear();                          // 0x33b4d8
    const float* getTouchPoint(long slot) const;     // 0x33b1a4
    const float* getFirstTouchPoint(long slot) const; // 0x33b1e0
    int getSwipeDirection(long slot) const;          // 0x33b2bc
    bool _IsQueueEmpty() const;            // 0x33a9a4

protected:
    Slot slots_[8];                        // +0x0 .. +0x0f0
    int queueHead_ = 0;                    // +0x1a0
    int queueTail_ = 0;                    // +0x1a4
};
