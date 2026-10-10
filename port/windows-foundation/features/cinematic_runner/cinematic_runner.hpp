#pragma once
// P16 CINE / OPENING2: generic cinematic presentation for the campaign script host.
//
// Owns only what the authored script commands ask for and nothing map-specific:
// - caption lines (StartDialog kind 10 / WaitDialog kind 12 / FlushMessages kind 79). The text is
//   the original StrID resolved by the host (MenuLocalization), never a copy made here;
// - the dialog box timing of the original Flash box (see "Dialog box timing" below);
// - the SKIP control state (flash menu_skipcutscene, set by the host) and its hit test;
// - a flat frame description (original panel batches + text slots) in the authored 480x320 space, which
//   main.cpp draws with the PC HUD batch path and the frontend text owner.
//
// The SKIP control and the caption box are the original dqhud_droid art (features/hud_panels, HUDART).
// Caption timing is the decoded original dialog box rule (see "Dialog box timing" below, OPENING2).
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

// Dialog box timing (IDA Script_StartDialog / MenuMessageManager<DialogMsg>, Flash dqhud_droid.swf):
// the box is the DialogBox movie (sprite 727, one frame label per DialogStyles value, isSkipable and btn_next
// set per frame) containing the dialogBox movie (sprite 730: label show = frame 1, hide = frame 19,
// onShowAnimEnd called on frame 12, onHideAnimEnd on frame 39, no Stop in between). Frame rate 30 fps.
//  - show: frame 1 -> 12 (11 frame steps). A skippable style stops here and waits for the tap (btn_next).
//  - tap (btn_next.onRelease, only when isSkipable): hideDialog = gotoAndPlay('hide'), frame 19 -> 39 (20 steps).
//  - a non-skippable style is not stopped at the show end: it plays on to frame 39 by itself (38 steps in all).
//  - onHideAnimEnd: NativeStopMessage('dialog') (or 'quest' for DialogStyles 5 and 19) -> StopDialog pops the
//    line and starts the next one at once.
inline constexpr std::uint32_t kDialogFps = 30;
inline constexpr std::uint32_t kDialogShowFrames = 11;
inline constexpr std::uint32_t kDialogHideFrames = 20;
inline constexpr std::uint32_t kDialogAutoFrames = 38;
std::uint32_t frames_to_ms(std::uint32_t frames) noexcept;
// True when the DialogStyles frame has isSkipable set (the box waits for a tap). Values are the DialogStyles
// constants of dialogs_pycst.bin. Unknown styles wait for a tap (the box stays up until the player taps).
bool dialog_style_waits_for_tap(std::int32_t style) noexcept;

// One DialogMsg as enqueued by Script_StartDialog (scalar 8 = actor, 12 = style, 16 = text id).
struct CaptionLine {
    std::int32_t text_id = -1;
    std::int32_t style = -1;
    std::int32_t actor = -1;
    std::string text;
};

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
    enum class Phase : std::uint8_t {
        Idle,      // nothing shown
        Showing,   // skippable line: show animation (frame 1 -> 12)
        WaitTap,   // skippable line: stopped at the show end until the player taps
        Hiding,    // hide animation after a tap (frame 19 -> 39), then the next line
        AutoRun    // non-skippable line: plays through to its end without input
    };

    // Cutscene contract (host calls these from the Begin/End and flash providers).
    void set_active(bool active);
    bool active() const noexcept { return active_; }
    void set_skip_visible(bool visible) noexcept { skip_visible_ = visible; }
    bool skip_visible() const noexcept { return skip_visible_; }

    // Script commands.
    void enqueue(CaptionLine line);                // StartDialog
    bool waiting() const noexcept;                 // WaitDialog is blocking while a line is queued or shown
    void flush() noexcept;                         // FlushMessages: drops queued and shown lines

    // Player tap (btn_next). Only a skippable line responds; returns true when the tap was used.
    bool tap() noexcept;
    // Advances the timers. Returns the number of lines that finished.
    std::size_t update(std::uint32_t dt_ms) noexcept;

    // Verification input (test harness, --caption-auto-tap-ms): a skippable line that waits at its show end is
    // tapped after this many ms. 0 (default) = wait for the player.
    void set_auto_tap_ms(std::uint32_t ms) noexcept { auto_tap_ms_ = ms; }

    Phase phase() const noexcept { return phase_; }
    const CaptionLine* current() const noexcept;
    std::size_t pending() const noexcept { return queue_.size() + (phase_ != Phase::Idle ? 1u : 0u); }
    std::uint64_t lines_shown() const noexcept { return lines_shown_; }

    // Hit test in the authored space. x/y are window pixels; the window size maps them.
    bool skip_hit(float x, float y, float window_w, float window_h, const SkipLayout& layout = {}) const noexcept;

    // Flat frame for the current state; empty when nothing is visible.
    Frame build_frame() const;

private:
    bool active_ = false;
    bool skip_visible_ = false;
    Phase phase_ = Phase::Idle;
    std::uint32_t left_ms_ = 0;      // time left in the current timed phase
    std::uint32_t wait_ms_ = 0;      // time spent in WaitTap (auto tap)
    std::uint32_t auto_tap_ms_ = 0;
    CaptionLine shown_line_{};
    std::deque<CaptionLine> queue_;
    std::uint64_t lines_shown_ = 0;

    void start_next() noexcept;
    void finish_shown() noexcept;
};

} // namespace dh::foundation::cinematic_runner
