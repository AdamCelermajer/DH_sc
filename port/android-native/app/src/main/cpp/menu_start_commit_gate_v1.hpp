#pragma once

namespace dh2::android_ui {

// NativeStartGame is committed to Java only after both source world loading
// and the authored player HUD attachment succeed.
constexpr bool menu_start_commit_gate_v1(bool world_loaded,
                                         bool player_hud_attached) noexcept {
    return world_loaded && player_hud_attached;
}

} // namespace dh2::android_ui
