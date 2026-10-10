#pragma once
#include "feature_audio.hpp"
#include "../../../engine-audio/audio_clock_v40.hpp"
#include <memory>
namespace dh::foundation::audio {
// WinMM ring: kWinmmBufferCount persistent buffers of kWinmmFramesPerBuffer stereo 16-bit frames.
// The queued audio must outlast the control thread's worst refill period (~20 ms nominal, ~31-35 ms
// observed on Windows' default 15.6 ms timer), so the ring is sized for that period plus slop (B039).
inline constexpr unsigned kWinmmBufferCount=8;
inline constexpr unsigned kWinmmFramesPerBuffer=512;
// Control-thread pump. Persistent buffers, real waveOut errors.
class WinmmAudioOutput {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 explicit WinmmAudioOutput(dh2::audio::AudioMixerV34&);
 ~WinmmAudioOutput();
 bool open(std::string&);bool update(std::string&);void close() noexcept;
 bool close_checked(std::string&);
 bool opened()const noexcept;
 bool focus(bool actual_focused,std::string&);
 AudioOutputServices services();
 bool device_clock(std::uint64_t generation,dh2::audio::AudioDeviceClockV40&,std::string&);
};
bool winmm_monotonic_ns(std::int64_t&,std::string&);
// B039 pump diagnostics (process-wide, Windows only; zeros elsewhere). An underrun is an
// update() call where every queued header had already completed (device ran dry).
// first_ns/last_ns: steady-clock time of the first and latest update(); gaps_over_40ms counts update gaps > 40 ms.
struct WinmmPumpStatsV1 {
 std::uint64_t updates{},refills{},underruns{},max_update_gap_ns{},first_ns{},last_ns{},gaps_over_40ms{};
 int pump_thread_priority{}; // GetThreadPriority of the promoted pump thread (15 = time-critical); 0 = not promoted
};
WinmmPumpStatsV1 winmm_pump_stats_v1() noexcept;
void winmm_pump_stats_reset_v1() noexcept;
// B039: the WinMM ring (8 x 512 frames = 85 ms) is refilled by polling from the dedicated audio control
// thread, so that thread must be scheduled ahead of game and other-process work, like a platform audio
// callback thread (AAudio/SDL do this themselves). Promotes the CALLING thread to
// THREAD_PRIORITY_TIME_CRITICAL; it sleeps between 20 ms ticks and renders well under 1 ms per tick.
// Returns false (changing nothing) where unsupported.
bool winmm_promote_pump_thread_v1() noexcept;
}
