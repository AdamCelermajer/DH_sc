// TouchScreenWin32: the Win32 touch screen reports no bounds.
#pragma once

class TouchScreenWin32 {
public:
    int getLeftBound() const;     // 0x33d48c
    int getRightBound() const;    // 0x33d494
    int getTopBound() const;      // 0x33d49c
    int getBottomBound() const;   // 0x33d4a4
};
