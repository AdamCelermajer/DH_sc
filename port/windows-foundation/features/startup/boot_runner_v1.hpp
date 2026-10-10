#pragma once

// Runs the Preview 15 boot (intro movie -> touch to continue) on the caller's
// Window/Renderer. Shared code: the only platform inputs are Window::key_down /
// cursor_position (mapped to one abstract press/tap edge) and Window::seconds().
// The picture comes from intro_movie_v2 (pl_mpeg); the soundtrack plays on the
// shared audio mixer, and the movie clock is the mixer's output frame counter.
// The host passes the mixer and a pump for the platform output (WinMM, SDL2).

#include "boot_flow_v1.hpp"

#include <cstdint>
#include <filesystem>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace dh2::audio {
class AudioMixerV34;
}

namespace dh::foundation {
class AssetCatalog;
class Renderer;
class Window;
}

namespace dh::foundation::startup {

struct BootRunConfig {
    const AssetCatalog* assets = nullptr;
    // Converted intro movie (intro_v1.mpg). Missing or invalid: the movie is skipped and the
    // reason is returned in movie_status (never a silent failure).
    std::filesystem::path intro_movie;
    // Window width selects the splash variant as the original GSInit did
    // (854 droid, 800 i9000, otherwise splash_final.tga). Language 0 assumed.
    int window_width = 0;
    // Optional hard stop for tests/captures; 0 = run until continue or close.
    double max_seconds = 0.0;
    // Soundtrack: mixer + platform pump (called once per loop, may be null). Without a mixer
    // the movie runs on the wall clock and says so in movie_clock.
    dh2::audio::AudioMixerV34* audio_mixer = nullptr;
    std::function<void()> audio_pump;
    std::uint64_t audio_latency_frames = 0;  // frames queued in the platform output (subtracted from the clock)
    // Verification hooks (quiet batches only). Scripted presses go through the same abstract
    // press edge as real input; captures are written by the host.
    // B064: called once when the title screen ("touch to continue") appears. The host starts the
    // frontend TitleMusic here (original menu_splash show -> NativePlayMusic("TitleMusic")). The intro
    // soundtrack voice is stopped first so a skipped movie never overlaps the title track.
    std::function<void()> on_title_entered;
    std::vector<double> scripted_presses;
    std::vector<std::pair<double, std::filesystem::path>> captures;
    std::function<void(const std::filesystem::path&, int, int)> capture;
};

enum class BootRunOutcome : unsigned char { complete, quit, failed };

struct BootRunResult {
    BootRunOutcome outcome = BootRunOutcome::failed;
    std::string error;          // set when outcome is failed or a movie/soundtrack step was skipped for a reason
    std::string movie_status;   // "played", "skipped: <reason>", "skipped: user", "stopped before movie end"
    std::string movie_clock;    // "audio" (mixer output frames) or "wall" (no audio output)
    int movie_frames_shown = 0;
    std::string movie_segments;     // segment table status (B054)
    int movie_presses_ignored = 0;  // presses ignored during an uninterruptible segment (B054)
    double soundtrack_seconds = 0.0;  // audible soundtrack time when the movie ended or was skipped
    bool soundtrack_released = true;  // the mixer released the soundtrack voice before the boot returned
    int handoff_voices = -1;          // B065: mixer voices still active when on_title_entered ran (-1 = no soundtrack)
    bool handoff_released = true;     // B065: the movie voice was released before on_title_entered ran
    double soundtrack_duration = 0.0; // decoded soundtrack length
    double seconds = 0.0;
};

BootRunResult run_boot_v1(Window& window, Renderer& renderer, const BootRunConfig& config);

} // namespace dh::foundation::startup
