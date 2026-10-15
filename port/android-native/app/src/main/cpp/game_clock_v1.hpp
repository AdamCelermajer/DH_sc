#pragma once
// The single game clock. Every gameplay-path time read (frame dt, animation
// sampling, timers, script waits, FX/particle time, level seeds) goes through
// here so one switch can put the whole runtime on a fixed timestep.
//
// Real time (default, Android and interactive desktop): the wall clock, exactly
// as before. Fixed step (desktop headless --frames runs only): time advances by
// exactly step_ms per rendered frame via advance_frame(), so repeated scenario
// runs see identical timing regardless of host speed.
#include <chrono>
#include <cstdint>

#if defined(_WIN32) && !defined(__MINGW32__)
#error "game_clock_v1 expects a POSIX-style toolchain (Android NDK or MinGW)"
#endif
#include <sys/time.h>

namespace dh2::game_clock {

struct State {
    bool fixed = false;
    std::uint64_t step_ms = 25;
    std::uint64_t frames = 0;
};

inline State& state() noexcept {
    static State value;
    return value;
}

// Wall-clock epoch used for the fixed timeline, so seeds derived from original
// clock reads are stable and look like a plausible Unix time.
inline constexpr std::uint64_t kFixedWallEpochMs = 1700000000000ull;

// Called once before the first frame of a headless --frames run.
inline void enable_fixed_step(std::uint64_t step_ms) noexcept {
    auto& s = state();
    s.fixed = true;
    s.step_ms = step_ms ? step_ms : 1;
    s.frames = 0;
}

inline bool fixed_step() noexcept { return state().fixed; }

// Called exactly once per rendered frame by the frame loop (fixed mode only).
inline void advance_frame() noexcept {
    if (state().fixed) ++state().frames;
}

// Game-timeline milliseconds since process start (real) or since frame 0 (fixed).
inline std::uint64_t monotonic_ms() noexcept {
    const auto& s = state();
    if (s.fixed) return s.frames * s.step_ms;
    return static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count());
}

// steady_clock-compatible time point on the game timeline. Differences between
// two results are the game-time elapsed, in either mode.
inline std::chrono::steady_clock::time_point steady_now() noexcept {
    const auto& s = state();
    if (s.fixed)
        return std::chrono::steady_clock::time_point{} +
               std::chrono::milliseconds(s.frames * s.step_ms);
    return std::chrono::steady_clock::now();
}

// Original Application/Random clock: gettimeofday milliseconds (wall time in
// real mode, the fixed timeline in fixed mode). Returns false on clock failure.
inline bool wall_ms(std::uint64_t& out) noexcept {
    const auto& s = state();
    if (s.fixed) {
        out = kFixedWallEpochMs + s.frames * s.step_ms;
        return true;
    }
    timeval now{};
    if (gettimeofday(&now, nullptr) != 0) return false;
    out = static_cast<std::uint64_t>(now.tv_sec) * 1000u +
          static_cast<std::uint64_t>(now.tv_usec) / 1000u;
    return true;
}

}  // namespace dh2::game_clock
