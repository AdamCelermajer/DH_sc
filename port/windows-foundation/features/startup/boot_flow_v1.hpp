#pragma once

// Startup boot flow (Preview 15). Pure state machine: no platform calls.
// The host supplies monotonic seconds and one abstract press/tap edge per frame
// (mouse/touch/Enter/Space/Escape are mapped by the platform adapter, never here).
//
// Original order (IDA GSInit::Update, steps 5-7 and menu_splash): the intro movie
// comes first and already contains the animated Gameloft logo (reference video
// shows it at about 0-4 s), then the splash background with "Touch the screen to
// continue", then the main menu. There is no separate logo stage.
//
// Phases: movie -> title -> complete, or quit at any time.
//   movie : intro.mp4 converted to MPEG-1/MP2 (intro_v1.mpg). A press skips it;
//           movie_finished() ends it. Original: videoDone is set by the player's
//           end/resume (IDA GSInit::Update case 6), not by a timer.
//   title : "Touch the screen to continue" (MENU_TOUCH_TO_CONTINUE). Waits for a press.
// skip_boot starts in complete (tests and --start-mode routes use it).

namespace dh::foundation::startup {

enum class BootPhase : unsigned char { movie, title, complete, quit };

class BootFlowV1 {
public:
    explicit BootFlowV1(bool skip_boot = false) noexcept;

    // now_seconds must be non-decreasing. press is a rising edge this frame.
    void update(double now_seconds, bool press) noexcept;
    // Called by the movie host when the stream ends or fails to decode.
    void movie_finished(double now_seconds) noexcept;
    void request_quit() noexcept;

    BootPhase phase() const noexcept { return phase_; }
    bool complete() const noexcept { return phase_ == BootPhase::complete; }

private:
    void enter(BootPhase next, double now_seconds) noexcept;

    BootPhase phase_ = BootPhase::movie;
    double phase_start_ = 0.0;
    double last_now_ = 0.0;
};

} // namespace dh::foundation::startup
