#pragma once

// Startup boot flow (Preview 15). Pure state machine: no platform calls.
// The host supplies monotonic seconds and one abstract press/tap edge per frame
// (mouse/touch/Enter/Space/Escape are mapped by the platform adapter, never here).
//
// Phases: logo -> movie -> title -> complete, or quit at any time.
//   logo  : gameloft.tga, fades in, holds, fades out. Not skippable.
//   movie : intro.mp4 converted to the portable intro stream. Press skips it;
//           movie_finished() ends it. Original: videoDone is set by the
//           player's end/resume (IDA GSInit::Update case 6), not by a timer.
//   title : "Touch the screen to continue" (MENU_TOUCH_TO_CONTINUE). Waits for
//           a press. The original menu_splash waits for a touch the same way.
// skip_boot starts in complete (tests and --start-mode routes use it).

#include <cstdint>

namespace dh::foundation::startup {

enum class BootPhase : std::uint8_t { logo, movie, title, complete, quit };

struct BootTiming {
    // Implementation choice, not recovered from IDA (no gameloft.tga reference
    // in the recovered code). Documented in STARTUP-report.md.
    double logo_fade_seconds = 0.5;
    double logo_hold_seconds = 2.0;
};

class BootFlowV1 {
public:
    explicit BootFlowV1(BootTiming timing = {}, bool skip_boot = false) noexcept;

    // now_seconds must be non-decreasing. press is a rising edge this frame.
    void update(double now_seconds, bool press) noexcept;
    // Called by the movie host when the stream ends or fails to decode.
    void movie_finished(double now_seconds) noexcept;
    void request_quit() noexcept;

    BootPhase phase() const noexcept { return phase_; }
    bool complete() const noexcept { return phase_ == BootPhase::complete; }
    // Logo opacity in [0,1] for the current time (valid in the logo phase).
    float logo_alpha(double now_seconds) const noexcept;
    double logo_total_seconds() const noexcept {
        return timing_.logo_fade_seconds * 2.0 + timing_.logo_hold_seconds;
    }

private:
    void enter(BootPhase next, double now_seconds) noexcept;

    BootTiming timing_;
    BootPhase phase_ = BootPhase::logo;
    double phase_start_ = 0.0;
    double last_now_ = 0.0;
};

} // namespace dh::foundation::startup
