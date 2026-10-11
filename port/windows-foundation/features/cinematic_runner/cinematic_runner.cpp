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
        case 3: // B068: EnterLocationDialog title ("The Boglands - Ancient Prison") leaves on its own in the reference video (t~76-79, nobody taps); the
                // Flash isSkipable bit of this frame was not read, so this is video evidence only.
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
    if (phase_ == Phase::Hiding) {   // B068 PC adaptation: finish the hide animation now (the original ignores this tap)
        left_ms_ = 0;
        return true;
    }
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

SkipLayout skip_art_layout() noexcept {
    SkipLayout out;
    bool first = true;
    float x0 = 0.f, y0 = 0.f, x1 = 0.f, y1 = 0.f;
    for (const auto& batch : hud_panels::skip_batches_v1()) {
        for (const auto& v : batch.triangles) {
            if (first || v.x < x0) x0 = v.x;
            if (first || v.x > x1) x1 = v.x;
            if (first || v.y < y0) y0 = v.y;
            if (first || v.y > y1) y1 = v.y;
            first = false;
        }
    }
    out.x = x0; out.y = y0; out.w = x1 - x0; out.h = y1 - y0;
    return out;
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
    // B068: the dialog box is independent of the cutscene mode (the EnterLocation title is queued one frame before the opening cutscene starts and
    // is on screen during its first seconds, video t~76-79); only the SKIP control belongs to the cutscene.
    if (!active_ && phase_ == Phase::Idle) return frame;
    if (skip_visible_ && active_) {
        frame.panels.push_back(&hud_panels::skip_batches_v1());
        for (const auto& slot : hud_panels::skip_texts_v1())
            frame.texts.push_back(text_from_slot(slot, hud_panels::kSkipLabelV1));
    }
    // Caption box shows in every phase where the original dialog box is on screen (show, wait for tap, hide, auto).
    if (phase_ != Phase::Idle) {
        // Caption box: the DialogBox variant of the line's DialogStyles name (name plate + text band, B068). A style without
        // exported art falls back to the ChestTuto variant that earlier builds used for every line.
        const hud_panels::DialogVariantV1* variant = nullptr;
        for (const auto& v : hud_panels::dialog_variants_v1())
            if (shown_line_.style_label == v.label) variant = &v; // B068: a variant without art (EnterLocationDialog) is text only, as in the video
        const std::vector<hud_panels::PcGameplayHudArtBatchV1>* batches = variant ? variant->batches : &hud_panels::caption_batches_v1();
        const std::vector<hud_panels::HudPanelTextSlotV1>* slots = variant ? variant->texts : &hud_panels::caption_texts_v1();
        if (!batches->empty()) frame.panels.push_back(batches);
        for (const auto& slot : *slots) {
            if (std::string(slot.container) == "TextBox")
                frame.texts.push_back(text_from_slot(slot, shown_line_.text));
            else if (std::string(slot.container) == "NameBox" && !shown_line_.speaker.empty())
                frame.texts.push_back(text_from_slot(slot, shown_line_.speaker));
        }
    }
    return frame;
}

} // namespace dh::foundation::cinematic_runner
