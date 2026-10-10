#include "audio_application_manager_v42.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <exception>
namespace dh2::audio {
bool AudioApplicationManagerV42::producer(std::string&error)const{
 if(std::this_thread::get_id()==producer_)return true;
 error="Required same Application audio producer thread";return false;
}
std::shared_ptr<AudioApplicationManagerV42> AudioApplicationManagerV42::create(
 AudioGameplaySourcesV40 sources,std::shared_ptr<void> lease,bool disabled,std::string&error
#ifdef DH2_AUDIO_NATIVE_SESSION_FIXTURE
 ,Factory factory,void* context
#endif
 ){
 auto next=std::shared_ptr<AudioApplicationManagerV42>(new AudioApplicationManagerV42(disabled));
 next->session_=std::make_shared<AudioNativeSessionV42>(next->identity(),sources,std::move(lease),application_audio_gate_v40()
#ifdef DH2_AUDIO_NATIVE_SESSION_FIXTURE
 ,factory,context
#endif
 );
 if(!next->session_->initialize(error)){
  const std::string construction_error=error;std::string close_error;
  if(!next->session_->shutdown(close_error)){
   // Failed driver closure must retain complete callback storage. Process
   // Application owns this failure lease; no replacement may be constructed.
   static auto* retained=new std::vector<std::shared_ptr<AudioApplicationManagerV42>>;
   retained->push_back(next);error=construction_error+"; retained failed owner: "+close_error;return {};
  }
  next->session_.reset();error=construction_error;return {};
 }
 error.clear();return next;
}
std::shared_ptr<AudioApplicationManagerV42> AudioApplicationManagerV42::create(
 AudioGameplaySourcesV40 sources,std::shared_ptr<void> lease,bool disabled,std::string&error,
 AudioSessionControlFactoryV42 factory){
 auto next=std::shared_ptr<AudioApplicationManagerV42>(new AudioApplicationManagerV42(disabled));
 next->session_=std::make_shared<AudioNativeSessionV42>(next->identity(),sources,std::move(lease),
  application_audio_gate_v40(),std::move(factory));
 if(!next->session_->initialize(error)){
  const std::string construction_error=error;std::string close_error;
  if(!next->session_->shutdown(close_error)){
   static auto* retained=new std::vector<std::shared_ptr<AudioApplicationManagerV42>>;
   retained->push_back(next);error=construction_error+"; retained failed owner: "+close_error;return {};
  }
  next->session_.reset();error=construction_error;return {};
 }
 error.clear();return next;
}
bool AudioApplicationManagerV42::set_actual_disabled(bool value,std::string&error){
 if(!producer(error))return false;
 if(closing_){error="Application SoundManager closing";return false;}
 disabled_=value;error.clear();return true;
}
bool AudioApplicationManagerV42::set_source_volume_v68(int selector,float percent,std::string&error){
 if(!producer(error)||closing_||!std::isfinite(percent)){if(error.empty())error="Required live source SetSoundVolume receiver/value";return false;}
 if(disabled_){error="Required actual platform SetSoundVolume JNI";return false;}
 auto*runtime=runtime_on_producer();if(!runtime){error="Required SAME source group engine";return false;}
 const char*label=selector==1?"SFX":selector==2?"MUSIC":selector==3?"VFX":nullptr;
 std::uint32_t mask{};if(label&&!runtime->catalog().group_mask(label,mask)){error="Required selected XML groupmask";return false;}
 const auto gain=std::clamp(percent/100.f,0.f,1.f);
 // Source SetGroupGain mask0 does not change master or any group. The pitched
 // group's selector7/8 follows this path; still retain the preceding RNG call.
 std::array<bool,3>posted{};
 for(const auto&group:runtime->catalog().groups())if(group.uid>0&&group.uid<32&&(mask&(std::uint32_t(1)<<group.uid))){
  if(group.volume_group<0||group.volume_group>2){error="Required selected source volume group";return false;}
  if(!posted[group.volume_group]){AudioCommandV34 command;command.kind=AudioCommandKindV34::volume;command.volume_group=group.volume_group;command.left=gain;command.source_fade_seconds=.05f;
   if(!runtime->mixer().post(command)){error="Source group setter queue full";return false;}posted[group.volume_group]=true;}
  source_group_gain_[group.uid]=gain;
 }
 error.clear();return true;
}
bool AudioApplicationManagerV42::initialize_source_settings_v68(int fx,int music,std::string&error){
 if(!producer(error)||closing_){if(error.empty())error="Required live Initialize receiver";return false;}
 if(settings_initialize_phase_==0){if(!set_source_volume_v68(1,float(fx),error))return false;++settings_initialize_phase_;}
 if(settings_initialize_phase_==1){if(!set_source_volume_v68(2,float(music),error))return false;++settings_initialize_phase_;}
 if(settings_initialize_phase_==2){original_vox_manager_general_init_v40(general_);++settings_initialize_phase_;}
 error.clear();return true;
}
bool AudioApplicationManagerV42::get_source_volume_v68(int selector,float&percent,std::string&error){
 if(!producer(error)||closing_){if(error.empty())error="Required live GetSoundVolume receiver";return false;}
 if(disabled_){percent=1.f;error.clear();return true;} // exact369b5c platform branch
 const char*label=selector==1?"SFX":selector==2?"MUSIC":selector==3?"VFX":nullptr;
 std::uint32_t group{};auto*runtime=runtime_on_producer();
 if(!runtime||(label&&!runtime->catalog().group_mask(label,group))){error="Required actual selected group mask getter";return false;}
 // Preserve original GetGroupMask -> GetGroupGain(int) asymmetry: do not
 // replace the original returned integer with a convenient slider value.
 percent=group<source_group_gain_.size()?source_group_gain_[group]*100.f:0.f;error.clear();return true;
}
bool AudioApplicationManagerV42::source_general_v68(OriginalVoxGeneralV40&out,bool&completed,std::string&error){
 if(!producer(error)||closing_){if(error.empty())error="Required live source general owner";return false;}
 out=general_;completed=initialize_complete_v100_&&settings_initialize_phase_==3;error.clear();return true;
}
bool AudioApplicationManagerV42::source_bus_volumes_v68(std::array<float,3>&out,std::string&error){
 if(!producer(error)||closing_||!initialize_complete_v100_||settings_initialize_phase_!=3){if(error.empty())error="Required whole source Initialize and reached group setters";return false;}
 auto*runtime=runtime_on_producer();if(!runtime){error="Required actual group catalog";return false;}
 std::array<bool,3>found{};
 for(const auto&group:runtime->catalog().groups()){if(group.uid<0||group.uid>=32||group.volume_group<0||group.volume_group>2){error="Original group gain domain";return false;}
  const auto bus=unsigned(group.volume_group);if(found[bus]&&out[bus]!=source_group_gain_[group.uid]){error="Required per-group transport for divergent source gains";return false;}
  out[bus]=source_group_gain_[group.uid];found[bus]=true;}
 if(!found[0]||!found[1]||!found[2]){error="Required all actual source volume groups";return false;}
 error.clear();return true;
}
bool AudioApplicationManagerV42::initialize_source_v100(const AudioInitializeServicesV100&s,std::string&error){
 if(!producer(error))return false;
 if(closing_){error="Process audio Initialize deferred: captured SoundManager is closing; complete serial-guarded shutdown before startup";return false;}
 if(!s.actual_application){error="Required retained actual App/Initialize receiver";return false;}
 if(initialize_busy_v100_){error="Original process audio Initialize reentry";return false;}
 if(initialize_attempted_v100_){
  if(initialize_complete_v100_&&initialize_application_v100_.lock()==s.actual_application){error.clear();return true;}
  error=initialize_error_v100_.empty()?"Source audio Initialize prefix cannot replay/replace App":initialize_error_v100_;return false;
 }
 initialize_attempted_v100_=initialize_busy_v100_=true;initialize_application_v100_=s.actual_application;
 struct Exit{bool&busy;~Exit(){busy=false;}}exit{initialize_busy_v100_};
 auto body=[&](){
  music_fields_.field_33=0;music_title38_v100_.clear();
  // Shipping Android Device.IsiOSVersionSupported("3.2")381744 returns1.
  // The zero branch would store33=0; no source iPod state is invented here.
  std::int32_t value{};
  if(!s.saved_option||!s.saved_option("VolumeFX",value,error)){if(error.empty())error="Required process Application.GetSavedOption(VolumeFX)";return false;}
  auto volume=[&](int selector,float percent){
   if(!disabled_)return set_source_volume_v68(selector,percent,error);
   if(!s.platform_volume){error="Required native platform volume setter during Initialize";return false;}
   return s.platform_volume(selector,percent,error);
  };
  if(!volume(1,float(value)))return false;
  if(!s.saved_option("VolumeMusic",value,error)||!volume(2,float(value)))return false;
  auto*runtime=runtime_on_producer();
  if(!runtime||!runtime->source_static_bus_routing_v100("/app_home/data/routing/template_dhpsn.vrt",error))return false;
  original_vox_manager_general_init_v40(general_);settings_initialize_phase_=3;
  if(!runtime->initialize_priority_banks_v100(error))return false;
  if(disabled_){error.clear();return true;}
  bool preload{};
  if(!s.high_performance||!s.high_performance(preload,error)){if(error.empty())error="Required actual Device.IsHighPerformance during Initialize";return false;}
  auto reached_flag=[&](const auto&getter,const char*name){
   if(!getter||!getter(preload,error)){if(error.empty())error=std::string("Required actual ")+name;return false;}return true;
  };
  if(!preload){
   if(!reached_flag(s.htc_devices,"HTC_DEVICES"))return false;
   if(!preload&&!reached_flag(s.sharp_devices,"SHARP_DEVICES"))return false;
   if(!preload&&!reached_flag(s.multiplayer_mode,"Is_In_Multiplayer_Mode"))return false;
  }
  if(!preload){error.clear();return true;}
  const auto event=runtime->catalog().event_uid("preload_sfx");
  if(event<0){error.clear();return true;} // source event size<=0 returns literally
  const auto count=runtime->catalog().events()[std::size_t(event)].remaining.size();
  AudioRandomV34 random{nullptr,[](void*,int&draw){draw=std::rand();return draw>=0;}};
  for(std::size_t i=0;i<count;++i){
   int uid=-1;if(!runtime->select_source_event_v100(event,random,uid,error))return false;
   bool trace{};
   if(!s.debug||!s.debug("isTracingPreload_SFX",trace,error)){if(error.empty())error="Required actual preload Debug.load/GetSwitch";return false;}
   preload_uids_v100_.push_back(uid); // SAME source vector store precedes LoadSound
   if(!precache_raw_uid(uid,error))return false;
  }
  error.clear();return true;
 };
 try{if(body()){initialize_complete_v100_=true;error.clear();return true;}}
 catch(const std::exception&ex){error=ex.what();}
 initialize_error_v100_=error.empty()?"Required reached whole source Vox Initialize body":error;error=initialize_error_v100_;return false;
}
bool AudioApplicationManagerV42::precache_raw_uid(int uid,std::string&error){
 if(!producer(error)||closing_){if(error.empty())error="Application SoundManager closing";return false;}
 if(disabled_||uid<0){error.clear();return true;}
 auto*runtime=runtime_on_producer();
 if(!runtime||!runtime->source_data_initialized()){error="Required constructed exact selected soundpack";return false;}
 const auto&catalog=runtime->catalog();const auto bound=catalog.sounds().size();
 if(std::size_t(uid)>bound){error.clear();return true;}
 // Original <=count permits the endpoint, but a missing metadata row cannot
 // be dereferenced safely. This remains required, never a fabricated slot.
 const auto*metadata=catalog.sound(uid);
 if(!metadata){error="Required actual source soundpack metadata row for raw UID";return false;}
 if(std::size_t(uid)>runtime->catalog().sounds().size()){error.clear();return true;}
 if(runtime->source_slot_identity_v101(uid)){error.clear();return true;}
 auto sample=runtime->load_sample_actual_xml_uid(uid,error);
 if(!sample){
  //Initialize36c2e4 discards LoadSound's return. A genuine missing file
  //retains the original invalid-handle slot; it is not a ready datasource.
  if(runtime->source_slot_unavailable_v100(uid)){error.clear();return true;}
  return false;
 }
 error.clear();return true; // SAME runtime bank now owns the sole UID slot.
}
bool AudioApplicationManagerV42::submit_actual_play(const dh2::character::CombatSoundPlayV1&play,std::int64_t event,std::string&error){
 if(!producer(error)||closing_||!session_){if(error.empty())error="Application SoundManager closing/unavailable";return false;}
 return session_->submit_actual_play(play,event,error);
}
bool AudioApplicationManagerV42::set_music_state_v101(const char*name,std::string&error){
 if(!producer(error)||closing_){if(error.empty())error="Required live source SetMusicState receiver";return false;}
 struct Frame {
  AudioApplicationManagerV42& manager;AudioGameplayRuntimeV42* runtime{};
  std::uintptr_t source_owner{};std::string diagnostic;
  struct Info {std::uintptr_t slot{};int uid{-1};std::uint64_t token{};} info;
  bool info_live{};
  static int invoke(void*raw,const dh2::sound::VoxMusicRequestV1*q,dh2::sound::VoxMusicResponseV1*out){
   auto&self=*static_cast<Frame*>(raw);using namespace dh2::sound;
   if(!q||!out||q->owner!=self.source_owner)return -1;*out={};
   if(q->service==vox_music_disabled_v1){out->integer=self.manager.disabled();return 0;}
   if(!self.runtime){self.diagnostic="Required SAME runtime on positive SetMusicState";return -1;}
   if(q->service==vox_music_event_index_v1){const auto*row=self.runtime->bindings().row(q->music);
    if(!row){self.diagnostic="Required actual generated current music row";return -1;}out->integer=row->uid;return 0;}
   if(q->service==vox_music_channel_v1){out->identity=self.runtime->source_slot_identity_v101(q->sound_index);return 0;}
   if(q->service==vox_music_channel_info_v1){
    if(self.info_live||!q->channel||q->channel!=self.runtime->source_slot_identity_v101(q->sound_index)){self.diagnostic="Required SAME source datasource/ChannelInfo receiver";return -1;}
    std::uint64_t token{};if(!self.runtime->first_source_channel_v101(q->sound_index,token,self.diagnostic))return -1;
    self.info={q->channel,q->sound_index,token};self.info_live=true;out->identity=reinterpret_cast<std::uintptr_t>(&self.info);out->integer=token?1:0;return 0;
   }
   if(!self.info_live||q->info!=reinterpret_cast<std::uintptr_t>(&self.info)||q->channel!=self.info.slot){self.diagnostic="Expired or foreign actual ChannelInfo borrow";return -1;}
   if(q->service==vox_music_channel_state_v1){return self.runtime->native_state_owned_v101(self.info.token,q->state,self.diagnostic)?0:-1;}
   if(q->service==vox_music_channel_info_destroy_v1){self.info_live=false;self.info={};return 0;}
   self.diagnostic="Unexpected source music service";return -1;
  }
 } frame{*this,runtime_on_producer(),0,{}, {},false};
 dh2::sound::VoxMusicStateOwnerV1 source(music_fields_,{&frame,Frame::invoke});frame.source_owner=source.identity();
 const auto status=source.set_music_state(name);
 if(status){error=frame.diagnostic.empty()?source.error():frame.diagnostic;if(error.empty())error="Required source SetMusicState argument";return false;}
 error.clear();return true;
}
std::shared_ptr<AudioNativeSessionV42> AudioApplicationManagerV42::session()const{std::lock_guard<std::mutex>lock(session_mutex_);return session_;}
bool AudioApplicationManagerV42::stop_sound_v106(int sound,int fade,
 const std::function<bool(int,std::string&)>&platform_stop,std::string&error){
 return stop_3d_v112(sound,fade,nullptr,0.f,platform_stop,error);
}
bool AudioApplicationManagerV42::stop_3d_v112(int sound,int fade,const float* center,float radius,
 const std::function<bool(int,std::string&)>&platform_stop,std::string&error){
 if(sound<0){error.clear();return true;} // before source platform/data reads
 if(!producer(error)||closing_){if(error.empty())error="Required SAME live per-source Stop manager";return false;}
 if(disabled_){if(!platform_stop){error="Required actual nativeStopSoundBig platform endpoint";return false;}return platform_stop(sound,error);}
 auto*runtime=runtime_on_producer();if(!runtime){error="Required SAME source Stop runtime";return false;}
 return runtime->stop_source_3d_v112(sound,fade,center,radius,error);
}
void AudioApplicationManagerV42::request_output_close(){closing_.store(true,std::memory_order_release);auto owner=session();if(owner)owner->request_output_close();}
bool AudioApplicationManagerV42::shutdown(std::string&error){
 if(!producer(error))return false;
 closing_=true;auto owner=session();
 if(owner&&!owner->shutdown(error))return false;
 {std::lock_guard<std::mutex>lock(session_mutex_);session_.reset();}error.clear();return true;
}
AudioGameplayRuntimeV42* AudioApplicationManagerV42::runtime_on_producer()noexcept{auto owner=session();return owner?owner->runtime_on_producer():nullptr;}
std::string AudioApplicationManagerV42::control_error()const{auto owner=session();return owner?owner->control_error():std::string{};}
}

namespace dh2::audio {
bool AudioApplicationManagerV42::source_producer_ready_v94(std::string& error)const{
 if(!producer(error)||closing_){if(error.empty())error="Required live SAME process audio producer";return false;}
 error.clear();return true;
}
}
