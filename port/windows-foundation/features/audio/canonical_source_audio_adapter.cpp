#include "canonical_source_audio_adapter.hpp"
#include "../../../engine-audio/audio_named_animation_sound_v38.hpp"
#include <cstring>
namespace dh::foundation::audio {
namespace {
bool ignored_source_audio_operation(const std::string& error){
 // These failures are returned by a reached, initialized source audio
 // operation. Original Script_PlaySound::Execute is void and its dispatcher
 // does not consume Play's result; retain diagnostics without vetoing script
 // progression. Required-provider/gate errors deliberately do not match.
 return error.rfind("Unavailable original audio asset: ",0)==0||
  error=="Audio voice observation capacity; drain producer receipts"||
  error=="Audio source command queue full"||
  error=="Source audio token exhaustion";
}
}
bool CanonicalSourceAudioAdapter::named_sound(const RetainedAnimationEvent& event,std::uintptr_t actor,std::uint64_t ordinal,std::int64_t frame_ns,std::string& error){if(event.name.rfind("sfx_",0)!=0)return true;Key key{event.generation,actor,event.clip_id,event.slot,event.wall_timestamp_ms,event.lag_ms,ordinal};if(delivered_.count(key))return true;
 const auto lag=std::int64_t(event.lag_ms)*1000000LL;if(frame_ns<=0||(lag>=0&&frame_ns<=lag)||(lag<0&&frame_ns>INT64_MAX+lag)){error="Required actual authored monotonic named-sound time";return false;}const auto event_ns=frame_ns-lag;
 struct Context {CanonicalSourceAudioAdapter* self;std::string* error;std::int64_t event_ns;} context{this,&error,event_ns};dh2::audio::AudioNamedAnimationSoundServicesV38 s;s.context=&context;s.actual_character=actor;
 s.manager=[](void* p,std::uintptr_t& identity){auto& c=*static_cast<Context*>(p);if(!c.self->leaves_.play3d_authorities_ready||!c.self->leaves_.play3d_authorities_ready(*c.error)){if(c.error->empty())*c.error="Required actual World/GS/Level and same-Level Vox command authorities";return -1;}identity=c.self->runtime_.manager();return identity?0:-1;};
 s.target_position=[](void* p,std::uintptr_t actual,std::array<float,3>& position){auto& c=*static_cast<Context*>(p);return c.self->leaves_.target_position&&c.self->leaves_.target_position(actual,position,*c.error)?0:-1;};
 s.play=[](void* p,const dh2::character::CombatSoundPlayV1& play){auto& c=*static_cast<Context*>(p);return c.self->runtime_.submit_actual_play(play,c.event_ns,*c.error)?0:-1;};
 dh2::audio::AudioNamedAnimationSoundResultV38 result;if(dh2::audio::audio_named_animation_sound_v38(event.name.c_str()+4,runtime_.bindings(),s,result)){
  if(ignored_source_audio_operation(error)){if(leaves_.audio_diagnostic)leaves_.audio_diagnostic(error);delivered_.emplace(std::move(key),0);error.clear();return true;}
  if(error.empty())error=result.required?result.required:"Actual source named sound failed";return false;
 }
 delivered_.emplace(std::move(key),result.source_id>=0?runtime_.last_token():0);return true;
}
bool CanonicalSourceAudioAdapter::campaign(CampaignCommandPhase phase,const OriginalCampaignCommand& c,bool received,std::int64_t ns,bool& handled,bool& blocking,std::string& error){handled=c.kind==13||c.kind==14;blocking=false;if(!handled||phase!=CampaignCommandPhase::execute)return true;
 auto field=[&](unsigned offset,std::uint32_t& value){auto i=c.scalars.find(offset);if(i==c.scalars.end()){error="Required exact original sound command field "+std::to_string(offset);return false;}value=i->second;return true;};std::uint32_t mode{},arg{},source{};if(!field(12,mode)||!field(8,arg)||!field(16,source))return false;if(c.kind==13&&received&&!mode)return true;
 if(!leaves_.trace_script){error="Required actual isTracingScriptCmd query";return false;}if(!leaves_.trace_script(error))return false;int id{},argument{};std::memcpy(&id,&source,4);std::memcpy(&argument,&arg,4);
 if(c.kind==14){if(mode){if(!leaves_.stop_music){error="Required SAME original StopMusic owner";return false;}return leaves_.stop_music(argument,error);}
  if(id<0)return true;if(!runtime_.source_data_initialized()){error="Required initialized SAME source Vox runtime for Stop";return false;}
  if(!runtime_.bindings().row(id)){error="Required exact original Stop source ordinal";return false;}
  if(runtime_.stop_source_sound_v106(id,argument,error))return true;
  // ScriptManager ignores Script_StopSound::Execute's return and this command
  // is nonblocking. A reached Stop that cannot enqueue/drain a native audio
  // control is diagnostic-only; absent runtime/row providers above stay fatal.
  if(error.find("command queue full")!=std::string::npos||error.find("receipt drain")!=std::string::npos){if(leaves_.audio_diagnostic)leaves_.audio_diagnostic(error);error.clear();return true;}
  return false;
 }
 std::uint32_t flag{};if(!field(13,flag))return false;if(mode){if(!leaves_.play_music){error="Required SAME original PlayMusic owner";return false;}return leaves_.play_music(id,flag!=0,argument!=0,2000,error);}if(id<0)return true;
 if(!runtime_.source_data_initialized()){error="Required initialized SAME source Vox runtime for regular Play";return false;}
 if(!leaves_.plain_command){error="Required whole original regular Play emitter/property authority";return false;}
 if(!runtime_.bindings().row(id)){error="Required exact original regular Play source ordinal";return false;}
 if(runtime_.submit_plain_source(id,ns,[&](const auto& sound,const auto& group,auto& command,std::string& e){return leaves_.plain_command(id,flag!=0,argument,0,false,sound,group,command,e);},error))return true;
 // Original VoxSoundManager::Play returns 0 after a missing/not-ready
 // datasource skips emitter creation. Script_PlaySound::Execute is void and
 // the script dispatcher ignores Execute's return, so an absent authored cue
 // is diagnostic-only once the actual source runtime and root leaves exist.
 if(ignored_source_audio_operation(error)){if(leaves_.audio_diagnostic)leaves_.audio_diagnostic(error);error.clear();return true;}
 return false;
}
bool CanonicalSourceAudioAdapter::retire(std::uint64_t generation,std::uintptr_t actor,std::string& error){for(auto i=delivered_.begin();i!=delivered_.end();){if(std::get<0>(i->first)!=generation||std::get<1>(i->first)!=actor){++i;continue;}if(i->second&&runtime_.voice(i->second)){dh2::audio::AudioCommandV34 stop;stop.kind=dh2::audio::AudioCommandKindV34::stop;stop.token=i->second;if(!runtime_.mixer().post(stop)){error="Original owned audio retirement queue full";return false;}}i=delivered_.erase(i);}return true;}
void CanonicalSourceAudioAdapter::observe_receipt(const dh2::audio::AudioReceiptV34& r){if(r.kind==dh2::audio::AudioReceiptKindV34::started||r.kind==dh2::audio::AudioReceiptKindV34::control_required)return;for(auto& i:delivered_)if(i.second==r.token)i.second=0;}
}
