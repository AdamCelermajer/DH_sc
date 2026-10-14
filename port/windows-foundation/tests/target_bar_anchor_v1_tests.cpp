// w5-mud2: the enemy target bar anchor is always available while the target is retained (a head outside the viewport keeps its
// projected position and the bar is clamped into the window; a head behind the eye uses the top centre).
#include "../features/hud_panels/target_bar_anchor_v1.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
using namespace dh::foundation;
static void check(bool ok, const std::string& what) { if (!ok) throw std::runtime_error("FAILED: " + what); }
int main() {
    const int w = 1200, h = 720;
    const std::array<float, 4> bounds{0.f, 170.f, 0.f, 45.f}; // xmin,xmax,ymin,ymax target-local pixels (bar art)
    const float scale = 1.0f;
    // On screen: anchor is the projected head, the bar sits with its bottom centre on it.
    { const float clip[4]{0.f, 0.f, 0.5f, 1.f}; // NDC (0,0): window centre
      const auto a = target_bar_anchor_v1(clip, w, h);
      check(a.on_screen && std::fabs(a.screen[0] - 600.f) < 1e-3f && std::fabs(a.screen[1] - 360.f) < 1e-3f, "centre head projects to the window centre");
      const auto o = target_bar_origin_v1(a, bounds, scale, w, h);
      check(std::fabs(o[0] - (600.f - 85.f)) < 1e-3f && std::fabs(o[1] - (360.f - 45.f)) < 1e-3f, "bar bottom-centre on the head"); }
    // Rearing Mud Lord: the head is above the top edge (NDC y = 1.6). Old code: no bar. Now: anchor kept, bar clamped to the top edge.
    { const float clip[4]{0.2f, 1.6f, 0.5f, 1.f};
      const auto a = target_bar_anchor_v1(clip, w, h);
      check(!a.on_screen, "head above the viewport is reported off screen");
      check(a.screen[1] < 0.f, "off-screen head keeps its projected position (above the top edge)");
      const auto o = target_bar_origin_v1(a, bounds, scale, w, h);
      check(o[1] >= 0.f && o[1] + 45.f <= float(h) && o[0] >= 0.f && o[0] + 170.f <= float(w), "bar clamped fully inside the window");
      check(std::fabs(o[1]) < 1e-3f, "bar rests on the top edge"); }
    // Head far left/right/below: clamped on that edge.
    { const float clip[4]{-9.f, -9.f, 0.5f, 1.f};
      const auto a = target_bar_anchor_v1(clip, w, h);
      const auto o = target_bar_origin_v1(a, bounds, scale, w, h);
      check(!a.on_screen && std::fabs(o[0]) < 1e-3f && std::fabs(o[1] - (float(h) - 45.f)) < 1e-3f, "bottom-left head clamps to the bottom-left corner"); }
    // Behind the eye (w <= 0) or non-finite: no projection, top centre, still inside the window.
    for (const float bad_w : {0.f, -3.f}) {
      const float clip[4]{0.f, 0.f, 0.f, bad_w};
      const auto a = target_bar_anchor_v1(clip, w, h);
      const auto o = target_bar_origin_v1(a, bounds, scale, w, h);
      check(!a.on_screen && std::fabs(a.screen[0] - 600.f) < 1e-3f && std::fabs(a.screen[1]) < 1e-3f, "behind-eye head anchors at the top centre");
      check(o[0] >= 0.f && o[1] >= 0.f && o[0] + 170.f <= float(w), "behind-eye bar inside the window"); }
    { const float clip[4]{NAN, 0.f, 0.f, 1.f};
      const auto a = target_bar_anchor_v1(clip, w, h);
      check(!a.on_screen && std::fabs(a.screen[1]) < 1e-3f, "non-finite clip falls back to the top centre"); }
    // Window smaller than the bar: low > high keeps the low bound (no crash, deterministic).
    { const float clip[4]{0.f, 0.f, 0.5f, 1.f};
      const auto o = target_bar_origin_v1(target_bar_anchor_v1(clip, 100, 30), bounds, scale, 100, 30);
      check(std::isfinite(o[0]) && std::isfinite(o[1]), "tiny window stays finite"); }
    std::cout << "target_bar_anchor_v1: on-screen, above-viewport, corner, behind-eye, non-finite and tiny-window cases pass\n";
    return 0;
}
