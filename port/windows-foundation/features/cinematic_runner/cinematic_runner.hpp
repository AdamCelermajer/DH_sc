#pragma once
// P16 CINE: generic cinematic presentation for the campaign script host.
//
// Owns only what the authored script commands ask for and nothing map-specific:
// - caption lines (StartDialog kind 10 / WaitDialog kind 12 / FlushMessages kind 79). The text is
//   the original StrID resolved by the host (MenuLocalization), never a copy made here;
// - the SKIP control state (flash menu_skipcutscene, set by the host) and its hit test;
// - a flat frame description (solid rects + text items) in the authored 480x320 space, which main.cpp
//   draws with the existing overlay and frontend text owners.
//
// Art and timing that the cache does not provide are PLACEHOLDERS and are labelled as such in the
// code and in the Preview 16 CINE report (Placeholders section). Nothing here reads a platform API.
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
// box, so each line is held for a fixed base plus a per-character term. Fitted by eye to the reference
// video (Part 1, v1.0.3; see the CINE report, Placeholders): "Is he... already dead?" ~2 s,
// "He's dead alright... But that runs in the family..." ~4 s, "Oh! It's a miracle!..." ~8 s.
inline constexpr std::uint32_t kCaptionBaseMsPlaceholder = 2500;
inline constexpr std::uint32_t kCaptionPerCharMsPlaceholder = 60;
inline constexpr std::uint32_t kCaptionMaxMsPlaceholder = 9000;
std::uint32_t caption_duration_ms_placeholder(const std::string& text) noexcept;

struct SolidRect {
    float x = 0, y = 0, w = 0, h = 0;
    std::array<float, 4> rgba{0, 0, 0, 1};
    const char* role = "";   // "placeholder-box", "placeholder-skip", or "caption-box"
};

struct TextItem {
    std::string text;
    float x = 0, y = 0, w = 0, h = 0;   // authored space
    std::array<std::uint8_t, 4> rgba{255, 255, 255, 255};
};

struct Frame {
    std::vector<SolidRect> rects;
    std::vector<TextItem> texts;
};

// Same mapping as the PC HUD: authored space scaled by window height, centred horizontally.
// window = (authored + offset) * scale.
struct Viewport { float scale = 1.f, offset = 0.f; };
Viewport viewport_for(float window_w, float window_h) noexcept;

// Placeholder SKIP control: the original art lives in dqhud.swf (frame label menu_skipcutscene) and is
// not decoded by the port yet. Position is the reference's top-left SKIP bar (CINE survey B5/B6).
struct SkipLayout {
    float x = 8, y = 8, w = 72, h = 20;
};
inline constexpr const char* kSkipLabelPlaceholder = "SKIP";

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
    Frame build_frame(const SkipLayout& layout = {}) const;

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
