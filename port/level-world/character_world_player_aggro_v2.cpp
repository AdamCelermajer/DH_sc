#include "character_world_player_aggro_v2.hpp"
namespace dh2::character {
CharacterWorldPlayerAggroV2::CharacterWorldPlayerAggroV2(WorldPlayerAggroBorrowV2 f,WorldPlayerAggroServicesV2 s,DebugSwitches& d,const DebugFileServices24& files):fields_(f),services_(s),debug_(d),files_(files){}
int CharacterWorldPlayerAggroV2::service(void* p,const PlayerAggroRequestV1* q,PlayerAggroResponseV1* out){
 auto& w=*static_cast<CharacterWorldPlayerAggroV2*>(p);if(!q||!out||q->player!=w.fields_.player)return -1;*out={};
 switch(q->service){
 case player_aggro_online_v1:{bool value=false;if(!w.services_.online||w.services_.online(w.services_.context,&value))return -1;out->word=value;return 0;}
 case player_aggro_local_player_v1:{bool value=false;if(!w.services_.local_player||w.services_.local_player(w.services_.context,q->player,&value))return -1;out->word=value;return 0;}
 case player_aggro_current_level_v1:return w.services_.level?w.services_.level(w.services_.context,&out->level):-1;
 case player_aggro_other_weight_v1:return w.services_.weight?w.services_.weight(w.services_.context,q->other,&out->integer):-1;
 case player_aggro_threshold_v1:return w.services_.threshold?w.services_.threshold(w.services_.context,&out->integer):-1;
 case player_aggro_sound_v1:{if(!w.fields_.music)return -1;auto& f=w.fields_.music->fields();out->sound={w.fields_.music->identity(),&f.ambient_31,&f.level_music_32};return 0;}
 case player_aggro_set_music_state_v1:{if(!w.fields_.music||!q->name)return -1;const auto status=w.fields_.music->set_music_state(q->name);if(status)w.error_=w.fields_.music->error();return status;}
 case player_aggro_play_music_v1:return w.services_.play_music?w.services_.play_music(w.services_.context,q):-1;
 case player_aggro_trace_v1:return w.services_.trace?w.services_.trace(w.services_.context,q):-1;
 default:return -1;
 }
}
int CharacterWorldPlayerAggroV2::ais(void* p,AIEventState64* ai,std::uintptr_t method,std::uintptr_t other){
 auto& w=*static_cast<CharacterWorldPlayerAggroV2*>(p);
 if(ai!=w.fields_.events||!ai||!ai->ai||!ai->owner||ai->owner->owner!=w.fields_.player||!ai->active||!w.fields_.selected_fields||(method!=0x3ddb70&&method!=0x3dde48)){
  w.error_="Required exact selected AISPlayer Aggro receiver/fields";return -1;
 }
 CharacterPlayerAggroOwnerV1 selected(ai->active,w.fields_.player,*w.fields_.selected_fields,w.debug_,w.files_,{&w,service});
 const auto status=method==0x3dde48?selected.on_deaggro(other):selected.on_aggro(other);if(status){const auto detail=w.error_;w.error_=selected.error();if(!detail.empty())w.error_+="; "+detail;}return status;
}
int CharacterWorldPlayerAggroV2::notify(std::uint32_t bit,std::uintptr_t owner,std::uintptr_t target){
 error_.clear();if(!owner||!target)return -1;
 if(bit!=data::aggro_notify_target&&bit!=data::aggro_notify_target_cleared){
  if(services_.remaining)return services_.remaining(services_.context,bit,owner,target);
  error_="Required original aggro continuation "+std::to_string(bit);return -2;
 }
 AIEventState64* receiver=nullptr;
 if(!services_.events||services_.events(services_.context,target,&receiver)||!receiver||!receiver->ai||!receiver->owner||receiver->owner->owner!=target){error_="Required SAME registered target CharAI event owner";return -2;}
 std::string detail;
 const auto status=bit==data::aggro_notify_target_cleared?
  world_deaggro_event_v2(*receiver,owner,&debug_,&files_,{this,ais},detail):
  world_aggro_event_v1(*receiver,owner,&debug_,&files_,{this,ais},detail);
 if(status){if(!error_.empty())detail+="; "+error_;error_=detail;}
 return status;
}
}
