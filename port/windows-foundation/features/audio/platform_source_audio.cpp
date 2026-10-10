#include "platform_source_audio.hpp"
namespace dh::foundation::audio {
PlatformSourceAudio::PlatformSourceAudio(std::uintptr_t manager,dh2::audio::AudioGameplaySourcesV40 source):source_(source){auto wrapped=source;wrapped.context=this;wrapped.output_ready=output_ready;wrapped.source_command=command;runtime_=std::make_unique<dh2::audio::AudioGameplayRuntimeV42>(manager,wrapped);output_=std::make_unique<WinmmAudioOutput>(runtime_->mixer());}
PlatformSourceAudio::~PlatformSourceAudio(){output_->close();runtime_->clock().invalidate(generation_);}
bool PlatformSourceAudio::output_ready(void* raw,std::string& error){auto& self=*static_cast<PlatformSourceAudio*>(raw);if(!self.opened_||!self.output_->opened()||!self.focused_){error="Required SAME actual focused WinMM source output";return false;}return true;}
bool PlatformSourceAudio::command(void* raw,const dh2::character::CombatSoundPlayV1& play,const dh2::audio::AudioSoundV34& sound,const dh2::audio::AudioGroupV34& group,dh2::audio::AudioCommandV34& out,std::string& error){auto& self=*static_cast<PlatformSourceAudio*>(raw);if(!self.source_.source_command){error="Required actual source emitter/listener/DSP command authority";return false;}return self.source_.source_command(self.source_.context,play,sound,group,out,error);}
bool PlatformSourceAudio::initialize_and_open(std::string& error){if(finalized_){error="Source process audio already finalized; construct a new actual process receiver";return false;}if(opened_)return true;if(!initialized_){if(!runtime_->initialize_exact_source(error))return false;initialized_=true;}if(!output_->open(error))return false;opened_=true;if(!output_->focus(focused_,error)||!publish_device_clock(error)){opened_=false;output_->close();return false;}return true;}
bool PlatformSourceAudio::publish_device_clock(std::string& error){dh::foundation::RetainedFrameAudioClock ignored;return publish_device_clock(ignored,error);}
bool PlatformSourceAudio::publish_device_clock(dh::foundation::RetainedFrameAudioClock& sample,std::string& error){
 sample={};dh2::audio::AudioDeviceClockV40 device;
 if(!opened_||!output_->device_clock(generation_,device,error)){
  runtime_->clock().invalidate(generation_);if(error.empty())error="Required actual source output device clock";return false;
 }
 device.ready=focused_;runtime_->clock().publish(device);
 sample={device.generation,device.position,device.monotonic_ns,device.ready};
 return true;
}
bool PlatformSourceAudio::published_device_clock(dh::foundation::RetainedFrameAudioClock& sample,std::string& error)const{
 sample={};dh2::audio::AudioDeviceClockV40 published;
 if(!runtime_->clock().snapshot(published)||!published.ready||published.generation!=generation_||published.position<0||published.monotonic_ns<=0){
  error="No valid ready clock is currently published by the same V42 runtime";return false;
 }
 sample={published.generation,published.position,published.monotonic_ns,published.ready};error.clear();return sample.valid();
}
bool PlatformSourceAudio::pump(std::string& error){if(!publish_device_clock(error))return false;if(focused_&&!output_->update(error)){runtime_->clock().invalidate(generation_);opened_=false;return false;}runtime_->pump_receipts();return true;}
bool PlatformSourceAudio::set_actual_focus(bool focused,std::string& error){if(!opened_){error="Required initialized actual source output";return false;}if(!output_->focus(focused,error))return false;focused_=focused;return publish_device_clock(error);}
bool PlatformSourceAudio::close_and_drain(std::string& error){if(finalized_)return true;if(!initialized_){if(!output_->close_checked(error))return false;opened_=false;return true;}if(!runtime_->stop_world(error))return false;runtime_->clock().invalidate(generation_);if(!output_->close_checked(error))return false;opened_=false;std::atomic<bool>closed{true},joined{true};if(!runtime_->finalize_after_output_closed({&runtime_->mixer(),&closed,&joined},error))return false;++generation_;finalized_=true;return true;}
}
