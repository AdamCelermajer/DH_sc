#include "core-platform/touch_screen_win32.hpp"

// 0x33d68c / 0x33d6d0 TouchScreenWin32::TouchScreenWin32(short, short)
TouchScreenWin32::TouchScreenWin32(short width, short height) : TouchScreenBase(width, height) {}

// 0x33d48c TouchScreenWin32::getLeftBound() const
int TouchScreenWin32::getLeftBound() const { return 0; }

// 0x33d494 TouchScreenWin32::getRightBound() const
int TouchScreenWin32::getRightBound() const { return 0; }

// 0x33d49c TouchScreenWin32::getTopBound() const
int TouchScreenWin32::getTopBound() const { return 0; }

// 0x33d4a4 TouchScreenWin32::getBottomBound() const
int TouchScreenWin32::getBottomBound() const { return 0; }

// 0x33d4b4 TouchScreenWin32::onEvent(glitch::SEvent const&)
// Mouse event code 0 (left down) begins a touch, 6 (move) continues it only
// while the button is held, and 3 (left up) ends it. Point is (mouseX, mouseY).
// Partial: the original's Point2D stack layout is split across two locals by the
// decompiler, and the begin call passes only the point (no id), so the id is
// taken as 0 here.
bool TouchScreenWin32::onEvent(const glitch::SEvent& event) {
    static bool isDown = false;  // _ZZ...onEvent...isDown in the original
    if (event.type != 1) {
        return false;
    }
    const Point2D<short> p{static_cast<short>(event.mouseX), static_cast<short>(event.mouseY)};
    switch (event.mouseEvent) {
        case 3:
            isDown = false;
            touchEnded(p, 0);
            return true;
        case 6:
            if (isDown) {
                touchMoved(p, 0);
                return true;
            }
            return false;
        case 0:
            isDown = true;
            touchBegan(p, 0);
            return true;
        default:
            return false;
    }
}
