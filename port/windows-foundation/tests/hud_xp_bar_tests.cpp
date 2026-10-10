// B038: the original XP bar (shape 148 background, shape 150 shrinking cover) must render
// its visible fill from the live XP frame. Checks rasterised coverage, not only numbers.
#include "../hud_geometry.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace dh::foundation;
namespace {
void check(bool condition, const std::string& why) {
    if (!condition) throw std::runtime_error(why);
}

bool inside_triangle(const HudGeometryVertex* t, float x, float y) {
    const auto side = [](float px, float py, float ax, float ay, float bx, float by) {
        return (px - bx) * (ay - by) - (ax - bx) * (py - by);
    };
    const float d1 = side(x, y, t[0].x, t[0].y, t[1].x, t[1].y);
    const float d2 = side(x, y, t[1].x, t[1].y, t[2].x, t[2].y);
    const float d3 = side(x, y, t[2].x, t[2].y, t[0].x, t[0].y);
    const bool negative = d1 < 0 || d2 < 0 || d3 < 0;
    const bool positive = d1 > 0 || d2 > 0 || d3 > 0;
    return !(negative && positive);
}

bool covered(const HudGeometryBatch& batch, float x, float y) {
    for (std::size_t i = 0; i + 2 < batch.triangles.size(); i += 3)
        if (inside_triangle(&batch.triangles[i], x, y)) return true;
    return false;
}

const HudGeometryBatch& batch_for(const HudGeometry& geometry, unsigned shape) {
    for (const auto& batch : geometry.batches)
        if (batch.shape_id == shape) return batch;
    throw std::runtime_error("XP bar batch missing: " + std::to_string(shape));
}

// Fraction of the XP background that is NOT hidden by the shrinking cover, i.e. the visible fill.
float visible_fill(const HudGeometry& geometry) {
    const auto& background = batch_for(geometry, 148);
    const auto& cover = batch_for(geometry, 150);
    float x0 = 1e9f, x1 = -1e9f, y0 = 1e9f, y1 = -1e9f;
    for (const auto& v : background.triangles) {
        x0 = std::min(x0, v.x); x1 = std::max(x1, v.x);
        y0 = std::min(y0, v.y); y1 = std::max(y1, v.y);
    }
    const float step = 0.1f;
    std::size_t total = 0, visible = 0;
    for (float y = y0; y <= y1; y += step)
        for (float x = x0; x <= x1; x += step) {
            if (!covered(background, x, y)) continue;
            ++total;
            if (!covered(cover, x, y)) ++visible;
        }
    check(total > 0, "XP background has no sampled area");
    return float(visible) / float(total);
}

// Geometry of every non-XP batch must be identical between two XP frames.
void check_other_batches_unchanged(const HudGeometry& a, const HudGeometry& b) {
    check(a.batches.size() == b.batches.size(), "XP frame changed batch count");
    for (std::size_t i = 0; i < a.batches.size(); ++i) {
        check(a.batches[i].shape_id == b.batches[i].shape_id, "XP frame reordered batches");
        if (a.batches[i].shape_id == 150) continue;
        check(a.batches[i].triangles.size() == b.batches[i].triangles.size(), "XP frame changed a non-XP batch");
        for (std::size_t j = 0; j < a.batches[i].triangles.size(); ++j) {
            const auto& p = a.batches[i].triangles[j];
            const auto& q = b.batches[i].triangles[j];
            check(p.x == q.x && p.y == q.y && p.u == q.u && p.v == q.v, "XP frame changed non-XP geometry");
        }
    }
}

HudGeometry compose_with_xp(unsigned xp_frame) {
    HudGeometry geometry;
    std::string error;
    check(compose_original_hud(0, 99, 99, xp_frame, 0, geometry, error), "XP compose failed: " + error);
    return geometry;
}
}

int main() {
    try {
        std::string error;

        // Rendered fill follows the frame: 0 = empty, 50 = half, 99 = almost full.
        const HudGeometry empty = compose_with_xp(0);
        const HudGeometry half = compose_with_xp(50);
        const HudGeometry full = compose_with_xp(99);
        const float empty_fill = visible_fill(empty);
        const float half_fill = visible_fill(half);
        const float full_fill = visible_fill(full);
        check(empty_fill <= 0.01f, "Frame 0 must render an empty XP bar");
        check(std::abs(half_fill - 0.5f) <= 0.02f, "Frame 50 must render half of the XP bar");
        check(full_fill >= 0.97f && full_fill <= 1.0f, "Frame 99 must render almost all of the XP bar");
        check_other_batches_unchanged(empty, half);
        check_other_batches_unchanged(empty, full);

        // Every frame 0..99 maps monotonically to the rendered fill within 2 percent.
        float previous = -1.f;
        for (unsigned frame = 0; frame < 100; ++frame) {
            const float fill = visible_fill(compose_with_xp(frame));
            check(fill + 1e-4f >= previous, "XP fill is not monotonic");
            check(std::abs(fill - float(frame) / 100.f) <= 0.02f, "XP fill does not match frame " + std::to_string(frame));
            previous = fill;
        }

        // Live sheet -> frame: plain progress, the 99 cap (no 100% frame), and the post-level-up carry.
        unsigned frame = 0;
        check(original_hud_xp_frame(1490, 1500, frame) && frame == 99, "1490/1500 must be frame 99");
        check(original_hud_xp_frame(1500, 1500, frame) && frame == 99, "Reaching the level cap must stay at frame 99");
        check(original_hud_xp_frame(0, 1500, frame) && frame == 0, "Zero XP must be frame 0");
        check(original_hud_xp_frame(750, 1500, frame) && frame == 50, "750/1500 must be frame 50");
        // Level-up carry: the sheet keeps XP beyond the old threshold (1530 - 1500 = 30) against the new threshold 1600.
        check(original_hud_xp_frame(30, 1600, frame) && frame == 1, "Post-level-up carry must start the new bar at frame 1");
        const float carry_fill = visible_fill(compose_with_xp(frame));
        check(carry_fill >= 0.0f && carry_fill <= 0.03f, "Post-level-up bar must render near empty, not the old fill");
        check(original_hud_xp_frame(0, 2000, frame) && frame == 0, "New level with no carry must reset to frame 0");

        // Edge cases: zero threshold rejects; out-of-domain XP clamps; 32-bit wrap matches the ARM 100*xp multiply.
        check(!original_hud_xp_frame(10, 0, frame), "Zero XP threshold must reject");
        check(original_hud_xp_frame(-5, 1000, frame) && frame == 0, "Negative XP must clamp to frame 0");
        check(original_hud_xp_frame(100000, 1000, frame) && frame == 99, "Overfull XP must cap at frame 99");
        check(original_hud_xp_frame(42949673, 1000, frame) && frame == 0, "100*xp must wrap in 32 bits like the original");

        // Invalid XP frame is transactional: output keeps the previous geometry.
        HudGeometry preserved = compose_with_xp(40);
        const float before = visible_fill(preserved);
        check(!compose_original_hud(0, 99, 99, 100, 0, preserved, error), "XP frame 100 must reject");
        check(std::abs(visible_fill(preserved) - before) < 1e-6f, "Rejected XP frame changed output");

        // Legacy overload keeps its batch set (no XP art), so existing callers see no new batches.
        HudGeometry legacy;
        check(compose_original_hud(0, 99, 99, 0, legacy, error), "Legacy compose failed");
        for (const auto& batch : legacy.batches)
            check(batch.shape_id != 148 && batch.shape_id != 150, "Legacy HUD must not draw the XP bar");

        std::cout << "XP bar rendered fill: 0=" << empty_fill << " 50=" << half_fill << " 99=" << full_fill
                  << "; monotonic frames 0..99, carry, cap, wrap and transactional guards passed\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "HUD XP bar test failure: " << exception.what() << '\n';
        return 1;
    }
}
