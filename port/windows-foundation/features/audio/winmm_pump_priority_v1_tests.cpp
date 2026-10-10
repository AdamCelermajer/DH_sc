// B039: the WinMM pump (AudioNativeSessionV42 control thread) is promoted to time-critical priority.
// Deterministic checks only (no device open): the helper promotes exactly the calling thread and
// records it in the pump stats; the production control factory promotes the thread that constructs
// the control owner (in production, the dedicated control thread) and not any other thread.
#include "winmm_output.hpp"
#include "windows_source_session_control_v1.hpp"
#include "../../../engine-audio/audio_mixer_v34.hpp"
#include "../../../engine-audio/audio_clock_v40.hpp"
#include "../../../engine-audio/integration-v40/focus/audio_lifecycle_gate_v40.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

namespace fa = dh::foundation::audio;
namespace da = dh2::audio;

static int failures = 0;
static void check(bool ok, const std::string& message) {
    if (!ok) { ++failures; std::cout << "FAIL " << message << '\n'; }
}

int main() {
    const int main_before = GetThreadPriority(GetCurrentThread());
    check(fa::winmm_pump_stats_v1().pump_thread_priority == 0, "stats report no promotion before any pump thread");

    // 1. Helper promotes the calling thread only.
    int helper_priority = 0; bool helper_ok = false;
    std::thread helper([&] { helper_ok = fa::winmm_promote_pump_thread_v1(); helper_priority = GetThreadPriority(GetCurrentThread()); });
    helper.join();
    check(helper_ok, "winmm_promote_pump_thread_v1 reports success");
    check(helper_priority == THREAD_PRIORITY_TIME_CRITICAL, "promoted thread runs at THREAD_PRIORITY_TIME_CRITICAL");
    check(fa::winmm_pump_stats_v1().pump_thread_priority == THREAD_PRIORITY_TIME_CRITICAL, "stats record the promoted priority");
    check(GetThreadPriority(GetCurrentThread()) == main_before, "caller of other threads is not promoted");

    // 2. Production factory: the constructing thread (the control thread in AudioNativeSessionV42) is promoted.
    fa::winmm_pump_stats_reset_v1();
    auto mixer = std::make_unique<da::AudioMixerV34>(); // heap: the mixer is large
    da::AudioClockV40 clock; da::AudioLifecycleGateV40 gate;
    auto factory = da::windows_source_session_control_v1();
    check(factory.valid(), "production Windows control factory is valid");
    int control_priority = 0; bool constructed = false; std::string error;
    std::thread control([&] {
        auto owner = factory.construct(factory.context, *mixer, clock, gate, error);
        constructed = owner != nullptr;
        control_priority = GetThreadPriority(GetCurrentThread());
    });
    control.join();
    check(constructed && error.empty(), "factory constructs the WinMM control owner: " + error);
    check(control_priority == THREAD_PRIORITY_TIME_CRITICAL, "factory promotes its constructing (control) thread");
    check(GetThreadPriority(GetCurrentThread()) == main_before, "factory does not promote the producer/main thread");

    if (failures) { std::cout << failures << " failure(s)\n"; return 1; }
    std::cout << "PASS winmm_pump_priority_v1: helper and production factory promote only the pump thread to time-critical\n";
    return 0;
}
