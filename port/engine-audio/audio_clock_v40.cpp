#include "audio_clock_v40.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
namespace dh2::audio {
void AudioClockV40::publish(const AudioDeviceClockV40&v)noexcept{
 sequence_.fetch_add(1,std::memory_order_acq_rel);
 ready_.store(false,std::memory_order_relaxed);position_.store(v.position,std::memory_order_relaxed);time_.store(v.monotonic_ns,std::memory_order_relaxed);base_.store(v.mixer_base_frame,std::memory_order_relaxed);generation_.store(v.generation,std::memory_order_relaxed);rate_.store(v.rate,std::memory_order_relaxed);ready_.store(v.ready,std::memory_order_relaxed);
 sequence_.fetch_add(1,std::memory_order_release);
}
void AudioClockV40::invalidate(std::uint64_t generation)noexcept{publish({0,0,0,generation,0,false});}
bool AudioClockV40::snapshot(AudioDeviceClockV40&out)const noexcept{
 for(unsigned attempt=0;attempt<3;++attempt){const auto before=sequence_.load(std::memory_order_acquire);if(before&1)continue;AudioDeviceClockV40 v;v.ready=ready_.load(std::memory_order_relaxed);v.position=position_.load(std::memory_order_relaxed);v.monotonic_ns=time_.load(std::memory_order_relaxed);v.mixer_base_frame=base_.load(std::memory_order_relaxed);v.generation=generation_.load(std::memory_order_relaxed);v.rate=rate_.load(std::memory_order_relaxed);std::atomic_thread_fence(std::memory_order_acquire);const auto after=sequence_.load(std::memory_order_relaxed);if(before==after){out=v;return true;}}
 return false;
}
bool AudioClockV40::frame_at(std::int64_t event,std::uint64_t written,std::uint64_t&frame)const noexcept{
 AudioDeviceClockV40 v;if(event<=0||!snapshot(v)||!v.ready||v.position<0||v.monotonic_ns<=0||v.rate<8000||v.rate>192000)return false;
 const double target=double(v.mixer_base_frame)+double(v.position)+double(event-v.monotonic_ns)*double(v.rate)/1e9;
 if(!std::isfinite(target)||target<0||target>=double(std::numeric_limits<std::uint64_t>::max()))return false;
 frame=std::max(written,std::uint64_t(target));return true;
}
}
