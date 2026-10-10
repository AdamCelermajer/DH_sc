#include "source_audio_runtime.hpp"
#include "../../../engine-audio/audio_named_animation_sound_v38.hpp"
#include <cstring>
#include <limits>
#include <algorithm>
namespace dh::foundation::audio {
bool SourceAudioRuntime::named_sound(const RetainedAnimationEvent& event,std::uint64_t producer,std::uint64_t occurrence,std::string& error){
 if(event.name.rfind("sfx_",0)!=0)return true;
 EventIdentity identity{event.generation,producer,event.clip_id,event.slot,event.wall_timestamp_ms,event.lag_ms,occurrence};auto known=event_occurrences_.find(identity);if(known==event_occurrences_.end()){if(!next_occurrence_){error="Audio occurrence identity exhaustion";return false;}known=event_occurrences_.emplace(std::move(identity),next_occurrence_++).first;}occurrence=known->second;
 struct Context {SourceAudioRuntime* self;const RetainedAnimationEvent* event;std::uint64_t producer,occurrence;std::string* error;} context{this,&event,producer,occurrence,&error};
 dh2::audio::AudioNamedAnimationSoundServicesV38 leaves;leaves.context=&context;leaves.actual_character=services_.actual_actor;
 leaves.manager=[](void* p,std::uintptr_t& m){m=static_cast<Context*>(p)->self->services_.actual_manager;return m?0:-1;};
 leaves.target_position=[](void* p,std::uintptr_t,std::array<float,3>& pos){auto& c=*static_cast<Context*>(p);auto& s=c.self->services_;return s.target_position&&s.target_position(pos,*c.error)?0:-1;};
 leaves.play=[](void* p,const dh2::character::CombatSoundPlayV1& play){auto& c=*static_cast<Context*>(p);auto& self=*c.self;auto row=self.bindings_.row(play.sound_id);if(!row)return -1;
  if(row->uid<0)return 0;const auto* sound=self.catalog_.sound(row->uid);if(row->event){*c.error="Required source Play3D event selection before emitter construction";return -1;}
  if(!sound){*c.error="Required actual named-sound catalog UID";return -1;}const auto* group=self.catalog_.group(sound->group);if(!group)return -1;
  if(!self.services_.prepare_3d||!self.services_.source_frame){*c.error="Required original Play3D authority and authored output-frame mapping";return -1;}
  SourceAudioRequest request;request.generation=c.event->generation;request.producer=c.producer;request.occurrence=c.occurrence;request.catalog_uid=row->uid;
  bool admitted{};if(!self.services_.prepare_3d(play,*sound,*group,request.command,admitted,*c.error))return -1;if(!admitted)return 0;
  const auto wall=std::int64_t(c.event->wall_timestamp_ms)-std::int64_t(c.event->lag_ms);
  if(!self.services_.source_frame(wall,request.command.start_frame,*c.error))return -1;
  std::uint64_t token{};if(!self.router_.submit(request,token,*c.error))return -1;self.generation_producers_[request.generation].insert(request.producer);auto& voices=self.generation_voices_[request.generation];if(token&&std::none_of(voices.begin(),voices.end(),[&](const auto& v){return v.second==token;}))voices.push_back({row->uid,token});return 0;
 };
 dh2::audio::AudioNamedAnimationSoundResultV38 result;const int status=dh2::audio::audio_named_animation_sound_v38(event.name.c_str()+4,bindings_,leaves,result);
 if(status&&error.empty())error=result.required?result.required:"Original named sound failed";return status==0;
}
bool SourceAudioRuntime::campaign(CampaignCommandPhase phase,const OriginalCampaignCommand& c,bool received,std::uint64_t generation,std::uint64_t producer,std::uint64_t occurrence,std::int64_t wall,bool& handled,bool& blocking,std::string& error){
 handled=c.kind==13||c.kind==14;blocking=false;if(!handled||phase!=CampaignCommandPhase::execute)return true;
 auto field=[&](unsigned offset,std::uint32_t& out){auto i=c.scalars.find(offset);if(i==c.scalars.end()){error="Required authored audio command field "+std::to_string(offset);return false;}out=i->second;return true;};
 std::uint32_t mode{},argument{},source{};if(!field(12,mode)||!field(8,argument)||!field(16,source))return false;
 if(c.kind==13&&received&&!mode)return true;
 if(!services_.trace_script){error="Required original isTracingScriptCmd query";return false;}if(!services_.trace_script(error))return false;
 int id{};std::memcpy(&id,&source,sizeof id);int arg{};std::memcpy(&arg,&argument,sizeof arg);
 if(c.kind==13){std::uint32_t flag{};if(!field(13,flag))return false;if(mode){if(!services_.play_music){error="Required original PlayMusic source owner";return false;}return services_.play_music(id,flag!=0,arg!=0,2000,error);}
  if(id<0)return true;const auto* row=bindings_.row(id);const auto* sound=row?catalog_.sound(row->uid):nullptr;const auto* group=sound?catalog_.group(sound->group):nullptr;if(!row||!sound||!group){error="Required generated regular Play row and exact catalog";return false;}
  if(!services_.prepare_plain||!services_.source_frame){error="Required original regular Play owner/frame mapping";return false;}
  SourceAudioRequest request;request.generation=generation;request.producer=producer;request.occurrence=occurrence;request.catalog_uid=row->uid;request.assign_catalog_group=false;bool admitted{};
  if(!services_.prepare_plain(id,flag!=0,arg,0,false,*sound,*group,request.command,admitted,error))return false;if(!admitted)return true;
  if(!services_.source_frame(wall,request.command.start_frame,error))return false;std::uint64_t token{};if(!router_.submit(request,token,error))return false;generation_producers_[generation].insert(producer);auto& voices=generation_voices_[generation];if(token&&std::none_of(voices.begin(),voices.end(),[&](const auto& v){return v.second==token;}))voices.push_back({row->uid,token});return true;
 }
 if(mode){if(!services_.stop_music){error="Required original StopMusic source owner";return false;}return services_.stop_music(arg,error);}
 if(id<0)return true;if(arg<0||!services_.output_rate||!services_.source_frame){error="Required original stop fade/output-frame authority";return false;}
 const std::uint64_t fade=std::uint64_t(arg)*services_.output_rate/1000;if(fade>UINT32_MAX){error="Original stop fade exceeds transport extent";return false;}std::uint64_t frame{};if(!services_.source_frame(wall,frame,error))return false;
 const auto* stop_row=bindings_.row(id);if(!stop_row){error="Required generated Stop sound row";return false;}unsigned stopped{};for(auto& g:generation_voices_)for(auto& voice:g.second)if(voice.first==stop_row->uid&&voice.second){if(stopped==10)return true;if(!router_.stop(voice.second,frame,std::uint32_t(fade),error))return false;++stopped;}return true;
}
bool SourceAudioRuntime::retire(std::uint64_t generation,std::int64_t wall,std::string& error){std::uint64_t frame{};if(!services_.source_frame||!services_.source_frame(wall,frame,error))return false;auto producers=generation_producers_.find(generation);if(producers!=generation_producers_.end())for(auto producer:producers->second)if(!router_.retire_producer_generation(generation,producer,frame,error))return false;generation_producers_.erase(generation);generation_voices_.erase(generation);for(auto i=event_occurrences_.begin();i!=event_occurrences_.end();)if(std::get<0>(i->first)==generation)i=event_occurrences_.erase(i);else ++i;return true;}
void SourceAudioRuntime::observe_receipt(const dh2::audio::AudioReceiptV34& receipt){if(receipt.kind==dh2::audio::AudioReceiptKindV34::started||receipt.kind==dh2::audio::AudioReceiptKindV34::control_required)return;for(auto& g:generation_voices_)g.second.erase(std::remove_if(g.second.begin(),g.second.end(),[&](const auto& v){return v.second==receipt.token;}),g.second.end());}
}
