#pragma once
// B082: the source black fades (shape496 in sprite742/743) cover only the 480x320 SWF stage. The port
// letterboxes that stage into the window, so on a wider/taller window the bars outside the stage stayed
// un-faded and the game world remained visible beside the "black screen". The original stage fills the
// device screen; here the outside bars take the same black alpha as the stage fade.
#include "../../overlay_renderer.hpp"

#include <array>
#include <vector>

namespace dh::foundation::persistence::death_flow {

struct LetterboxRectV1 {
    float x0 = 0, y0 = 0, x1 = 0, y1 = 0;
};

// Four non-overlapping rectangles (left, right, top, bottom) covering the window minus the stage rectangle.
// Empty (zero-area) bars are returned with x1<=x0 or y1<=y0 and must be skipped by the caller.
inline std::array<LetterboxRectV1, 4> letterbox_bar_rects_v1(float viewport_width, float viewport_height,
                                                             float stage_scale, float stage_ox, float stage_oy) {
    const float x0 = stage_ox, x1 = stage_ox + 480.f * stage_scale;
    const float y0 = stage_oy, y1 = stage_oy + 320.f * stage_scale;
    return {{{0.f, 0.f, x0, viewport_height}, {x1, 0.f, viewport_width, viewport_height},
             {x0, 0.f, x1, y0}, {x0, y1, x1, viewport_height}}};
}

inline bool draw_letterbox_black_v1(OverlayRenderer& overlay, int viewport_width, int viewport_height,
                                    float stage_scale, float stage_ox, float stage_oy,
                                    std::uint8_t alpha) {
    if (alpha == 0 || viewport_width <= 0 || viewport_height <= 0) return true;
    const std::array<float, 4> black{0.f, 0.f, 0.f, static_cast<float>(alpha) / 255.f};
    for (const auto& r : letterbox_bar_rects_v1(static_cast<float>(viewport_width), static_cast<float>(viewport_height),
                                                stage_scale, stage_ox, stage_oy)) {
        if (r.x1 - r.x0 < 0.5f || r.y1 - r.y0 < 0.5f) continue;
        const std::vector<OverlayTriangleVertex> v{
            {r.x0, r.y0, 0.f, 0.f}, {r.x1, r.y0, 0.f, 0.f}, {r.x1, r.y1, 0.f, 0.f},
            {r.x0, r.y0, 0.f, 0.f}, {r.x1, r.y1, 0.f, 0.f}, {r.x0, r.y1, 0.f, 0.f}};
        if (!overlay.drawTriangles(v, 0, black)) return false;
    }
    return true;
}

} // namespace dh::foundation::persistence::death_flow
