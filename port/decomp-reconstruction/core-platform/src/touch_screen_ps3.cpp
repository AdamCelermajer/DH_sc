#include "core-platform/touch_screen_ps3.hpp"

// 0x33d3f4 / 0x33d440 TouchScreenPS3::TouchScreenPS3()
// The original sets the active count (+0x190) to zero and points +0x1b4 at a
// data address that is not recovered; nullptr stands in for it.
TouchScreenPS3::TouchScreenPS3() : TouchScreenBase(0, 0) {}

// 0x33d1f4 TouchScreenPS3::getLeftBound() const
int TouchScreenPS3::getLeftBound() const { return 0; }

// 0x33d1fc TouchScreenPS3::getRightBound() const
int TouchScreenPS3::getRightBound() const { return 0; }

// 0x33d204 TouchScreenPS3::getTopBound() const
int TouchScreenPS3::getTopBound() const { return 0; }

// 0x33d20c TouchScreenPS3::getBottomBound() const
int TouchScreenPS3::getBottomBound() const { return 0; }

// 0x33d21c TouchScreenPS3::onEvent(glitch::SEvent const&)
// Type 1 events only. Event code 0 begins a touch, 6 moves it while the pad
// button is held, and 3 ends it. The point is (mouseX, mouseY) as shorts. The
// held state is a function-local static, as in the original.
bool TouchScreenPS3::onEvent(const glitch::SEvent& event) {
    static bool isDown = false;
    if (event.type != 1) {
        return false;
    }
    switch (event.mouseEvent) {
        case 3: {
            isDown = false;
            touchEnded(Point2D<short>{static_cast<short>(event.mouseX), static_cast<short>(event.mouseY)}, 0);
            return true;
        }
        case 6: {
            if (isDown) {
                touchMoved(Point2D<short>{static_cast<short>(event.mouseX), static_cast<short>(event.mouseY)}, 0);
                return true;
            }
            return false;
        }
        case 0: {
            isDown = true;
            touchBegan(Point2D<short>{static_cast<short>(event.mouseX), static_cast<short>(event.mouseY)}, 0);
            return true;
        }
        default:
            return false;
    }
}
