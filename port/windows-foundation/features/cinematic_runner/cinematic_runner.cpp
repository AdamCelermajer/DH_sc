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

namespace {
// Text slot of an original panel (hud_panels). Slots carry the authored box and format.
TextItem text_from_slot(const hud_panels::HudPanelTextSlotV1& slot, std::string text) {
    TextItem item;
    item.text = std::move(text);
    item.x = slot.rect[0];
    item.y = slot.rect[1];
    item.w = slot.rect[2] - slot.rect[0];
    item.h = slot.rect[3] - slot.rect[1];
    item.rgba = slot.rgba;
    item.height = slot.height;
    item.align = slot.align;
    return item;
}
} // namespace

Frame CinematicRunner::build_frame() const {
    Frame frame;
    if (!active_) return frame;
    if (skip_visible_) {
        frame.panels.push_back(&hud_panels::skip_batches_v1());
        for (const auto& slot : hud_panels::skip_texts_v1())
            frame.texts.push_back(text_from_slot(slot, hud_panels::kSkipLabelV1));
    }
    if (shown_) {
        // Caption box: the original dialog frame (text band and name plate). The text band holds the line; the
        // name plate needs the speaker name, which the host does not resolve yet (the plate is drawn without text).
        frame.panels.push_back(&hud_panels::caption_batches_v1());
        for (const auto& slot : hud_panels::caption_texts_v1()) {
            if (std::string(slot.container) == "TextBox")
                frame.texts.push_back(text_from_slot(slot, shown_line_.text));
        }
    }
    return frame;
}

} // namespace dh::foundation::cinematic_runner
