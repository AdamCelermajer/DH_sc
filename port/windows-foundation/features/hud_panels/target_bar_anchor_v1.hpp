#pragma once
// w5-mud2: screen anchor of the enemy target bar (HUDelements.HealthBars.enemy).
//
// Original: InfoHUDManager::FastUpdate (0x41e064) shows the bar for as long as the local player's AI target is a monster
// (CharAI+968 target, or the Character+5284 object of interest) and hides it only when that target is gone. Nothing in
// the original drops the bar because the monster is tall or near a screen edge. The port anchors the bar bottom-centre to
// the projected head of the target; a head above or beside the viewport used to fail the projection and the bar vanished
// while the player kept attacking (a rearing Mud Lord at the top edge). The anchor is now always available: a head outside
// the viewport keeps its (off-screen) projected position, which the caller clamps into the window; a head behind the eye
// (no projection) uses the top centre.
#include <array>
#include <cmath>

namespace dh::foundation {

struct TargetBarAnchorV1 {
    std::array<float, 2> screen{}; // surface pixels, may lie outside [0,width]x[0,height] (caller clamps)
    bool on_screen = false;        // the head itself is inside the viewport (old behaviour: the bar was drawn only then)
};

// clip = homogeneous clip-space coordinates (x, y, z, w) of the head point.
inline TargetBarAnchorV1 target_bar_anchor_v1(const float clip[4], int width, int height) {
    TargetBarAnchorV1 out;
    if (!std::isfinite(clip[0]) || !std::isfinite(clip[1]) || !std::isfinite(clip[2]) || !std::isfinite(clip[3]) || clip[3] <= 0.0f) {
        out.screen = {static_cast<float>(width) * 0.5f, 0.0f};
        return out;
    }
    const float x = clip[0] / clip[3], y = clip[1] / clip[3], z = clip[2] / clip[3];
    out.screen = {(x + 1.0f) * static_cast<float>(width) * 0.5f, (1.0f - y) * static_cast<float>(height) * 0.5f};
    out.on_screen = !(x < -1 || x > 1 || y < -1 || y > 1 || z < -1 || z > 1);
    return out;
}

// Top-left of a bar whose local bounds are [xmin,xmax]x[ymin,ymax] (target-local pixels, scaled by `scale`): bottom-centre on
// the anchor, then clamped so the complete bar stays inside the window.
inline std::array<float, 2> target_bar_origin_v1(const TargetBarAnchorV1& anchor, const std::array<float, 4>& bounds, float scale,
                                                 int width, int height) {
    const auto bounded = [](float value, float low, float high) { return low > high ? low : (value < low ? low : (value > high ? high : value)); };
    const float left = bounded(anchor.screen[0] - (bounds[0] + bounds[1]) * 0.5f * scale, -bounds[0] * scale, static_cast<float>(width) - bounds[1] * scale);
    const float top = bounded(anchor.screen[1] - bounds[3] * scale, -bounds[2] * scale, static_cast<float>(height) - bounds[3] * scale);
    return {left, top};
}

} // namespace dh::foundation
