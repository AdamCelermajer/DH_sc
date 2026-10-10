#include "windows_source_session_control_v1.hpp"
#include <memory>
#include <stdexcept>

namespace dh::foundation::audio {
WindowsSourceSessionControlV1::WindowsSourceSessionControlV1(
 dh2::audio::AudioMixerV34& mixer,dh2::audio::AudioClockV40& clock,
 dh2::audio::AudioLifecycleGateV40& gate)
 :clock_(clock),gate_(gate),output_(mixer),epoch_(gate.source_epoch()),
 generation_((std::uint64_t(epoch_)<<32)|1u){}

bool WindowsSourceSessionControlV1::tick(std::string& error){
 if(stopped_){error="Windows source audio control has stopped";return false;}
 if(!generation_||!epoch_){error="Required same-session source epoch for WinMM control";return false;}
 if(!opened_){if(!output_.open(error))return false;opened_=true;}
 const bool permitted=gate_.permitted_for(epoch_);
 if(focused_!=permitted){
  if(!output_.focus(permitted,error)){ready_.store(false,std::memory_order_release);clock_.invalidate(generation_);return false;}
  focused_=permitted;
 }
 dh2::audio::AudioDeviceClockV40 sample;
 if(!output_.device_clock(generation_,sample,error)){
  ready_.store(false,std::memory_order_release);clock_.invalidate(generation_);return false;
 }
 sample.ready=permitted;clock_.publish(sample);
 if(permitted&&!output_.update(error)){
  ready_.store(false,std::memory_order_release);clock_.invalidate(generation_);return false;
 }
 ready_.store(permitted&&gate_.permitted_for(epoch_),std::memory_order_release);
 error.clear();return true;
}

bool WindowsSourceSessionControlV1::shutdown(std::string& error){
 stopped_=true;ready_.store(false,std::memory_order_release);clock_.invalidate(generation_);
 if(!output_.close_checked(error))return false;
 opened_=focused_=false;closed_.store(true,std::memory_order_release);error.clear();return true;
}
}

namespace dh2::audio {
namespace {
struct WindowsControlFactoryContextV1 {std::uint32_t signature=0x57494e31u;};
std::unique_ptr<AudioSessionControlOwnerV40> construct_windows_control(
 void* raw,AudioMixerV34& mixer,AudioClockV40& clock,AudioLifecycleGateV40& gate,std::string& error){
 auto* context=static_cast<WindowsControlFactoryContextV1*>(raw);
 if(!context||context->signature!=0x57494e31u){error="Required pinned Windows WinMM control factory context";return {};}
 // Runs on AudioNativeSessionV42's dedicated control thread, the WinMM pump owner (B039). A failed
 // promotion is not fatal; it is reported as pump_thread_priority=0 in the run log.
 dh::foundation::audio::winmm_promote_pump_thread_v1();
 error.clear();return std::make_unique<dh::foundation::audio::WindowsSourceSessionControlV1>(mixer,clock,gate);
}
}
AudioSessionControlFactoryV42 windows_source_session_control_v1(){
 auto owner=std::make_shared<WindowsControlFactoryContextV1>();
 return {construct_windows_control,owner.get(),std::move(owner)};
}
}
