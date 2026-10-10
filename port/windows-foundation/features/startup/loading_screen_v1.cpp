#include "loading_screen_v1.hpp"

#include "overlay_renderer.hpp"
#include "platform_sleep.hpp"
#include "platform_win32.hpp"
#include "renderer.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <utility>

namespace dh::foundation::startup {
namespace {

void fill_rect(OverlayRenderer& overlay, float x, float y, float w, float h, std::array<float, 4> color) {
    const std::array<OverlayTriangleVertex, 6> quad{{{x, y, 0, 0}, {x + w, y, 0, 0}, {x + w, y + h, 0, 0},
                                                     {x, y, 0, 0}, {x + w, y + h, 0, 0}, {x, y + h, 0, 0}}};
    overlay.drawTriangles(quad.data(), quad.size(), 0, color);
}

} // namespace

LoadingScreenV1::LoadingScreenV1(Window& window, Renderer& renderer, bool enabled, double min_seconds) noexcept
    : window_(window), renderer_(renderer), enabled_(enabled), minSeconds_(min_seconds), start_(Window::seconds()) {}

void LoadingScreenV1::draw(double fraction) {
    last_ = std::clamp(fraction, 0.0, 1.0);
    window_.poll();
    const int w = window_.width(), h = window_.height();
    renderer_.resize(w, h);
    renderer_.beginFrame(Camera{});
    OverlayRenderer overlay;
    overlay.begin(w, h);
    fill_rect(overlay, 0, 0, float(w), float(h), {0, 0, 0, 1});
    // Bar: centred, 50% of the window width, 14 px high, 2 px frame.
    const float barW = float(w) * 0.5f, barH = 14.0f;
    const float barX = (float(w) - barW) * 0.5f, barY = float(h) * 0.9f;
    fill_rect(overlay, barX - 2, barY - 2, barW + 4, barH + 4, {0.55f, 0.45f, 0.25f, 1});
    fill_rect(overlay, barX, barY, barW, barH, {0.05f, 0.04f, 0.03f, 1});
    fill_rect(overlay, barX, barY, barW * float(last_), barH, {0.9f, 0.65f, 0.2f, 1});
    overlay.end();
    renderer_.endFrame();
    if (capture_ && last_ != lastCaptured_) {
        lastCaptured_ = last_;
        const int percent = int(std::lround(last_ * 100.0));
        char name[32];
        std::snprintf(name, sizeof name, "-%03d.ppm", percent);
        capture_(std::filesystem::path(capturePrefix_.string() + name), w, h);  // host reads the back buffer
    }
    window_.swap();
    platform_sleep_milliseconds(4);
}

void LoadingScreenV1::set_capture(std::filesystem::path prefix, CaptureFn fn) {
    capturePrefix_ = std::move(prefix);
    capture_ = std::move(fn);
}

void LoadingScreenV1::progress(double fraction) {
    if (!enabled_) return;
    draw(fraction);
}

void LoadingScreenV1::finish() {
    if (!enabled_) return;
    draw(1.0);
    while (Window::seconds() - start_ < minSeconds_) {
        draw(1.0);
        if (window_.should_close()) break;
    }
}

} // namespace dh::foundation::startup
