#include "linux_sdl2_output.hpp"

#ifndef _WIN32
#include <SDL.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <string>
#endif

namespace dh::foundation::audio {

struct LinuxSdl2AudioOutput::Impl {
 dh2::audio::AudioMixerV34& mixer;
#ifndef _WIN32
 SDL_AudioDeviceID device{};
 bool owns_audio_subsystem{};
 bool focused{true};
 std::uint64_t base_frame{},submitted_frames{};
 unsigned next_buffer{};
 std::array<std::array<std::int16_t,1024>,4> pcm{};
#endif
 explicit Impl(dh2::audio::AudioMixerV34& m):mixer(m){}
};

LinuxSdl2AudioOutput::LinuxSdl2AudioOutput(dh2::audio::AudioMixerV34& m):impl_(std::make_unique<Impl>(m)){}
LinuxSdl2AudioOutput::~LinuxSdl2AudioOutput(){close();}

bool LinuxSdl2AudioOutput::opened()const noexcept {
#ifndef _WIN32
 return impl_->device!=0;
#else
 return false;
#endif
}

bool LinuxSdl2AudioOutput::open(std::string& error) {
#ifndef _WIN32
 if(opened()){error.clear();return true;}
 impl_->owns_audio_subsystem=(SDL_WasInit(SDL_INIT_AUDIO)&SDL_INIT_AUDIO)==0;
 if(impl_->owns_audio_subsystem&&SDL_InitSubSystem(SDL_INIT_AUDIO)!=0){
  error=std::string("SDL audio initialization: ")+SDL_GetError();impl_->owns_audio_subsystem=false;return false;
 }
 SDL_AudioSpec wanted{};wanted.freq=48000;wanted.format=AUDIO_S16SYS;wanted.channels=2;wanted.samples=512;
 SDL_AudioSpec obtained{};
 impl_->device=SDL_OpenAudioDevice(nullptr,0,&wanted,&obtained,0);
 if(!impl_->device){
  error=std::string("SDL audio output open: ")+SDL_GetError();
  if(impl_->owns_audio_subsystem)SDL_QuitSubSystem(SDL_INIT_AUDIO);
  impl_->owns_audio_subsystem=false;return false;
 }
 if(obtained.freq!=wanted.freq||obtained.format!=wanted.format||obtained.channels!=wanted.channels){
  error="SDL output did not provide required 48 kHz stereo signed-16 PCM";
  SDL_CloseAudioDevice(impl_->device);impl_->device=0;
  if(impl_->owns_audio_subsystem)SDL_QuitSubSystem(SDL_INIT_AUDIO);
  impl_->owns_audio_subsystem=false;return false;
 }
 if(!impl_->mixer.set_rate(unsigned(obtained.freq))){
  error="Required stopped mixer before SDL output open";close();return false;
 }
 impl_->base_frame=impl_->mixer.output_frame();impl_->submitted_frames=0;impl_->next_buffer=0;impl_->focused=true;
 SDL_ClearQueuedAudio(impl_->device);
 SDL_PauseAudioDevice(impl_->device,0);
 error.clear();return true;
#else
 error="Required Linux SDL2 audio backend";return false;
#endif
}

bool LinuxSdl2AudioOutput::update(std::string& error) {
#ifndef _WIN32
 if(!opened()){error="Required opened SDL audio output";return false;}
 if(!impl_->focused){error.clear();return true;}
 constexpr Uint32 bytes_per_block=Uint32(1024*sizeof(std::int16_t));
 // Keep at most about four 512-frame blocks queued (42.7 ms at 48 kHz).
 // SDL copies the data, so each persistent conversion buffer can be reused.
 Uint32 queued=SDL_GetQueuedAudioSize(impl_->device);
 while(queued+bytes_per_block<=bytes_per_block*4u){
  std::array<float,1024> mixed{};
  auto& pcm=impl_->pcm[impl_->next_buffer];impl_->next_buffer=(impl_->next_buffer+1u)%unsigned(impl_->pcm.size());
  impl_->mixer.render(mixed.data(),512);
  for(unsigned i=0;i<mixed.size();++i){
   const float value=std::isfinite(mixed[i])?std::clamp(mixed[i],-1.f,1.f):0.f;
   pcm[i]=std::int16_t(value*32767.f);
  }
  if(SDL_QueueAudio(impl_->device,pcm.data(),bytes_per_block)!=0){error=std::string("SDL audio queue: ")+SDL_GetError();return false;}
  impl_->submitted_frames+=512;queued=SDL_GetQueuedAudioSize(impl_->device);
 }
 error.clear();return true;
#else
 error="Required actual platform audio output";return false;
#endif
}

void LinuxSdl2AudioOutput::close() noexcept {
#ifndef _WIN32
 if(impl_->device){SDL_ClearQueuedAudio(impl_->device);SDL_CloseAudioDevice(impl_->device);impl_->device=0;}
 if(impl_->owns_audio_subsystem){SDL_QuitSubSystem(SDL_INIT_AUDIO);impl_->owns_audio_subsystem=false;}
 impl_->focused=false;
#endif
}

AudioOutputServices LinuxSdl2AudioOutput::services(){
 return {this,
  [](void* p,std::string& e){return static_cast<LinuxSdl2AudioOutput*>(p)->open(e);},
  [](void* p,std::string& e){return static_cast<LinuxSdl2AudioOutput*>(p)->update(e);},
  [](void* p){static_cast<LinuxSdl2AudioOutput*>(p)->close();}};
}

bool LinuxSdl2AudioOutput::focus(bool focused,std::string& error) {
#ifndef _WIN32
 if(!opened()){error="Required opened actual SDL audio focus receiver";return false;}
 SDL_PauseAudioDevice(impl_->device,focused?0:1);
 impl_->focused=focused;
 error.clear();return true;
#else
 (void)focused;error="Required actual platform focus receiver";return false;
#endif
}

bool linux_monotonic_ns(std::int64_t& ns,std::string& error) {
#ifndef _WIN32
 const auto now=std::chrono::steady_clock::now().time_since_epoch();
 ns=std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
 if(ns<=0){error="Required positive steady monotonic clock";return false;}
 error.clear();return true;
#else
 (void)ns;error="Required Linux steady monotonic clock";return false;
#endif
}

bool LinuxSdl2AudioOutput::device_clock(std::uint64_t generation,dh2::audio::AudioDeviceClockV40& clock,std::string& error) {
 clock={};
#ifndef _WIN32
 if(!opened()||!generation||!impl_->focused){error="Required live focused SDL audio clock generation";return false;}
 const Uint32 bytes=SDL_GetQueuedAudioSize(impl_->device);
 constexpr std::uint64_t bytes_per_frame=2u*sizeof(std::int16_t);
 const std::uint64_t queued_frames=std::uint64_t(bytes)/bytes_per_frame;
 const std::uint64_t played=impl_->submitted_frames>queued_frames?impl_->submitted_frames-queued_frames:0;
 std::int64_t now{};if(!linux_monotonic_ns(now,error))return false;
 if(played>std::uint64_t(INT64_MAX)){error="SDL queued clock position overflow";return false;}
 // SDL exposes only its own queue depth. This estimate omits buffering after
 // SDL submits data to the host audio server/device.
 clock={std::int64_t(played),now,impl_->base_frame,generation,48000,true};
 error.clear();return true;
#else
 (void)generation;error="Required actual platform device timestamp";return false;
#endif
}

}
