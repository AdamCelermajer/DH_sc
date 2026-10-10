#include "winmm_output.hpp"
#include <array>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#endif
namespace dh::foundation::audio {
#ifdef _WIN32
namespace {
constexpr unsigned kBuffers=kWinmmBufferCount;
constexpr unsigned kFrames=kWinmmFramesPerBuffer;
constexpr unsigned kSamples=kFrames*2;
// Process-wide pump diagnostics (B039). Written only by the single pump owner.
std::atomic<std::uint64_t> g_pump_updates{},g_pump_refills{},g_pump_underruns{},g_pump_max_gap_ns{},g_pump_last_ns{},g_pump_first_ns{},g_pump_gaps_over_40ms{};
std::uint64_t pump_now_ns() {
 return std::uint64_t(std::chrono::duration_cast<std::chrono::nanoseconds>(
  std::chrono::steady_clock::now().time_since_epoch()).count());
}
}
#endif
struct WinmmAudioOutput::Impl {
 dh2::audio::AudioMixerV34& mixer;
#ifdef _WIN32
 HWAVEOUT device{};std::array<WAVEHDR,kBuffers> headers{};
 std::array<std::array<std::int16_t,kSamples>,kBuffers> pcm{};
 std::array<bool,kBuffers> prepared{},submitted{};
 std::uint64_t base_frame{},position_wrap{};DWORD last_position{};
#endif
 explicit Impl(dh2::audio::AudioMixerV34& m):mixer(m){}
};
WinmmAudioOutput::WinmmAudioOutput(dh2::audio::AudioMixerV34& m):impl_(std::make_unique<Impl>(m)){}
WinmmAudioOutput::~WinmmAudioOutput(){close();}
bool WinmmAudioOutput::opened()const noexcept {
#ifdef _WIN32
 return impl_->device!=nullptr;
#else
 return false;
#endif
}
bool WinmmAudioOutput::open(std::string& error){
#ifdef _WIN32
 if(opened())return true;WAVEFORMATEX f{};f.wFormatTag=WAVE_FORMAT_PCM;f.nChannels=2;f.nSamplesPerSec=48000;f.wBitsPerSample=16;f.nBlockAlign=4;f.nAvgBytesPerSec=192000;
 auto result=waveOutOpen(&impl_->device,WAVE_MAPPER,&f,0,0,CALLBACK_NULL);if(result!=MMSYSERR_NOERROR){impl_->device=nullptr;error="Required WinMM output: "+std::to_string(result);return false;}
 if(!impl_->mixer.set_rate(f.nSamplesPerSec)){error="Required stopped mixer before WinMM open";close();return false;}
 impl_->base_frame=impl_->mixer.output_frame();impl_->position_wrap=0;impl_->last_position=0;
 for(unsigned i=0;i<kBuffers;++i){auto& h=impl_->headers[i];h={};h.lpData=reinterpret_cast<char*>(impl_->pcm[i].data());h.dwBufferLength=kSamples*sizeof(std::int16_t);result=waveOutPrepareHeader(impl_->device,&h,sizeof h);if(result!=MMSYSERR_NOERROR){error="WinMM prepare: "+std::to_string(result);close();return false;}impl_->prepared[i]=true;}
 return true;
#else
 error="Required Windows WinMM backend (Android uses original AAudio endpoint)";return false;
#endif
}
bool WinmmAudioOutput::update(std::string& error){
#ifdef _WIN32
 if(!opened()){error="Required opened WinMM output";return false;}
 {
  // Underrun = every queued header had already finished before this pump call, so the device ran dry.
  bool any_submitted=false,all_done=true;
  for(unsigned i=0;i<kBuffers;++i){if(!impl_->submitted[i])continue;any_submitted=true;if(!(impl_->headers[i].dwFlags&WHDR_DONE))all_done=false;}
  if(any_submitted&&all_done)g_pump_underruns.fetch_add(1,std::memory_order_relaxed);
  const auto now=pump_now_ns(),last=g_pump_last_ns.exchange(now,std::memory_order_relaxed);
  if(!g_pump_first_ns.load(std::memory_order_relaxed))g_pump_first_ns.store(now,std::memory_order_relaxed);
  if(last&&now>last&&now-last>g_pump_max_gap_ns.load(std::memory_order_relaxed))g_pump_max_gap_ns.store(now-last,std::memory_order_relaxed);
  if(last&&now>last&&now-last>40000000u)g_pump_gaps_over_40ms.fetch_add(1,std::memory_order_relaxed);
  g_pump_updates.fetch_add(1,std::memory_order_relaxed);
 }
 std::array<float,kSamples> mixed{};
 for(unsigned i=0;i<kBuffers;++i){auto& h=impl_->headers[i];if(impl_->submitted[i]&&!(h.dwFlags&WHDR_DONE))continue;
  impl_->mixer.render(mixed.data(),kFrames);
  for(unsigned j=0;j<kSamples;++j){float v=std::isfinite(mixed[j])?std::clamp(mixed[j],-1.f,1.f):0.f;impl_->pcm[i][j]=std::int16_t(v*32767.f);}
  auto result=waveOutWrite(impl_->device,&h,sizeof h);if(result!=MMSYSERR_NOERROR){error="WinMM write: "+std::to_string(result);return false;}impl_->submitted[i]=true;
  g_pump_refills.fetch_add(1,std::memory_order_relaxed);
 }return true;
#else
 error="Required actual platform output";return false;
#endif
}
void WinmmAudioOutput::close() noexcept {
 try {std::string ignored;close_checked(ignored);}catch(...){}
}
bool WinmmAudioOutput::close_checked(std::string& error) {
#ifdef _WIN32
 if(!opened())return true;auto status=waveOutReset(impl_->device);if(status!=MMSYSERR_NOERROR){error="WinMM reset: "+std::to_string(status);return false;}for(unsigned i=0;i<kBuffers;++i){if(impl_->prepared[i]){status=waveOutUnprepareHeader(impl_->device,&impl_->headers[i],sizeof(WAVEHDR));if(status!=MMSYSERR_NOERROR){error="WinMM unprepare: "+std::to_string(status);return false;}}impl_->prepared[i]=false;impl_->submitted[i]=false;}status=waveOutClose(impl_->device);if(status!=MMSYSERR_NOERROR){error="WinMM close: "+std::to_string(status);return false;}impl_->device=nullptr;return true;
#else
 (void)error;return true;
#endif
}
AudioOutputServices WinmmAudioOutput::services(){return {this,[](void* p,std::string& e){return static_cast<WinmmAudioOutput*>(p)->open(e);},[](void* p,std::string& e){return static_cast<WinmmAudioOutput*>(p)->update(e);},[](void* p){static_cast<WinmmAudioOutput*>(p)->close();}};}
bool winmm_monotonic_ns(std::int64_t& ns,std::string& error){
#ifdef _WIN32
 LARGE_INTEGER counter{},frequency{};if(!QueryPerformanceCounter(&counter)||!QueryPerformanceFrequency(&frequency)||frequency.QuadPart<=0){error="Required actual QPC monotonic clock";return false;}const auto seconds=counter.QuadPart/frequency.QuadPart,remainder=counter.QuadPart%frequency.QuadPart;ns=seconds*1000000000LL+std::int64_t(double(remainder)*1e9/double(frequency.QuadPart));return ns>0;
#else
 (void)ns;error="Required actual Windows output clock";return false;
#endif
}
bool WinmmAudioOutput::device_clock(std::uint64_t generation,dh2::audio::AudioDeviceClockV40& clock,std::string& error){clock={};
#ifdef _WIN32
 if(!opened()||!generation){error="Required live WinMM clock generation";return false;}MMTIME time{};time.wType=TIME_SAMPLES;auto status=waveOutGetPosition(impl_->device,&time,sizeof time);if(status!=MMSYSERR_NOERROR||time.wType!=TIME_SAMPLES){error="Required actual WinMM sample timestamp: "+std::to_string(status);return false;}std::int64_t ns{};if(!winmm_monotonic_ns(ns,error))return false;
 if(time.u.sample<impl_->last_position)impl_->position_wrap+=std::uint64_t(1)<<32;impl_->last_position=time.u.sample;clock={std::int64_t(impl_->position_wrap+time.u.sample),ns,impl_->base_frame,generation,48000,true};return true;
#else
 (void)generation;error="Required actual platform device timestamp";return false;
#endif
}
bool WinmmAudioOutput::focus(bool focused,std::string& error){
#ifdef _WIN32
 if(!opened()){error="Required opened actual WinMM focus receiver";return false;}auto status=focused?waveOutRestart(impl_->device):waveOutPause(impl_->device);if(status!=MMSYSERR_NOERROR){error="WinMM focus state: "+std::to_string(status);return false;}return true;
#else
 (void)focused;error="Required actual platform focus receiver";return false;
#endif
}
#ifdef _WIN32
WinmmPumpStatsV1 winmm_pump_stats_v1() noexcept {
 return {g_pump_updates.load(),g_pump_refills.load(),g_pump_underruns.load(),g_pump_max_gap_ns.load(),g_pump_first_ns.load(),g_pump_last_ns.load(),g_pump_gaps_over_40ms.load()};
}
void winmm_pump_stats_reset_v1() noexcept {
 g_pump_updates=0;g_pump_refills=0;g_pump_underruns=0;g_pump_max_gap_ns=0;g_pump_last_ns=0;g_pump_first_ns=0;g_pump_gaps_over_40ms=0;
}
#else
WinmmPumpStatsV1 winmm_pump_stats_v1() noexcept {return {};}
void winmm_pump_stats_reset_v1() noexcept {}
#endif
}
