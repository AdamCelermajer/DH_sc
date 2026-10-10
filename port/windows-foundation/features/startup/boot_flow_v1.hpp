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

// B053: splash_final*.tga is a 2048x1024 atlas. The splash picture is its top-left 1280x752
// (pixel scan of the cache texture: art ends at x=1280, y=752); everything below/right is
// condensed UI art (rings, arrows, power icons) that belongs to other menus and must not be drawn.
// The original stretched the picture over the whole screen (reference video 0:38-0:45 shows it
// filling the frame at 16:9), so the layout is the largest 16:9 rectangle centred in the window.
struct SplashLayout {
    float x = 0, y = 0, width = 0, height = 0;  // window pixels
    float u0 = 0, v0 = 0, u1 = 1, v1 = 1;       // atlas uv
};

inline SplashLayout splash_layout(int windowW, int windowH, int atlasW, int atlasH) noexcept {
    SplashLayout l;
    constexpr float kRegionW = 1280.0f / 2048.0f, kRegionH = 752.0f / 1024.0f;
    // Half-texel inset so bilinear sampling never bleeds the atlas neighbours into the border.
    l.u1 = kRegionW - 0.5f / float(atlasW > 0 ? atlasW : 1);
    l.v1 = kRegionH - 0.5f / float(atlasH > 0 ? atlasH : 1);
    l.u0 = 0.5f / float(atlasW > 0 ? atlasW : 1);
    l.v0 = 0.5f / float(atlasH > 0 ? atlasH : 1);
    const float aspect = 16.0f / 9.0f;
    float w = float(windowW), h = float(windowH);
    if (w > h * aspect) w = h * aspect; else h = w / aspect;
    l.width = w;
    l.height = h;
    l.x = (float(windowW) - w) * 0.5f;
    l.y = (float(windowH) - h) * 0.5f;
    return l;
}

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
