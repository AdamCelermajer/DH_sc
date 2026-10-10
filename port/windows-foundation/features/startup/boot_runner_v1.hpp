#pragma once

// Runs the Preview 15 boot (logo -> intro movie -> touch to continue) on the
// caller's Window/Renderer. Shared code: the only platform inputs are
// Window::key_down/cursor_position (mapped to one abstract press/tap edge)
// and Window::seconds(). The movie picture comes from the portable intro
// stream (intro_stream_v1.hpp). Soundtrack playback is not wired here yet.

#include "boot_flow_v1.hpp"

#include <filesystem>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace dh::foundation {
class AssetCatalog;
class Renderer;
class Window;
}

namespace dh::foundation::startup {

struct BootRunConfig {
    const AssetCatalog* assets = nullptr;
    // Converted picture stream; if missing or invalid the movie is skipped and
    // the reason is returned in the result (never a silent failure).
    std::filesystem::path intro_stream;
    BootTiming timing{};
    // Window width selects the splash variant as the original GSInit did
    // (854 droid, 800 i9000, otherwise splash_final.tga). Language 0 assumed.
    int window_width = 0;
    // Optional hard stop for tests/captures; 0 = run until continue or close.
    double max_seconds = 0.0;
    // Verification hooks (quiet batches only). Scripted presses go through the
    // same abstract press edge as real input; captures are written by the host.
    std::vector<double> scripted_presses;
    std::vector<std::pair<double, std::filesystem::path>> captures;
    std::function<void(const std::filesystem::path&, int, int)> capture;
};

enum class BootRunOutcome : std::uint8_t { complete, quit, failed };

struct BootRunResult {
    BootRunOutcome outcome = BootRunOutcome::failed;
    std::string error;          // set when outcome is failed or a movie was skipped for a reason
    std::string movie_status;   // "played", "skipped: <reason>", or "skipped: user"
    int movie_frames_shown = 0;
    double seconds = 0.0;
};

BootRunResult run_boot_v1(Window& window, Renderer& renderer, const BootRunConfig& config);

} // namespace dh::foundation::startup
