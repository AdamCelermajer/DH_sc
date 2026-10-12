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
