#include "cinematic_runner.hpp"

#include <algorithm>

namespace dh::foundation::cinematic_runner {

std::uint32_t frames_to_ms(std::uint32_t frames) noexcept {
    return (frames * 1000u + kDialogFps / 2u) / kDialogFps;
}

bool dialog_style_waits_for_tap(std::int32_t style) noexcept {
    // isSkipable is false for the frame labels ScrollingDialogFull (6), TopBubble (7), TopBubbleAvatarLeft (8),
    // TopBubbleAvatarRight (9), Warning (10) and QuestCompletedMsgDialog (19). Every other style is skippable.
    switch (style) {
        case 6: case 7: case 8: case 9: case 10: case 19:
            return false;
        default:
            return true;
    }
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
    if (phase_ == Phase::Idle) start_next();
}

bool CinematicRunner::waiting() const noexcept {
    return phase_ != Phase::Idle || !queue_.empty();
}

void CinematicRunner::flush() noexcept {
    queue_.clear();
    phase_ = Phase::Idle;
    left_ms_ = 0;
    wait_ms_ = 0;
}

bool CinematicRunner::tap() noexcept {
    if (phase_ != Phase::Showing && phase_ != Phase::WaitTap) return false;
    // btn_next.onRelease -> hideDialog(): gotoAndPlay('hide') from wherever the box is.
    phase_ = Phase::Hiding;
    left_ms_ = frames_to_ms(kDialogHideFrames);
    wait_ms_ = 0;
    return true;
}

std::size_t CinematicRunner::update(std::uint32_t dt_ms) noexcept {
    std::size_t finished = 0;
    std::uint32_t dt = dt_ms;
    while (phase_ != Phase::Idle) {
        if (phase_ == Phase::WaitTap) {
            if (auto_tap_ms_ == 0) break;
            const std::uint32_t need = auto_tap_ms_ > wait_ms_ ? auto_tap_ms_ - wait_ms_ : 0u;
            if (dt < need) {
                wait_ms_ += dt;
                break;
            }
            dt -= need;
            tap();
            continue;
        }
        if (dt < left_ms_) {
            left_ms_ -= dt;
            break;
        }
        dt -= left_ms_;
        left_ms_ = 0;
        if (phase_ == Phase::Showing) {
            phase_ = Phase::WaitTap;   // onShowAnimEnd: the skippable box stops and waits for the tap
            wait_ms_ = 0;
        } else {                       // Phase::Hiding or Phase::AutoRun: onHideAnimEnd -> StopDialog
            finish_shown();
            ++finished;
        }
    }
    return finished;
}

const CaptionLine* CinematicRunner::current() const noexcept {
    return phase_ != Phase::Idle ? &shown_line_ : nullptr;
}

void CinematicRunner::start_next() noexcept {
    if (queue_.empty()) {
        phase_ = Phase::Idle;
        return;
    }
    shown_line_ = std::move(queue_.front());
    queue_.pop_front();
    ++lines_shown_;
    wait_ms_ = 0;
    if (dialog_style_waits_for_tap(shown_line_.style)) {
        phase_ = Phase::Showing;
        left_ms_ = frames_to_ms(kDialogShowFrames);
    } else {
        phase_ = Phase::AutoRun;
        left_ms_ = frames_to_ms(kDialogAutoFrames);
    }
}

void CinematicRunner::finish_shown() noexcept {
    left_ms_ = 0;
    wait_ms_ = 0;
    start_next();   // StopDialog pops the line; the next queued line starts at once
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
    // Caption box shows in every phase where the original dialog box is on screen (show, wait for tap, hide, auto).
    if (phase_ != Phase::Idle) {
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
