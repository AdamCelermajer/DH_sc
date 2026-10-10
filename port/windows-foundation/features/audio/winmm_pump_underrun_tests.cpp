// B039 harness: WinMM output underruns under simulated game-frame hitches.
// Real AudioMixerV34 + WinmmAudioOutput + WindowsSourceSessionControlV1 on real WinMM.
// Scenario A pumps WinmmAudioOutput::update() from a simulated game frame (old hypothesis, not the
// production path). Scenario B runs the production control-thread pump (same 20 ms wait loop as
// AudioNativeSessionV42::control_thread) while a simulated game thread hitches. Underrun = an update()
// call where every queued header had already finished (see winmm_pump_stats_v1).
#include "winmm_output.hpp"
#include "windows_source_session_control_v1.hpp"
#include "../../../engine-audio/audio_mixer_v34.hpp"
#include "../../../engine-audio/audio_clock_v40.hpp"
#include "../../../engine-audio/integration-v40/focus/audio_lifecycle_gate_v40.hpp"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <memory>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

namespace fa = dh::foundation::audio;
namespace da = dh2::audio;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

namespace {
int failures = 0;
void check(bool ok, const std::string& message) {
    if (!ok) { ++failures; std::cout << "FAIL " << message << '\n'; }
}

struct Pattern {
    const char* name;
    int frame_ms;        // simulated game frame / pump period
    int hitch_ms;        // 0 = no hitch
    int hitch_period_ms; // hitch every N ms
};

struct Outcome {
    bool ok{};
    std::string error;
    fa::WinmmPumpStatsV1 stats{};
};

// Scenario A: update() is called once per simulated game frame, then the frame sleeps.
Outcome run_game_frame_pump(const Pattern& p, int run_ms) {
    Outcome out;
    auto mixer_owner=std::make_unique<da::AudioMixerV34>();da::AudioMixerV34& mixer=*mixer_owner; // heap: the mixer is large (main-thread stack overflow otherwise)
    fa::WinmmAudioOutput output(mixer);
    if (!output.open(out.error)) return out;
    fa::winmm_pump_stats_reset_v1();
    const auto start = steady_clock::now();
    auto next_hitch = start + milliseconds(p.hitch_period_ms);
    while (steady_clock::now() - start < milliseconds(run_ms)) {
        if (!output.update(out.error)) { output.close(); return out; }
        int frame = p.frame_ms;
        if (p.hitch_ms && steady_clock::now() >= next_hitch) {
            frame = p.hitch_ms;
            next_hitch += milliseconds(p.hitch_period_ms);
        }
        std::this_thread::sleep_for(milliseconds(frame));
    }
    out.stats = fa::winmm_pump_stats_v1();
    out.ok = output.close_checked(out.error);
    return out;
}

// Scenario B: production control-thread pump. stall_ms > 0 injects a starvation of the pump
// thread (not the game thread) every stall_period_ms to measure remaining margin.
Outcome run_control_thread(const Pattern& game, int run_ms, int stall_ms, int stall_period_ms) {
    Outcome out;
    auto mixer_owner=std::make_unique<da::AudioMixerV34>();da::AudioMixerV34& mixer=*mixer_owner; // heap: the mixer is large (main-thread stack overflow otherwise)
    da::AudioClockV40 clock;
    da::AudioLifecycleGateV40 gate;
    if (!gate.publish_activity(1, 1, true, true, true, false)) { out.error = "gate activity"; return out; }
    const auto epoch = gate.begin_source();
    if (!gate.publish_source(epoch, true)) { out.error = "gate source"; return out; }
    fa::WindowsSourceSessionControlV1 control(mixer, clock, gate);

    std::mutex mutex;
    std::condition_variable changed;
    bool stop = false;
    std::string tick_error;
    fa::winmm_pump_stats_reset_v1();
    std::thread pump([&] {
        // Mirrors AudioNativeSessionV42::control_thread: tick, then wait_for(20 ms) or stop.
        std::unique_lock<std::mutex> lock(mutex);
        auto last_stall = steady_clock::now();
        while (!stop) {
            lock.unlock();
            std::string error;
            if (!control.tick(error)) { lock.lock(); tick_error = error; break; }
            if (stall_ms > 0 && steady_clock::now() - last_stall >= milliseconds(stall_period_ms)) {
                std::this_thread::sleep_for(milliseconds(stall_ms));
                last_stall = steady_clock::now();
            }
            lock.lock();
            changed.wait_for(lock, milliseconds(20), [&] { return stop; });
        }
    });

    // Simulated game thread: frames and hitches only; it never touches the output.
    const auto start = steady_clock::now();
    auto next_hitch = start + milliseconds(game.hitch_period_ms);
    while (steady_clock::now() - start < milliseconds(run_ms)) {
        int frame = game.frame_ms;
        if (game.hitch_ms && steady_clock::now() >= next_hitch) {
            frame = game.hitch_ms;
            next_hitch += milliseconds(game.hitch_period_ms);
        }
        std::this_thread::sleep_for(milliseconds(frame));
    }
    {
        std::lock_guard<std::mutex> lock(mutex);
        stop = true;
    }
    changed.notify_all();
    pump.join();
    out.stats = fa::winmm_pump_stats_v1();
    if (!tick_error.empty()) { out.error = "tick: " + tick_error; return out; }
    out.ok = control.shutdown(out.error) && control.close_succeeded();
    return out;
}

void report(const char* label, const Outcome& o, int run_ms) {
    const double gap_ms = double(o.stats.max_update_gap_ns) / 1e6;
    std::cout << label << " run_ms=" << run_ms << " updates=" << o.stats.updates
              << " refills=" << o.stats.refills << " underruns=" << o.stats.underruns
              << " max_update_gap_ms=" << gap_ms << (o.error.empty() ? "" : " error=" + o.error) << '\n';
}

// Lifecycle: open/close/reset twice, shutdown while pumping, focus loss/restore while pumping.
void lifecycle_tests() {
    for (int i = 0; i < 2; ++i) {
        auto mixer_owner=std::make_unique<da::AudioMixerV34>();da::AudioMixerV34& mixer=*mixer_owner; // heap: the mixer is large (main-thread stack overflow otherwise)
        fa::WinmmAudioOutput output(mixer);
        std::string error;
        check(output.open(error), "lifecycle open #" + std::to_string(i) + ": " + error);
        check(output.update(error), "lifecycle update #" + std::to_string(i) + ": " + error);
        check(output.close_checked(error), "lifecycle close #" + std::to_string(i) + ": " + error);
        check(!output.opened(), "lifecycle closed flag #" + std::to_string(i));
        check(output.close_checked(error), "lifecycle second close is a no-op #" + std::to_string(i));
    }
    for (int i = 0; i < 3; ++i) {
        const Pattern quick{"shutdown", 16, 0, 0};
        const auto o = run_control_thread(quick, 300 + 100 * i, 0, 0);
        check(o.ok, "shutdown while pumping #" + std::to_string(i) + ": " + o.error);
    }
    // Focus loss then restore while the control thread is pumping.
    auto mixer_owner=std::make_unique<da::AudioMixerV34>();da::AudioMixerV34& mixer=*mixer_owner; // heap: the mixer is large (main-thread stack overflow otherwise)
    da::AudioClockV40 clock;
    da::AudioLifecycleGateV40 gate;
    check(gate.publish_activity(1, 1, true, true, true, false), "focus fixture activity");
    const auto epoch = gate.begin_source();
    check(gate.publish_source(epoch, true), "focus fixture source");
    fa::WindowsSourceSessionControlV1 control(mixer, clock, gate);
    std::string error;
    check(control.tick(error), "focus fixture first tick: " + error);
    check(gate.publish_activity(1, 2, true, true, false, false), "focus loss publish");
    check(control.tick(error), "focus loss tick: " + error);
    check(gate.publish_activity(1, 3, true, true, true, false), "focus restore publish");
    check(control.tick(error), "focus restore tick: " + error);
    check(control.shutdown(error) && control.close_succeeded(), "focus fixture shutdown: " + error);
}
}  // namespace

int main(int argc, char** argv) {
    // "--stress" adds the control-thread starvation sweep; default is the decision set.
    std::cout << std::unitbuf;
    const bool stress = argc > 1 && std::string(argv[1]) == "--stress";
    std::cout << "WinMM ring: " << fa::kWinmmBufferCount << " x " << fa::kWinmmFramesPerBuffer
              << " frames (" << (1000.0 * fa::kWinmmBufferCount * fa::kWinmmFramesPerBuffer / 48000.0)
              << " ms queued)\n";
    const int run_ms = 6000;
    const Pattern patterns[] = {
        {"A-16ms", 16, 0, 0},
        {"A-33ms", 33, 0, 0},
        {"A-50ms", 50, 0, 0},
        {"A-80ms", 80, 0, 0},
        {"A-16ms+120ms-hitch-every-2s", 16, 120, 2000},
    };
    std::cout << "Scenario A: game-frame-driven pump (old hypothesis)\n";
    for (const auto& p : patterns) {
        const auto o = run_game_frame_pump(p, run_ms);
        check(o.error.empty(), std::string("scenario A ") + p.name + ": " + o.error);
        report(p.name, o, run_ms);
    }
    std::cout << "Scenario B: production control-thread pump (WindowsSourceSessionControlV1, 20 ms loop)\n";
    const Pattern game_patterns[] = {
        {"B-16ms+120ms-hitch-every-2s", 16, 120, 2000},
        {"B-80ms-frames", 80, 0, 0},
    };
    for (const auto& p : game_patterns) {
        const auto o = run_control_thread(p, run_ms, 0, 0);
        check(o.ok, std::string("scenario B ") + p.name + ": " + o.error);
        report(p.name, o, run_ms);
        check(o.stats.underruns == 0, std::string("scenario B ") + p.name + " must have 0 underruns");
    }
    if (stress) {
        std::cout << "Stress: control-thread starvation injected every 2 s (informational margin)\n";
        const int stalls[] = {20, 30, 40, 60};
        for (int stall : stalls) {
            const Pattern game{"stress", 16, 0, 0};
            const auto o = run_control_thread(game, run_ms, stall, 2000);
            report((std::string("B-stall-") + std::to_string(stall) + "ms").c_str(), o, run_ms);
        }
    }
    lifecycle_tests();
    std::cout << (failures ? "FAILED" : "PASS") << " winmm pump underrun harness failures=" << failures << '\n';
    return failures ? EXIT_FAILURE : EXIT_SUCCESS;
}
