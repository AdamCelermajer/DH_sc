#pragma once
// P16 CINE: generic cinematic presentation for the campaign script host.
//
// Owns only what the authored script commands ask for and nothing map-specific:
// - caption lines (StartDialog kind 10 / WaitDialog kind 12 / FlushMessages kind 79). The text is
//   the original StrID resolved by the host (MenuLocalization), never a copy made here;
// - the SKIP control state (flash menu_skipcutscene, set by the host) and its hit test;
// - a flat frame description (original panel batches + text slots) in the authored 480x320 space, which
//   main.cpp draws with the PC HUD batch path and the frontend text owner.
//
// The SKIP control and the caption box are the original dqhud_droid art (features/hud_panels, HUDART).
// Caption timing is still a PLACEHOLDER (the Flash dialogue advance is not decoded).
#include "../hud_panels/hud_panels_art_v1.hpp"

#include <array>
#include <cstdint>
#include <cstddef>
#include <deque>
#include <string>
#include <vector>

namespace dh::foundation::cinematic_runner {

// Authored (virtual) space of the original HUD and menus.
inline constexpr float kAuthoredWidth = 480.f;
inline constexpr float kAuthoredHeight = 320.f;

// One DialogMsg as enqueued by Script_StartDialog (scalar 8 = actor, 12 = style, 16 = text id).
struct CaptionLine {
    std::int32_t text_id = -1;
    std::int32_t style = -1;
    std::int32_t actor = -1;
    std::string text;
};

// PLACEHOLDER timing. The original advances the dialogue inside its Flash box; the port has no decoded
// advance rule, so each line is held for a fixed base plus a per-character term. Fitted by eye to the reference
// video (Part 1, v1.0.3; see the CINE report, Placeholders): "Is he... already dead?" ~2 s (104-106 s),
// "He's dead alright..." ~4 s (108-112 s); chest tutorial lines ~2-4 s each (192-204 s).
inline constexpr std::uint32_t kCaptionBaseMsPlaceholder = 2000;
inline constexpr std::uint32_t kCaptionPerCharMsPlaceholder = 25;
inline constexpr std::uint32_t kCaptionMaxMsPlaceholder = 6000;
std::uint32_t caption_duration_ms_placeholder(const std::string& text) noexcept;

struct TextItem {
    std::string text;
    float x = 0, y = 0, w = 0, h = 0;   // authored space
    std::array<std::uint8_t, 4> rgba{255, 255, 255, 255};
    float height = 14.f;                // source glyph height (px)
    int align = 0;                      // 0 left, 2 centre (EditText layout)
};

struct Frame {
    // Original stage-space art sets (hud_panels), drawn in order with the atlas bitmap.
    std::vector<const std::vector<hud_panels::PcGameplayHudArtBatchV1>*> panels;
    std::vector<TextItem> texts;
};

// Same mapping as the PC HUD: authored space scaled by window height, centred horizontally.
// window = (authored + offset) * scale.
struct Viewport { float scale = 1.f, offset = 0.f; };
Viewport viewport_for(float window_w, float window_h) noexcept;

// SKIP hit area in the authored space: the exported SKIP art bounds (X icon and label, hud_panels).
struct SkipLayout {
    float x = 3.4f, y = 0.f, w = 193.f, h = 54.5f;
};

class CinematicRunner {
public:
    // Cutscene contract (host calls these from the Begin/End and flash providers).
    void set_active(bool active);
    bool active() const noexcept { return active_; }
    void set_skip_visible(bool visible) noexcept { skip_visible_ = visible; }
    bool skip_visible() const noexcept { return skip_visible_; }

    // Script commands.
    void enqueue(CaptionLine line);                // StartDialog
    bool waiting() const noexcept;                 // WaitDialog is blocking while a line is queued or shown
    void flush() noexcept;                         // FlushMessages: drops queued and shown lines
    // Advances the shown line's timer. Returns the number of lines that finished.
    std::size_t update(std::uint32_t dt_ms) noexcept;

    const CaptionLine* current() const noexcept;
    std::size_t pending() const noexcept { return queue_.size() + (shown_ ? 1u : 0u); }
    std::uint64_t lines_shown() const noexcept { return lines_shown_; }

    // Hit test in the authored space. x/y are window pixels; the window size maps them.
    bool skip_hit(float x, float y, float window_w, float window_h, const SkipLayout& layout = {}) const noexcept;

    // Flat frame for the current state; empty when nothing is visible.
    Frame build_frame() const;

private:
    bool active_ = false;
    bool skip_visible_ = false;
    bool shown_ = false;
    std::uint32_t shown_left_ms_ = 0;
    CaptionLine shown_line_{};
    std::deque<CaptionLine> queue_;
    std::uint64_t lines_shown_ = 0;

    void start_next() noexcept;
};

} // namespace dh::foundation::cinematic_runner
