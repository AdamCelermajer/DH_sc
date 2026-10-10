#pragma once
#include "retained_frame_audio_clock.hpp"
#include "winmm_output.hpp"
#include "../../../engine-audio/audio_gameplay_runtime_v42.hpp"
namespace dh::foundation::audio {
// Owns ONE recovered V42 source manager runtime, sample bank, channel registry,
// mixer and WinMM output. Do not attach SourceAudioRouter as a second runtime.
// Root supplies real original gates and source_command authorities. This wrapper
// supplies the concrete platform output-ready leaf and actual device clock.
class PlatformSourceAudio {
 dh2::audio::AudioGameplaySourcesV40 source_;
 std::unique_ptr<dh2::audio::AudioGameplayRuntimeV42> runtime_;
 std::unique_ptr<WinmmAudioOutput> output_;
 std::uint64_t generation_{1};bool initialized_{},opened_{},focused_{},finalized_{};
 static bool output_ready(void*,std::string&);
 static bool command(void*,const dh2::character::CombatSoundPlayV1&,const dh2::audio::AudioSoundV34&,const dh2::audio::AudioGroupV34&,dh2::audio::AudioCommandV34&,std::string&);
public:
 PlatformSourceAudio(std::uintptr_t actual_manager,dh2::audio::AudioGameplaySourcesV40);
 ~PlatformSourceAudio();
 bool initialize_and_open(std::string&);
 // Call before dispatch to publish hardware sample clock, after dispatch to
 // submit output buffers and drain actual channel receipts. No private timer.
 bool publish_device_clock(std::string&);
 bool publish_device_clock(dh::foundation::RetainedFrameAudioClock&,std::string&);
 // Read only the clock currently published by the same V42 runtime. Does not
 // recapture hardware time or manufacture a replacement sample.
 bool published_device_clock(dh::foundation::RetainedFrameAudioClock&,std::string&)const;
 bool set_actual_focus(bool focused,std::string&);
 bool pump(std::string&);
 bool close_and_drain(std::string&);
 dh2::audio::AudioGameplayRuntimeV42& runtime() noexcept{return *runtime_;}
 WinmmAudioOutput& output() noexcept{return *output_;}
 bool initialized()const noexcept{return initialized_;}
};
}
