#include "loading_screen_v1.hpp"

#include "asset_catalog.hpp"
#include "content_paths.hpp"
#include "overlay_renderer.hpp"
#include "platform_sleep.hpp"
#include "platform_win32.hpp"
#include "renderer.hpp"
#include "texture_loader.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <exception>
#include <utility>
#include <vector>

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

// Fallback (textures missing) layout, fractions of the window height.
constexpr float kBoxTop = 0.36f;
constexpr float kBoxHeight = 0.16f;

constexpr std::array<float, 4> kBackground{0.05f, 0.04f, 0.03f, 1.0f};
constexpr std::array<float, 4> kPanel{0.13f, 0.11f, 0.08f, 1.0f};
constexpr std::array<float, 4> kBorder{0.52f, 0.41f, 0.22f, 1.0f};
constexpr std::array<float, 4> kHeading{0.88f, 0.76f, 0.5f, 1.0f};
constexpr std::array<float, 4> kTipText{0.92f, 0.86f, 0.72f, 1.0f};
constexpr std::array<float, 4> kBarTrack{0.07f, 0.03f, 0.02f, 1.0f};
constexpr std::array<float, 4> kBarFill{0.82f, 0.1f, 0.07f, 1.0f};
constexpr std::array<float, 4> kBarSpark{1.0f, 0.78f, 0.55f, 1.0f};

// Stage placement measured on the reference video (Part 1, 70-76 s): the 1024x768 stage is stretched to the
// window and then scaled up ~12% about its centre (edges cropped). Fitted to landmark positions
// (bar ends, heading, tip corners); the original scaling code is not recovered.
constexpr float kZoom = 1.12f;
struct StagePlacement { float sx, sy, ox, oy; };
StagePlacement stage_placement(int w, int h, const LoadingArt& art) {
    const float sx = float(w) / art.stageW * kZoom, sy = float(h) / art.stageH * kZoom;
    return {sx, sy, (float(w) - art.stageW * sx) * 0.5f, (float(h) - art.stageH * sy) * 0.5f};
}

// Sutherland-Hodgman clip of a triangle against an axis-aligned rectangle (u,v interpolated).
void clip_triangle(const ArtVertex* t, float x0, float x1, float y0, float y1, std::vector<ArtVertex>& out) {
    std::vector<ArtVertex> poly(t, t + 3), next;
    auto pass = [&](int axis, float edge, bool keepGreater) {
        next.clear();
        auto value = [&](const ArtVertex& v) { return axis == 0 ? v.x : v.y; };
        auto inside = [&](const ArtVertex& v) { return keepGreater ? value(v) >= edge : value(v) <= edge; };
        for (size_t i = 0; i < poly.size(); ++i) {
            const ArtVertex& a = poly[i];
            const ArtVertex& b = poly[(i + 1) % poly.size()];
            const bool ia = inside(a), ib = inside(b);
            if (ia) next.push_back(a);
            if (ia != ib) {
                const float k = (edge - value(a)) / (value(b) - value(a));
                next.push_back({a.x + (b.x - a.x) * k, a.y + (b.y - a.y) * k, a.u + (b.u - a.u) * k,
                                a.v + (b.v - a.v) * k});
            }
        }
        poly.swap(next);
    };
    pass(0, x0, true);
    pass(0, x1, false);
    pass(1, y0, true);
    pass(1, y1, false);
    for (size_t i = 1; i + 1 < poly.size(); ++i) {
        out.push_back(poly[0]);
        out.push_back(poly[i]);
        out.push_back(poly[i + 1]);
    }
}

} // namespace

LoadingScreenV1::LoadingScreenV1(Window& window, Renderer& renderer, bool enabled, double min_seconds,
                                 const AssetCatalog* assets) noexcept
    : window_(window), renderer_(renderer), enabled_(enabled), minSeconds_(min_seconds), assets_(assets),
      start_(Window::seconds()) {}

LoadingScreenV1::~LoadingScreenV1() {
    destroy_text_label(renderer_, heading_);
    destroy_text_label(renderer_, tipText_);
    for (auto t : artTex_)
        if (t) renderer_.destroyTexture(t);
}

void LoadingScreenV1::set_tip(LoadingTip tip) {
    tip_ = std::move(tip);
    hasTip_ = !tip_.text.empty();
}

void LoadingScreenV1::build_art() {
    if (artBuilt_ || !assets_) return;
    artBuilt_ = true;
    static const char* const files[] = {"", "MenuGraphics01.tga", "MenuGraphics02.tga", "MenuGraphics03.tga",
                                        "MenuGraphics04.tga", "MenuGraphics05.tga", "MenusGraphics.tga"};
    bool ok = true;
    for (int i = 1; i <= 6; ++i) {
        try {
            TextureImage image;
            std::string error;
            const auto path = resolve_content_path(*assets_, std::string("data/3d/textures/") + files[i]);
            if (!load_texture(path, image, error)) {
                ok = false;
                continue;
            }
            artTex_[i] = renderer_.createTexture(int(image.width), int(image.height), image.rgba.data());
        } catch (const std::exception&) {
            ok = false;
        }
    }
    artOk_ = ok;
}

void LoadingScreenV1::draw_art(int w, int h, double fraction, OverlayRenderer& overlay) {
    const LoadingArt& art = loading_art();
    const StagePlacement sp = stage_placement(w, h, art);
    std::vector<OverlayTriangleVertex> tri;
    auto emit = [&](const LayerSpan& span, const float* m, float alpha) {
        for (int i = 0; i < span.count; ++i) {
            const Layer& layer = span.layers[i];
            tri.clear();
            for (const auto& v : layer.verts) {
                const float x = m ? m[0] * v.x + m[2] * v.y + m[4] : v.x;
                const float y = m ? m[1] * v.x + m[3] * v.y + m[5] : v.y;
                tri.push_back({sp.ox + x * sp.sx, sp.oy + y * sp.sy, v.u, v.v});
            }
            std::array<float, 4> color = layer.color;
            color[3] *= alpha;
            overlay.drawTriangles(tri, layer.texture ? artTex_[layer.texture] : 0, color);
        }
    };
    emit(art.bg, nullptr, 1.0f);
    emit(art.frame, nullptr, 1.0f);
    emit(art.track, nullptr, 1.0f);
    const int frame = std::clamp(int(std::lround(fraction * 100.0)), 0, 100);
    const FrameInfo& f = art.frames[frame];
    // Progress fill: the original masks the red bar with a rectangle that grows with the frame.
    for (int i = 0; i < art.fill.count; ++i) {
        const Layer& layer = art.fill.layers[i];
        std::vector<ArtVertex> clipped;
        for (size_t k = 0; k + 2 < layer.verts.size(); k += 3)
            clip_triangle(&layer.verts[k], f.maskX0, f.maskX1, f.maskY0, f.maskY1, clipped);
        tri.clear();
        for (const auto& v : clipped) tri.push_back({sp.ox + v.x * sp.sx, sp.oy + v.y * sp.sy, v.u, v.v});
        if (!tri.empty()) overlay.drawTriangles(tri, layer.texture ? artTex_[layer.texture] : 0, layer.color);
    }
    if (f.sparkAlpha > 0.0f) emit(art.spark, f.spark, f.sparkAlpha);
}

void LoadingScreenV1::build_labels(int w, int h) {
    if (labelsBuilt_ || !assets_) return;
    labelsBuilt_ = true;
    const LoadingArt& art = loading_art();
    const StagePlacement sp = stage_placement(w, h, art);
    const auto rgba = [](const TextRect& r) {
        return std::array<float, 4>{r.rgba[0] / 255.0f, r.rgba[1] / 255.0f, r.rgba[2] / 255.0f, r.rgba[3] / 255.0f};
    };
    // Heading: the authored word "LOADING" is the original MenuLoading heading (reference frames).
    const TextRect& hr = art.heading;
    build_wrapped_label(renderer_, *assets_, "LOADING", int(std::lround(hr.size * sp.sy)), sp.ox + hr.x * sp.sx,
                        sp.oy + hr.y * sp.sy, hr.w * sp.sx, rgba(hr), heading_);
    if (hasTip_) {
        const TextRect& tr = art.tip;
        build_wrapped_label(renderer_, *assets_, tip_.text, int(std::lround(tr.size * sp.sy)), sp.ox + tr.x * sp.sx,
                            sp.oy + tr.y * sp.sy, tr.w * sp.sx, rgba(tr), tipText_);
    }
}

// Plain fill layout used only when the original menu textures are unavailable.
void LoadingScreenV1::draw_fallback(OverlayRenderer& overlay, float fw, float fh) {
    frame_rect(overlay, fw * 0.02f, fh * 0.03f, fw * 0.96f, fh * 0.94f, 3.0f, kBorder);
    for (const auto& glyph : heading_.sprites) overlay.drawSprite(glyph);
    const float boxX = fw * 0.22f, boxY = fh * kBoxTop, boxW = fw * 0.56f, boxH = fh * kBoxHeight;
    fill_rect(overlay, boxX, boxY, boxW, boxH, kPanel);
    frame_rect(overlay, boxX, boxY, boxW, boxH, 2.0f, kBorder);
    for (const auto& glyph : tipText_.sprites) overlay.drawSprite(glyph);
    const float barW = fw * 0.76f, barH = 14.0f;
    const float barX = (fw - barW) * 0.5f, barY = fh * 0.86f;
    fill_rect(overlay, barX - 2, barY - 2, barW + 4, barH + 4, kBorder);
    fill_rect(overlay, barX, barY, barW, barH, kBarTrack);
    const float fillW = barW * float(last_);
    if (fillW > 0.0f) fill_rect(overlay, barX, barY, fillW, barH, kBarFill);
    if (fillW > 0.0f && last_ < 1.0) fill_rect(overlay, barX + fillW - 3.0f, barY - 4.0f, 4.0f, barH + 8.0f, kBarSpark);
}

void LoadingScreenV1::draw(double fraction) {
    last_ = std::clamp(fraction, 0.0, 1.0);
    window_.poll();
    const int w = window_.width(), h = window_.height();
    renderer_.resize(w, h);
    build_labels(w, h);
    build_art();
    renderer_.beginFrame(Camera{});
    OverlayRenderer overlay;
    overlay.begin(w, h);
    const float fw = float(w), fh = float(h);
    fill_rect(overlay, 0, 0, fw, fh, kBackground);
    if (artOk_) {
        draw_art(w, h, last_, overlay);
        for (const auto& glyph : heading_.sprites) overlay.drawSprite(glyph);
        for (const auto& glyph : tipText_.sprites) overlay.drawSprite(glyph);
    } else {
        draw_fallback(overlay, fw, fh);
    }
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
