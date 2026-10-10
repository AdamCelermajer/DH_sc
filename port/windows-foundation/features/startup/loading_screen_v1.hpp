#pragma once

// Campaign loading screen (Preview 15). The host calls progress() after each real load
// stage completes (level, actors, collision) and finish() before the first gameplay frame.
// finish() holds the screen for at least min_seconds. Shared code: Window poll/swap,
// Renderer and OverlayRenderer only; text comes from the authored font via text_label_v1.
//
// Layout follows the reference Part 1 frames at 70-76 s (IDA-free observation, see report):
// dark frame, "LOADING" heading, tip box (string from the original hint table), red progress bar.
// Not recovered yet: the ornate frame/corner art and the avatar badge (loadanims*.swf), so the frame
// is drawn with fills. Disabled instances (tests: --skip-boot, swamp route) are no-ops.

#include "loading_tip_v1.hpp"
#include "text_label_v1.hpp"

#include <filesystem>
#include <functional>
#include <string>

namespace dh::foundation {
class AssetCatalog;
class Renderer;
class Window;
}

namespace dh::foundation::startup {

class LoadingScreenV1 {
public:
    LoadingScreenV1(Window& window, Renderer& renderer, bool enabled, double min_seconds,
                    const AssetCatalog* assets = nullptr) noexcept;
    ~LoadingScreenV1();
    LoadingScreenV1(const LoadingScreenV1&) = delete;
    LoadingScreenV1& operator=(const LoadingScreenV1&) = delete;

    // Tip shown in the tip box (empty = no tip text).
    void set_tip(LoadingTip tip);

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
    void build_labels(int w, int h);

    Window& window_;
    Renderer& renderer_;
    bool enabled_;
    double minSeconds_;
    const AssetCatalog* assets_;
    double start_ = 0.0;
    double last_ = 0.0;
    double lastCaptured_ = -1.0;
    LoadingTip tip_;
    bool hasTip_ = false;
    TextLabel heading_, tipText_;
    bool labelsBuilt_ = false;
    std::filesystem::path capturePrefix_;
    CaptureFn capture_;
};

} // namespace dh::foundation::startup
