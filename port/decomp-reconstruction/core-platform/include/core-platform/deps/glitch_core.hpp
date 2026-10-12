// Minimal stand-ins for glitch (custom Irrlicht) types used by this category.
// Layout matches the original: plain float/int members, no vtable.
#pragma once

namespace glitch::core {
template <typename T>
struct vector3d {
    T X, Y, Z;
};
}  // namespace glitch::core

namespace glitch::memory {
// Allocation hint. The enumerator values were not recovered from the binary;
// only the type identity matters to the allocator wrappers.
enum E_MEMORY_HINT : int {
    EMH_UNRECOVERED = 0,
};
}  // namespace glitch::memory

namespace glitch::core {
// Axis-aligned rectangle; the original stores x1, y1, x2, y2 in that order.
template <typename T>
struct rect {
    T X1, Y1, X2, Y2;
};
}  // namespace glitch::core

namespace glitch {
// Input event. Only the fields read by TouchScreenWin32::onEvent are named:
// [0] type, [2] mouse X, [3] mouse Y, [5] mouse event code (word indices).
struct SEvent {
    int type;
    int reserved1;
    int mouseX;
    int mouseY;
    int reserved4;
    int mouseEvent;
    int reserved6;
    int reserved7;
};
}  // namespace glitch
