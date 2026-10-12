// TouchScreenWin32: mouse-driven touch input. The Win32 touch screen reports no
// bounds, so the four bound getters return zero.
#pragma once

#include "core-platform/deps/glitch_core.hpp"
#include "core-platform/touch_screen_base.hpp"

class TouchScreenWin32 : public TouchScreenBase {
public:
    TouchScreenWin32(short width, short height);

    int getLeftBound() const override;    // 0x33d48c
    int getRightBound() const override;   // 0x33d494
    int getTopBound() const override;     // 0x33d49c
    int getBottomBound() const override;  // 0x33d4a4

    // Maps mouse events onto touch begin/move/end. Returns true when consumed.
    bool onEvent(const glitch::SEvent& event);  // 0x33d4b4

private:
    int eventReceiverVtable_ = 0;  // +0x1b4 secondary interface vtable (not recovered)
};
