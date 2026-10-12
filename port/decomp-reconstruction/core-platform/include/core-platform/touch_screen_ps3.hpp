// TouchScreenPS3: a pad-driven touch source. Bounds are fixed at zero, and mouse-style
// events (the pad's pointer) drive begin, move and end.
#pragma once

#include "core-platform/deps/glitch_core.hpp"
#include "core-platform/touch_screen_base.hpp"

class TouchScreenPS3 : public TouchScreenBase {
public:
    TouchScreenPS3();

    int getLeftBound() const override;    // 0x33d1f4
    int getRightBound() const override;   // 0x33d1fc
    int getTopBound() const override;     // 0x33d204
    int getBottomBound() const override;  // 0x33d20c

    // Maps a pointer event onto touch began/moved/ended. Returns true when consumed.
    bool onEvent(const glitch::SEvent& event);  // 0x33d21c

private:
    const void* eventReceiverVtable_ = nullptr;  // +0x1b4 (data address not recovered)
};
