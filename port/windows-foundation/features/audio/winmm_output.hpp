#pragma once
#include "feature_audio.hpp"
#include "../../../engine-audio/audio_clock_v40.hpp"
#include <memory>
namespace dh::foundation::audio {
// Control-thread pump. Four persistent 512-frame buffers, real waveOut errors.
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
}
