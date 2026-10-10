#pragma once

namespace dh::foundation::platform_key {

// Stable key identities used by the game host. Values deliberately match the
// Windows virtual-key values already consumed by the shared main loop, so the
// Linux adapter can preserve existing input policy without changing gameplay.
inline constexpr int mouse_left = 0x01;
inline constexpr int tab = 0x09;
inline constexpr int enter = 0x0d;  // Preview 15 boot press/tap (VK_RETURN)
inline constexpr int shift = 0x10;
inline constexpr int space = 0x20;
inline constexpr int left = 0x25;
inline constexpr int up = 0x26;
inline constexpr int right = 0x27;
inline constexpr int down = 0x28;
inline constexpr int escape = 0x1b;
inline constexpr int f5 = 0x74;
inline constexpr int f9 = 0x78;

} // namespace dh::foundation::platform_key

// The current shared main loop uses Win32 key names as its stable host API.
// Keep those identifiers available when building that loop on Linux.
#if !defined(_WIN32)
inline constexpr int VK_LBUTTON = 0x01;
inline constexpr int VK_TAB = 0x09;
inline constexpr int VK_RETURN = 0x0d;
inline constexpr int VK_SHIFT = 0x10;
inline constexpr int VK_SPACE = 0x20;
inline constexpr int VK_LEFT = 0x25;
inline constexpr int VK_UP = 0x26;
inline constexpr int VK_RIGHT = 0x27;
inline constexpr int VK_DOWN = 0x28;
inline constexpr int VK_ESCAPE = 0x1b;
inline constexpr int VK_F5 = 0x74;
inline constexpr int VK_F9 = 0x78;
#endif
