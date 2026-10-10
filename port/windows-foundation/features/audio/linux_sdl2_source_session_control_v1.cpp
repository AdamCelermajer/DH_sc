#include "linux_sdl2_source_session_control_v1.hpp"
#include <memory>

namespace dh::foundation::audio {
LinuxSdl2SourceSessionControlV1::LinuxSdl2SourceSessionControlV1(
 dh2::audio::AudioMixerV34& mixer,dh2::audio::AudioClockV40& clock,
 dh2::audio::AudioLifecycleGateV40& gate)
 :clock_(clock),gate_(gate),output_(mixer),epoch_(gate.source_epoch()),
 generation_((std::uint64_t(epoch_)<<32)|1u){}

bool LinuxSdl2SourceSessionControlV1::tick(std::string& error){
 if(stopped_){error="Linux SDL2 source audio control has stopped";return false;}
 if(!generation_||!epoch_){error="Required same-session source epoch for SDL2 control";return false;}
 if(!opened_){if(!output_.open(error))return false;opened_=true;focused_=true;}
 const bool permitted=gate_.permitted_for(epoch_);
 if(focused_!=permitted){
  if(!output_.focus(permitted,error)){ready_.store(false,std::memory_order_release);clock_.invalidate(generation_);return false;}
  focused_=permitted;
 }
 if(!permitted){
  ready_.store(false,std::memory_order_release);clock_.invalidate(generation_);error.clear();return true;
 }
 dh2::audio::AudioDeviceClockV40 sample;
 if(!output_.device_clock(generation_,sample,error)){
  ready_.store(false,std::memory_order_release);clock_.invalidate(generation_);return false;
 }
 clock_.publish(sample);
 if(!output_.update(error)){
  ready_.store(false,std::memory_order_release);clock_.invalidate(generation_);return false;
 }
 const bool still_permitted=gate_.permitted_for(epoch_);
 ready_.store(still_permitted,std::memory_order_release);
 if(!still_permitted)clock_.invalidate(generation_);
 error.clear();return true;
}

bool LinuxSdl2SourceSessionControlV1::shutdown(std::string& error){
 stopped_=true;ready_.store(false,std::memory_order_release);clock_.invalidate(generation_);
 output_.close();opened_=focused_=false;
 if(output_.opened()){error="Required SDL2 output close barrier";return false;}
 closed_.store(true,std::memory_order_release);error.clear();return true;
}
}

namespace dh2::audio {
namespace {
struct LinuxSdl2ControlFactoryContextV1 {std::uint32_t signature=0x53444c31u;};
std::unique_ptr<AudioSessionControlOwnerV40> construct_linux_sdl2_control(
 void* raw,AudioMixerV34& mixer,AudioClockV40& clock,AudioLifecycleGateV40& gate,std::string& error){
 auto* context=static_cast<LinuxSdl2ControlFactoryContextV1*>(raw);
 if(!context||context->signature!=0x53444c31u){error="Required pinned Linux SDL2 control factory context";return {};}
 error.clear();return std::make_unique<dh::foundation::audio::LinuxSdl2SourceSessionControlV1>(mixer,clock,gate);
}
}
AudioSessionControlFactoryV42 linux_sdl2_source_session_control_v1(){
 auto owner=std::make_shared<LinuxSdl2ControlFactoryContextV1>();
 return {construct_linux_sdl2_control,owner.get(),std::move(owner)};
}
}
