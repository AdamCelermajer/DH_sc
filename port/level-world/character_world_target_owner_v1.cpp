#include "character_world_target_owner_v1.hpp"
#include <cstring>
namespace dh2::character::skills {
extern "C" const float* dh2_world_target_position_v1(const float* position,const float* cached,std::uintptr_t node,std::uint8_t enabled){return node&&enabled?cached:position;}
CharacterWorldTargetOwnerV1::CharacterWorldTargetOwnerV1(const target_search::Registry8& registry,const data::AiTables& ai,WorldTargetServicesV1 services):registry_(registry),ai_(ai),services_(services){
 for(const auto& row:ai.rows)types_.push_back(row.type);type_view_={types_.data(),std::uint32_t(types_.size()),0};
}
bool CharacterWorldTargetOwnerV1::actor(std::uintptr_t identity,WorldTargetActorBorrowV1& out){
 out={};if(!identity||!services_.actor||services_.actor(services_.context,identity,&out)||out.identity!=identity||!out.search||out.search->identity!=identity||!out.scene){error_="Required SAME live World actor/Scene borrow";return false;}
 if(out.character&&(!out.life||!out.character->resolved||out.character->identity!=identity||out.life->dead>255)){error_="Required SAME live Character/property/life borrow";return false;}
 if(out.character)out.character->dead1449=std::uint8_t(out.life->dead);
 return true;
}
int CharacterWorldTargetOwnerV1::query(std::uint32_t op,std::uintptr_t identity,std::uintptr_t other,std::int32_t& result){
 WorldTargetActorBorrowV1 a,b;if(!actor(identity,a))return -1;
 if(!a.character){
  unsigned base_op=op==target_providers::is_interactive?1:op==target_providers::interaction_type?2:op==target_providers::is_character?3:op==target_providers::is_zonable?4:0;
  if(!base_op||(base_op==4&&!a.base_byte2ed)){error_="Required genuine base GameObject target continuation";return -1;}
  return target_providers::dh2_gameobject_target_query(&result,base_op,a.base_byte2ed?*a.base_byte2ed:0)?-1:0;
 }
 if(other&&!actor(other,b))return -1;
 const target_providers::Services16 services{this,query_service};
 auto code=services_.character_flags?dh2_character_skill_target_query_live_flags_v6(&result,op,a.character,b.character,&type_view_,&services):dh2_character_skill_target_query_v6(&result,op,a.character,b.character,&type_view_,&services);
 if(code){if(error_.empty())error_="Required native Character target query continuation";return -1;}return 0;
}
int CharacterWorldTargetOwnerV1::query_service(void* context,const target_providers::Request24* q,std::uintptr_t* out){
 if(!q||!out)return -1;auto& w=*static_cast<CharacterWorldTargetOwnerV1*>(context);WorldTargetActorBorrowV1 a;
 if(!w.actor(q->subject,a))return -1;
 using namespace target_providers;
 if(q->service==live_state_flags){std::uint32_t flags;if(!w.services_.character_flags||w.services_.character_flags(w.services_.context,q->subject,&flags)){w.error_="Required SAME State56.flags for Character interaction";return -1;}*out=flags;return 0;}
 if(q->service==virtual_character){*out=a.character?1:0;return 0;}
 if(q->service==virtual_dead){if(!a.life){w.error_="Required genuine virtual IsDead owner";return -1;}*out=a.life->dead;return 0;}
 if(q->service==virtual_player){std::int32_t value;if(w.query(is_player,q->subject,0,value))return -1;*out=std::uint32_t(value);return 0;}
 if(q->service==ai_friend){if(!w.services_.friend_query){w.error_="Required original AI_IsFriend continuation";return -1;}return w.services_.friend_query(w.services_.context,q->subject,q->other,out);}
 if(q->service==ai_enemy){
  if(!w.services_.enemy_query){w.error_="Required whole AI_IsEnemy handle/kind/assertion continuation";return -1;}
  return w.services_.enemy_query(w.services_.context,q->subject,q->other,out);
 }
 w.error_="Required unknown native target service";return -1;
}
int CharacterWorldTargetOwnerV1::search_service(void* context,const target_search::Request24* q,target_search::Response16* out){
 if(!q||!out)return -1;auto& w=*static_cast<CharacterWorldTargetOwnerV1*>(context);*out={};using namespace target_search;
 if(q->service==resolve_character&&!q->subject)return 0;
 WorldTargetActorBorrowV1 a;if(!w.actor(q->subject,a))return -1;
 if(q->service==resolve_character){out->word=a.character?reinterpret_cast<std::uintptr_t>(a.search):0;return 0;}
 if(q->service==melee_radius||q->service==interaction_radius){
  if(a.character){const auto* row=data::ai_props(w.ai_,a.character->resolved[1]);if(!row){w.error_="Required actual AI radius row";return -1;}out->number=q->service==melee_radius?row->melee_radius:row->interact_radius;return 0;}
  if(q->service==interaction_radius&&a.aabb6)return target_providers::dh2_gameobject_interaction_radius(&out->number,a.aabb6)?-1:0;
  w.error_="Required actual GameObject radius owner";return -1;
 }
 if(q->service==is_enemy){target_providers::Request24 request{target_providers::ai_enemy,0,q->subject,q->other};return query_service(&w,&request,&out->word);}
 std::uint32_t op=0;switch(q->service){
 case is_player:op=target_providers::is_player;break;case is_dead:op=target_providers::is_dead;break;
 case is_interactive:op=target_providers::is_interactive;break;case interaction_type:op=target_providers::interaction_type;break;
 case is_zonable:op=target_providers::is_zonable;break;case is_character:op=target_providers::is_character;break;default:return -1;}
 std::int32_t value;if(w.query(op,q->subject,q->other,value))return -1;out->word=std::uintptr_t(std::intptr_t(value));return 0;
}
int CharacterWorldTargetOwnerV1::control_service(void* context,const CharacterControlRequest32* q,CharacterControlResponse16* out){
 if(!q||!out)return -1;auto& w=*static_cast<CharacterWorldTargetOwnerV1*>(context);WorldTargetActorBorrowV1 a;if(!w.actor(q->subject,a))return -1;
 if(q->service==control_target_position){
  if(!a.target_node||!a.position||(*a.target_node&&!a.target_enabled)){w.error_="Required exact GetTargetPosition source fields";return -1;}
  const auto* point=dh2_world_target_position_v1(a.position,a.cached_target_position,*a.target_node,*a.target_node?*a.target_enabled:0);
  if(!point){w.error_="Required actual target-node cache";return -1;}std::memcpy(out->position,point,12);return 1;
 }
 if(q->service==control_look_at_point){
  if(!a.position||!a.heading_angle||!a.controller_heading_angle){w.error_="Required actual Character/controller heading borrow";return -1;}
  LookAtState16 state{{a.position[0],a.position[1],a.position[2]},*a.heading_angle};
  if(dh2_character_look_at_point(&state,q->position))return -1;
  *a.heading_angle=*a.controller_heading_angle=state.heading_angle;return 1;
 }
 w.error_="Required genuine move/stop controller continuation";return -1;
}
int CharacterWorldTargetOwnerV1::look_at(std::uintptr_t character,std::uintptr_t target){
 error_.clear();ControllerCommandState32 controller{};
 if(!services_.controller||services_.controller(services_.context,character,&controller)||controller.owner!=character){error_="Required SAME controller command state";return -1;}
 const CharacterControlServices16 services{this,control_service};
 const auto code=dh2_character_controller_character(&controller,controller_look_object,target,&services);
 if(code!=1){if(error_.empty())error_="Required Cmd_LookAt continuation";return -1;}return 0;
}
SkillNativeWorldV5 CharacterWorldTargetOwnerV1::native_world(std::uintptr_t player,const dh2_script_object_services* objects){
 WorldTargetActorBorrowV1 a;if(!actor(player,a))return {};
 return {a.search,&registry_,search_services(),this,
 [](void* p,std::uintptr_t c,std::uintptr_t t){return static_cast<CharacterWorldTargetOwnerV1*>(p)->look_at(c,t);},
 [](void* p,std::uintptr_t id,target_search::Object48** out){WorldTargetActorBorrowV1 a;if(!out||!static_cast<CharacterWorldTargetOwnerV1*>(p)->actor(id,a))return -1;*out=a.search;return 0;},objects};
}
}
