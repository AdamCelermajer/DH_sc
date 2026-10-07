#include "audio_campaign_bridge_v46.hpp"
#include <cmath>
namespace dh2::audio {
namespace {thread_local std::int64_t authored_event_ns_v46{};}
AudioAuthoredEventScopeV46::AudioAuthoredEventScopeV46(std::int64_t value):previous_(authored_event_ns_v46){authored_event_ns_v46=value>0?value:0;}
AudioAuthoredEventScopeV46::~AudioAuthoredEventScopeV46(){authored_event_ns_v46=previous_;}
std::int64_t AudioAuthoredEventScopeV46::current()noexcept{return authored_event_ns_v46;}
bool AudioCampaignBridgeV46::current(std::shared_ptr<dh2::loader::CanonicalLevelContextV1>&out,std::string&error){
 if(std::this_thread::get_id()!=producer_){error="Required same campaign audio producer";return false;}
 if(!services_.current_level){error="Required actual Application/GS current Level getter";return false;}
 if(!services_.current_level(out,error))return false;
 if(out&&out!=services_.selected_level){error="Audio campaign provider belongs to another selected Level";return false;}
 return true;
}
std::shared_ptr<AudioCampaignBridgeV46> AudioCampaignBridgeV46::publish(AudioCampaignServicesV46 services,std::string&error){
 if(!services.actual_world||!services.actual_gs||!services.selected_level||!services.current_level||
    !services.settings_owner||!services.rng_owner||!services.camera_owner||!services.actual_debug||
    !services.actual_online||
    !services.actual_trace||!services.actual_general||!services.actual_emitter_modifiers||!services.actual_group_volumes||
    !services.actual_update_listener||!services.actual_vox_random.next||!services.exact_legacy_listener_stream){
  error="Required complete selected campaign World/GS/settings/RNG/listener provider lease";return {};
 }
 auto owner=std::make_shared<AudioCampaignBridgeV46>();owner->services_=std::move(services);
 if(!dh2::android_audio::borrow_application_audio_v42(owner->manager_,error)||!owner->manager_.manager)return {};
 auto*runtime=owner->manager_.manager->runtime_on_producer();
 if(!runtime||!runtime->source_data_initialized()){error="Required SAME constructed Application soundpack";return {};}
 const auto&stream=*owner->services_.exact_legacy_listener_stream;
 if(!audio_listener_rows_v38(stream.data(),stream.size(),owner->listeners_,error))return {};
 owner->epoch_=owner->services_.selected_level->identity();
 if(!owner->epoch_){error="Required actual selected Level identity/epoch";return {};}
 AudioGameplaySourcesV40 sources;sources.context=owner.get();sources.gates={owner.get(),gates};
 sources.random={owner.get(),random};sources.source_command=command;
 if(!dh2::android_audio::publish_actual_playback_v42(sources,owner,error))return {};
 error.clear();return owner;
}
int AudioCampaignBridgeV46::gates(void*raw,const dh2::sound::VoxPlay3DRequestV2&q,dh2::sound::VoxPlay3DResponseV2&out){
 auto&self=*static_cast<AudioCampaignBridgeV46*>(raw);using O=dh2::sound::VoxPlay3DOperationV2;
 if(!q.play||q.play->manager!=self.manager_.identity())return -1;
 bool value{};std::string error;
 if(q.operation==O::disabled){
  if(!self.services_.actual_debug("IsDisablingSounds",value,error)){self.failure_=error;return -1;}
  out.value=value;return 0;
 }
 if(q.operation==O::current_level){
  std::shared_ptr<dh2::loader::CanonicalLevelContextV1>level;
  if(!self.current(level,error)){self.failure_=error;return -1;}
  out.identity=level?level->identity():0;out.value=0;
  if(level){dh2::loader::CanonicalLevelContextV1::LoadingFieldsV26 fields;
   if(!level->loading_fields_v26(fields,error)||!fields.state130){self.failure_=error.empty()?"Required actual Level loading field":error;return -1;}
   out.value=std::int32_t(*fields.state130);
  }
  return 0;
 }
 auto flag=[&](const auto&fn){if(!fn||!fn(value,error)){self.failure_=error.empty()?"Required actual Vox flag provider":error;return -1;}out.value=value;return 0;};
 if(q.operation==O::online)return flag(self.services_.actual_online);
 if(q.operation==O::network_muted)return flag(self.services_.actual_network_muted);
 // Original platform branch reads9f6409, the SAME process flag used by
 // StopAll/LoadSound and owned by the actual Application manager.
 if(q.operation==O::platform_route){out.value=self.manager_.manager->disabled();return 0;}
 if(q.operation==O::platform_play){if(!self.services_.actual_platform_play||!self.services_.actual_platform_play(*q.play,error)){self.failure_=error.empty()?"Required actual platform sound route":error;return -1;}return 0;}
 if(q.operation==O::trace){if(!self.services_.actual_trace(*q.play,q.selected_sound,error)){self.failure_=error;return -1;}return 0;}
 self.failure_="Unexpected campaign Vox gate operation";return -1;
}
bool AudioCampaignBridgeV46::random(void*raw,int&value){
 auto&self=*static_cast<AudioCampaignBridgeV46*>(raw);
 return self.services_.actual_vox_random.next(self.services_.actual_vox_random.context,value);
}
bool AudioCampaignBridgeV46::command(void*raw,const character::CombatSoundPlayV1&play,const AudioSoundV34&sound,
 const AudioGroupV34&group,AudioCommandV34&out,std::string&error){
 auto&self=*static_cast<AudioCampaignBridgeV46*>(raw);std::shared_ptr<dh2::loader::CanonicalLevelContextV1>level;
 if(!self.current(level,error)||!level){if(error.empty())error="Required actual current Level listener";return false;}
 const auto index=level->constructor_fields_v3().phase_e4;
 const auto&cached=self.manager_.manager->listener_row_on_producer();
 if(!cached&&index>=self.listeners_.size()){error="Actual Level listener row outside source table";return false;}
 const auto&row=cached?*cached:self.listeners_[index];float position[3]{},front[3]{},up[3]{};
 if(!self.services_.actual_update_listener(row,position,front,up,error))return false;
 AudioSourceCommandAuthorityV40 authority;authority.manager=play.manager;authority.actual_current_level=level->identity();
 authority.world_epoch=authority.listener_world_epoch=self.epoch_;
 authority.soundpack_initialized=self.manager_.manager->runtime_on_producer()->source_data_initialized();
 if(!self.services_.actual_general(authority.general,authority.manager_general_initialized,error))return false;
 if(!authority.manager_general_initialized){error="Required completed original Vox Initialize/settings operation";return false;}
 if(!audio_listener_update_v38(row,position,front,up,authority.listener,error))return false;
 authority.listener_initialized=true;
 if(!self.services_.actual_emitter_modifiers(play,sound,group,authority.actual_gain,authority.actual_pitch,authority.actual_native_initial_state,error))return false;
 // Group setters in the source modifier prefix affect existing voices too.
 // Keep emitter gain separate; post changed actual bus values before play.
 std::array<float,3>volumes{};
 if(!self.services_.actual_group_volumes(volumes,error))return false;
 auto*runtime=self.manager_.manager->runtime_on_producer();
 for(unsigned i=0;i<volumes.size();++i){
  if(!std::isfinite(volumes[i])||volumes[i]<0||volumes[i]>1){error="Required actual source group volume within mixer capacity";return false;}
  if(!self.volumes_posted_||volumes[i]!=self.posted_volumes_[i]){
   AudioCommandV34 volume;volume.kind=AudioCommandKindV34::volume;volume.volume_group=int(i);volume.left=volumes[i];
   if(!runtime->mixer().post(volume)){error="Actual group volume command queue full after source modifier prefix";return false;}
   self.posted_volumes_[i]=volumes[i];
  }
 }
 self.volumes_posted_=true;
 out.source_dsp_bus=group.bus.c_str();
 AudioDeviceClockV40 actual_clock;
 if(!runtime->clock().snapshot(actual_clock)||!actual_clock.ready||!actual_clock.rate){error="Required actual output rate for PlaySoundPackSound source fade";return false;}
 out.fade_frames=actual_clock.rate/20; // source Play(.05), distinct from regular Play(ms)
 return audio_source_command_v40(authority,play,sound,group,out,error);
}
bool AudioCampaignBridgeV46::submit(AudioCategoryV46,const character::CombatSoundPlayV1&play,std::int64_t event,std::string&error){
 if(std::this_thread::get_id()!=producer_){error="Required actual authored audio producer thread";return false;}
 if(play.manager!=manager_.identity()){error="Required SAME process Application manager";return false;}
 failure_.clear();
 const bool delivered=manager_.manager->submit_actual_play(play,event,error);
 if(!delivered&&!failure_.empty())error+="; "+failure_;
 return delivered;
}
bool AudioCampaignBridgeV46::source_prefix_without_clock(const character::CombatSoundPlayV1&play,std::string&error){
 if(std::this_thread::get_id()!=producer_||play.manager!=manager_.identity()){error="Required SAME source prefix producer/manager";return false;}
 failure_.clear();dh2::sound::VoxPlay3DOwnerV2 prefix({this,gates});
 if(prefix.play(play)){error=failure_.empty()?prefix.error():failure_;return false;}
 error.clear();return true;
}
bool AudioCampaignBridgeV46::take_receipt(AudioReceiptV34&receipt){
 if(std::this_thread::get_id()!=producer_)return false;
 auto*runtime=manager_.manager->runtime_on_producer();if(!runtime)return false;
 runtime->pump_receipts();return runtime->take_receipt(receipt);
}
bool AudioCampaignBridgeV46::native_music_state(std::uint64_t token,const char*name,std::string&error){
 if(std::this_thread::get_id()!=producer_){error="Required actual music control producer";return false;}
 auto*runtime=manager_.manager->runtime_on_producer();
 if(!runtime){error="Required SAME active music runtime";return false;}
 return runtime->native_state(token,name,error);
}
}
