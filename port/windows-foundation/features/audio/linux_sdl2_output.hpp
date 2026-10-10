#pragma once
#include "feature_audio.hpp"
#include "../../../engine-audio/audio_clock_v40.hpp"
#include <memory>

namespace dh::foundation::audio {

// Control-thread SDL2 queued-audio consumer for the shared V34 mixer.
// The device clock is an estimate based on SDL's queued byte count, not a
// hardware timestamp.
class LinuxSdl2AudioOutput {
 struct Impl;
 std::unique_ptr<Impl> impl_;
public:
 explicit LinuxSdl2AudioOutput(dh2::audio::AudioMixerV34&);
 ~LinuxSdl2AudioOutput();
 LinuxSdl2AudioOutput(const LinuxSdl2AudioOutput&)=delete;
 LinuxSdl2AudioOutput& operator=(const LinuxSdl2AudioOutput&)=delete;
 bool open(std::string&);
 bool update(std::string&);
 void close() noexcept;
 bool opened()const noexcept;
 bool focus(bool actual_focused,std::string&);
 AudioOutputServices services();
 bool device_clock(std::uint64_t generation,dh2::audio::AudioDeviceClockV40&,std::string&);
};

bool linux_monotonic_ns(std::int64_t&,std::string&);

}
