// Linux: the shared game loop talks to the WinMM-named audio interface (winmm_output.hpp). On Linux that
// interface is backed by the SDL2 queued-audio output; no cue or mixer logic lives here.
#ifndef _WIN32
#include "winmm_output.hpp"
#include "linux_sdl2_output.hpp"

namespace dh::foundation::audio {

struct WinmmAudioOutput::Impl {
 LinuxSdl2AudioOutput output;
 explicit Impl(dh2::audio::AudioMixerV34& mixer):output(mixer){}
};

WinmmAudioOutput::WinmmAudioOutput(dh2::audio::AudioMixerV34& mixer):impl_(std::make_unique<Impl>(mixer)){}
WinmmAudioOutput::~WinmmAudioOutput()=default;
bool WinmmAudioOutput::open(std::string& error){return impl_->output.open(error);}
bool WinmmAudioOutput::update(std::string& error){return impl_->output.update(error);}
void WinmmAudioOutput::close() noexcept{impl_->output.close();}
bool WinmmAudioOutput::close_checked(std::string& error){impl_->output.close();error.clear();return true;}
bool WinmmAudioOutput::opened() const noexcept{return impl_->output.opened();}
bool WinmmAudioOutput::focus(bool actual_focused,std::string& error){return impl_->output.focus(actual_focused,error);}
AudioOutputServices WinmmAudioOutput::services(){return impl_->output.services();}
bool WinmmAudioOutput::device_clock(std::uint64_t generation,dh2::audio::AudioDeviceClockV40& clock,std::string& error){
 return impl_->output.device_clock(generation,clock,error);
}
bool winmm_monotonic_ns(std::int64_t& ns,std::string& error){return linux_monotonic_ns(ns,error);}
WinmmPcmStatsV1 winmm_pcm_stats_v1() noexcept{return {};}
WinmmPumpStatsV1 winmm_pump_stats_v1() noexcept{return {};}
void winmm_pump_stats_reset_v1() noexcept{}
bool winmm_promote_pump_thread_v1() noexcept{return false;}

}
#endif
