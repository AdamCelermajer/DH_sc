#pragma once

// Campaign loading screen (Preview 15). The host calls progress() after each
// real load stage completes (level, actors, collision) and finish() before the
// first gameplay frame. finish() holds the screen for at least min_seconds.
// Shared code: Window poll/swap and Renderer/OverlayRenderer only.
//
// Not recovered: the original MenuLoading tip strings and progress art
// (loadanims*.swf) are not rendered yet; this draws a bar only. Disabled
// instances (tests: --skip-boot, swamp route) are no-ops.

#include <filesystem>
#include <functional>
#include <string>

namespace dh::foundation {
class Renderer;
class Window;
}

namespace dh::foundation::startup {

class LoadingScreenV1 {
public:
    LoadingScreenV1(Window& window, Renderer& renderer, bool enabled, double min_seconds) noexcept;

    // Draws the screen at fraction in [0,1] (clamped) after a completed stage.
    void progress(double fraction);
    // Draws 100% and keeps the screen up until min_seconds have elapsed since construction.
    void finish();

    // Verification hook: writes <prefix>-<NNN>.ppm (NNN = percent) once per distinct stage fraction.
    using CaptureFn = std::function<void(const std::filesystem::path&, int, int)>;
    void set_capture(std::filesystem::path prefix, CaptureFn fn);

    bool enabled() const noexcept { return enabled_; }
    double last_fraction() const noexcept { return last_; }

private:
    void draw(double fraction);

    Window& window_;
    Renderer& renderer_;
    bool enabled_;
    double minSeconds_;
    double start_ = 0.0;
    double last_ = 0.0;
    double lastCaptured_ = -1.0;
    std::filesystem::path capturePrefix_;
    CaptureFn capture_;
};

} // namespace dh::foundation::startup
