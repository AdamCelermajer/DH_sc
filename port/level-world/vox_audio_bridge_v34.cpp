#include "vox_audio_bridge_v34.hpp"
namespace dh2::sound {
bool VoxAudioBridgeV34::bind_source_autogen(std::vector<audio::AudioSoundAutoGenV34>bindings,std::string&error){
 if(!manager_||bindings.empty()){error="Required actual Vox identity/source SoundAutoGen bindings";return false;}
 for(const auto&b:bindings){if(b.event<0||b.event>1||b.uid< -1){error="Source SoundAutoGen UID/event domain";return false;}if(b.uid<0)continue;if(b.event){if(std::size_t(b.uid)>=catalog_.events().size()){error="Required original source event UID";return false;}}else if(!catalog_.sound(b.uid)){error="Required original source sound UID";return false;}}
 bindings_=std::move(bindings);return true;
}
int VoxAudioBridgeV34::emit(const character::CombatSoundPlayV1&play,int uid,bool event){
 if(event){if(!catalog_.select_event(uid,authorities_.random,uid,error_))return -1;if(uid<0)return 0;}
 const auto*sound=catalog_.sound(uid);if(!sound){error_="Required same original sound UID";return -1;}const auto*group=catalog_.group(sound->group);if(!group){error_="Required same original sound group";return -1;}
 // Source event history/RNG and data-source loading precede driver readiness.
 // Preserve those prefix effects even when the modern output is unavailable.
 if(!bank_.load(uid)){error_=bank_.error();return -1;}
 if(!authorities_.output_ready||!authorities_.output_ready(authorities_.context,error_)){if(error_.empty())error_="Required actual opened/focused audio output";return -1;}
 audio::AudioCommandV34 command;
 if(!authorities_.source_command||!authorities_.source_command(authorities_.context,play,*sound,*group,command,error_)){if(error_.empty())error_="Required original source emitter/3D/music fields";return -1;}
 if(!authorities_.event_frame||!authorities_.event_frame(authorities_.context,command.start_frame,error_)){if(error_.empty())error_="Required real source event audio clock";return -1;}
 std::uint64_t token;if(!bank_.play_uid(uid,command,token)){error_=bank_.error();return -1;}return 0;
}
int VoxAudioBridgeV34::invoke(void*raw,const VoxPlay3DRequestV2&q,VoxPlay3DResponseV2&r){
 auto&self=*static_cast<VoxAudioBridgeV34*>(raw);using O=VoxPlay3DOperationV2;
 if(!q.play||q.play->manager!=self.manager_){self.error_="Required same source Vox manager identity";return -1;}
 if(q.operation==O::sound_row){if(q.play->sound_id<0||std::size_t(q.play->sound_id)>=self.bindings_.size()){self.error_="Required source SoundAutoGen sound row";return -1;}const auto&b=self.bindings_[q.play->sound_id];r.value=b.uid;r.type=b.event;return 0;}
 if(q.operation==O::bank_info){const auto*s=self.catalog_.sound(q.selected_sound);if(!s){self.error_="Required original selected soundpack UID";return -1;}
  // Native-sized typed catalog remains authority for filename; these cells
  // carry stable UID/format/bank/group/loading metadata across source trace.
  r.bank_fields[0]=s->uid;r.bank_fields[1]=s->format;r.bank_fields[2]=s->bank;r.bank_fields[3]=s->group;r.bank_fields[4]=s->loading_flags;return 0;}
 if(q.operation==O::emit)return self.emit(*q.play,q.selected_sound,false);
 if(q.operation==O::native_play)return self.emit(*q.play,q.selected_sound,true);
 if(!self.authorities_.gates.invoke){self.error_="Required original World/Vox gate operation "+std::to_string(unsigned(q.operation));return -1;}
 return self.authorities_.gates.invoke(self.authorities_.gates.context,q,r);
}
}
