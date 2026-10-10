#include "cinematic_runner.hpp"

#include <algorithm>

namespace dh::foundation::cinematic_runner {

std::uint32_t caption_duration_ms_placeholder(const std::string& text) noexcept {
    const std::uint64_t chars = text.size(); // bytes; close enough for a placeholder timer
    const std::uint64_t ms = kCaptionBaseMsPlaceholder + chars * kCaptionPerCharMsPlaceholder;
    return static_cast<std::uint32_t>(std::min<std::uint64_t>(ms, kCaptionMaxMsPlaceholder));
}

Viewport viewport_for(float window_w, float window_h) noexcept {
    Viewport v;
    if (window_h <= 0.f) return v;
    v.scale = window_h / kAuthoredHeight;
    v.offset = (window_w / v.scale - kAuthoredWidth) * 0.5f;
    return v;
}

void CinematicRunner::set_active(bool active) {
    active_ = active;
    if (!active) {
        skip_visible_ = false;
        flush();
    }
}

void CinematicRunner::enqueue(CaptionLine line) {
    queue_.push_back(std::move(line));
    if (!shown_) start_next();
}

bool CinematicRunner::waiting() const noexcept {
    return shown_ || !queue_.empty();
}

void CinematicRunner::flush() noexcept {
    queue_.clear();
    shown_ = false;
    shown_left_ms_ = 0;
}

std::size_t CinematicRunner::update(std::uint32_t dt_ms) noexcept {
    if (!shown_) return 0;
    if (dt_ms < shown_left_ms_) {
        shown_left_ms_ -= dt_ms;
        return 0;
    }
    shown_ = false;
    shown_left_ms_ = 0;
    if (!queue_.empty()) start_next();
    return 1;
}

const CaptionLine* CinematicRunner::current() const noexcept {
    return shown_ ? &shown_line_ : nullptr;
}

void CinematicRunner::start_next() noexcept {
    shown_line_ = std::move(queue_.front());
    queue_.pop_front();
    shown_ = true;
    shown_left_ms_ = caption_duration_ms_placeholder(shown_line_.text);
    ++lines_shown_;
}

bool CinematicRunner::skip_hit(float x, float y, float window_w, float window_h, const SkipLayout& layout) const noexcept {
    if (!active_ || !skip_visible_) return false;
    const Viewport v = viewport_for(window_w, window_h);
    if (v.scale <= 0.f) return false;
    const float ax = x / v.scale - v.offset;
    const float ay = y / v.scale;
    return ax >= layout.x && ax < layout.x + layout.w && ay >= layout.y && ay < layout.y + layout.h;
}

Frame CinematicRunner::build_frame(const SkipLayout& layout) const {
    Frame frame;
    if (!active_) return frame;
    if (skip_visible_) {
        frame.rects.push_back({layout.x, layout.y, layout.w, layout.h, {0.55f, 0.08f, 0.08f, 0.85f}, "placeholder-skip"});
        frame.texts.push_back({kSkipLabelPlaceholder, layout.x, layout.y, layout.w, layout.h, {255, 255, 255, 255}});
    }
    if (shown_) {
        // Caption box: bottom band of the authored screen. Placeholder art (no decoded dialogue box).
        frame.rects.push_back({32.f, 238.f, 416.f, 64.f, {0.f, 0.f, 0.f, 0.72f}, "placeholder-box"});
        frame.texts.push_back({shown_line_.text, 44.f, 246.f, 392.f, 48.f, {255, 255, 255, 255}});
    }
    return frame;
}

} // namespace dh::foundation::cinematic_runner
