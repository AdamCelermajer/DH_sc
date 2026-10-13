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

// Width at +0x0 and Height at +0x4, the order ObjToStr writes them in.
template <typename T>
struct dimension2d {
    T Width, Height;
};
}  // namespace glitch::core

namespace glitch::video {
// Four colour bytes. The stream operators store the channels in this order
// (channel 3 is set to zero when a colour is read from text).
struct SColor {
    unsigned char R, G, B, A;
};
}  // namespace glitch::video

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

// Stand-in for glitch::IEventReceiver (glitch-core-io). The secondary vtable of
// TouchScreenPS3 and TouchScreenWin32 holds the two destructor entries and onEvent,
// so the interface is declared with those three slots.
class IEventReceiver {
public:
    virtual ~IEventReceiver() = default;
    virtual bool onEvent(const SEvent& event) = 0;
};
}  // namespace glitch
