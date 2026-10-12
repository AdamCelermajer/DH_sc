// Minimal stand-ins for glitch (custom Irrlicht) core types used by this
// category. Layout matches the original: plain float/int members, no vtable.
#pragma once

namespace glitch::core {
template <typename T>
struct vector3d {
    T X, Y, Z;
};
}  // namespace glitch::core
