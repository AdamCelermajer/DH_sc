#include "loading_screen_v1.hpp"

#include "asset_catalog.hpp"
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

// A rectangle outline of the given thickness (four filled edges).
void frame_rect(OverlayRenderer& overlay, float x, float y, float w, float h, float t, std::array<float, 4> color) {
    fill_rect(overlay, x, y, w, t, color);
    fill_rect(overlay, x, y + h - t, w, t, color);
    fill_rect(overlay, x, y, t, h, color);
    fill_rect(overlay, x + w - t, y, t, h, color);
}

// Tip box geometry, fractions of the window height (box width 56% of the window).
constexpr float kBoxTop = 0.36f;
constexpr float kBoxHeight = 0.16f;

// Reference colours sampled from the Part 1 loading frames (approximate).
constexpr std::array<float, 4> kBackground{0.05f, 0.04f, 0.03f, 1.0f};
constexpr std::array<float, 4> kPanel{0.13f, 0.11f, 0.08f, 1.0f};
constexpr std::array<float, 4> kBorder{0.52f, 0.41f, 0.22f, 1.0f};
constexpr std::array<float, 4> kHeading{0.88f, 0.76f, 0.5f, 1.0f};
constexpr std::array<float, 4> kTipText{0.92f, 0.86f, 0.72f, 1.0f};
constexpr std::array<float, 4> kBarTrack{0.07f, 0.03f, 0.02f, 1.0f};
constexpr std::array<float, 4> kBarFill{0.82f, 0.1f, 0.07f, 1.0f};
constexpr std::array<float, 4> kBarSpark{1.0f, 0.78f, 0.55f, 1.0f};

} // namespace

LoadingScreenV1::LoadingScreenV1(Window& window, Renderer& renderer, bool enabled, double min_seconds,
                                 const AssetCatalog* assets) noexcept
    : window_(window), renderer_(renderer), enabled_(enabled), minSeconds_(min_seconds), assets_(assets),
      start_(Window::seconds()) {}

LoadingScreenV1::~LoadingScreenV1() {
    destroy_text_label(renderer_, heading_);
    destroy_text_label(renderer_, tipText_);
}

void LoadingScreenV1::set_tip(LoadingTip tip) {
    tip_ = std::move(tip);
    hasTip_ = !tip_.text.empty();
}

void LoadingScreenV1::build_labels(int w, int h) {
    if (labelsBuilt_ || !assets_) return;
    labelsBuilt_ = true;
    // Heading: the authored word "LOADING" is the original MenuLoading heading (reference frames).
    build_wrapped_label(renderer_, *assets_, "LOADING", 34, 0.0f, float(h) * 0.13f, float(w), kHeading, heading_);
    if (hasTip_) {
        const float boxW = float(w) * 0.56f;
        const float pad = 22.0f;
        build_wrapped_label(renderer_, *assets_, tip_.text, 18, (float(w) - boxW) * 0.5f + pad,
                            float(h) * kBoxTop, boxW - 2.0f * pad, kTipText, tipText_);
        // Centre the wrapped lines vertically inside the tip box.
        const float shift = (float(h) * kBoxHeight - tipText_.height) * 0.5f;
        for (auto& s : tipText_.sprites) s.y += shift;
    }
}

void LoadingScreenV1::draw(double fraction) {
    last_ = std::clamp(fraction, 0.0, 1.0);
    window_.poll();
    const int w = window_.width(), h = window_.height();
    renderer_.resize(w, h);
    build_labels(w, h);
    renderer_.beginFrame(Camera{});
    OverlayRenderer overlay;
    overlay.begin(w, h);
    const float fw = float(w), fh = float(h);
    fill_rect(overlay, 0, 0, fw, fh, kBackground);
    frame_rect(overlay, fw * 0.02f, fh * 0.03f, fw * 0.96f, fh * 0.94f, 3.0f, kBorder);
    for (const auto& glyph : heading_.sprites) overlay.drawSprite(glyph);
    // Tip box: framed panel holding the wrapped tip text.
    const float boxX = fw * 0.22f, boxY = fh * kBoxTop, boxW = fw * 0.56f, boxH = fh * kBoxHeight;
    fill_rect(overlay, boxX, boxY, boxW, boxH, kPanel);
    frame_rect(overlay, boxX, boxY, boxW, boxH, 2.0f, kBorder);
    for (const auto& glyph : tipText_.sprites) overlay.drawSprite(glyph);
    // Progress bar: red fill along the real load stages, bright spark at the front.
    const float barW = fw * 0.76f, barH = 14.0f;
    const float barX = (fw - barW) * 0.5f, barY = fh * 0.86f;
    fill_rect(overlay, barX - 2, barY - 2, barW + 4, barH + 4, kBorder);
    fill_rect(overlay, barX, barY, barW, barH, kBarTrack);
    const float fillW = barW * float(last_);
    if (fillW > 0.0f) fill_rect(overlay, barX, barY, fillW, barH, kBarFill);
    if (fillW > 0.0f && last_ < 1.0) fill_rect(overlay, barX + fillW - 3.0f, barY - 4.0f, 4.0f, barH + 8.0f, kBarSpark);
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
