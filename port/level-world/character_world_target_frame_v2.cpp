#include "character_world_target_frame_v2.hpp"
#include <cstring>
namespace dh2::character::skills {
bool CharacterWorldTargetFrameV2::position(std::uintptr_t id,const float*& out){
 WorldTargetActorBorrowV1 b{};
 if(world_.actor(id,&b)||!b.target_node||!b.position||(*b.target_node&&!b.target_enabled)){
  error_="Required actual GetTargetPosition fields for AI frame";return false;
 }
 out=dh2_world_target_position_v1(b.position,b.cached_target_position,*b.target_node,*b.target_node?*b.target_enabled:0);
 if(!out){error_="Required actual target-node cache for AI frame";return false;}return true;
}
int CharacterWorldTargetFrameV2::query(void* raw,TargetState48* state,const TargetUpdateRequest24* q,std::uint32_t* out){
 auto& t=*static_cast<CharacterWorldTargetFrameV2*>(raw);
 if(!state||!state->owner||!q||!out)return -1;*out=0;
 const auto owner=state->owner->identity;
 if(q->service==target_update_awaiting_spawn||q->service==target_update_in_limbus){
  std::uint32_t actual;if(!t.services_.machine_state||t.services_.machine_state(t.services_.context,q->subject,&actual)){
   t.error_="Required actual SM_GetState for AI frame";return -1;
  }
  *out=actual==(q->service==target_update_awaiting_spawn?17u:0u);return 0;
 }
 if(q->service==target_update_raise_event){
  if(!t.services_.raise_event||t.services_.raise_event(t.services_.context,q->subject,q->event,q->other)){
   t.error_="Required Character RaiseEvent for AI target event "+std::to_string(q->event);return -1;
  }return 0;
 }
 if(q->service==target_update_interactive||q->service==target_update_dead){
  const auto services=t.world_.targets().search_services();target_search::Response16 response{};
  const target_search::Request24 request{q->service==target_update_interactive?target_search::is_interactive:target_search::is_dead,0,q->subject,q->other};
  if(services.invoke(services.context,&request,&response)){t.error_=t.world_.targets().error();return -1;}
  *out=std::uint32_t(response.word);return 0;
 }
 if(q->service==target_update_owner_ai_id||q->service==target_update_sight){
  WorldTargetActorBorrowV1 b{};std::int32_t id;
  if(t.world_.actor(owner,&b)||!b.character||dh2_character_target_ai_id(&id,b.character->resolved,t.ai_.rows.size())){
   t.error_="Required same AI properties for target frame";return -1;
  }
  if(q->service==target_update_owner_ai_id){std::memcpy(out,&id,4);return 0;}
  // q.subject is the embedded AI identity, NOT its Character owner.
  if(q->subject!=state->identity){t.error_="Mismatched same AI sight receiver";return -1;}
  const auto* row=data::ai_props(t.ai_,id);const float *a,*bpos;
  if(!row||!t.position(owner,a)||!t.position(q->other,bpos))return -1;
  return dh2_character_target_sight(out,a,bpos,row->view_radius);
 }
 if(q->service==target_update_close_range){
  if(!t.services_.close_range||t.services_.close_range(t.services_.context,owner,q->other,out)){
   t.error_="Required whole AI_IsInCloseRange for target frame";return -1;
  }return 0;
 }
 WorldAIAttackQueryV1 operation;
 if(q->service==target_update_can_range)operation=WorldAIAttackQueryV1::CharacterCanRangeAttack;
 else if(q->service==target_update_ranged_range)operation=WorldAIAttackQueryV1::IsInRange;
 else if(q->service==target_update_melee_range)operation=WorldAIAttackQueryV1::IsInMeleeRange;
 else {t.error_="Unknown AI target frame query";return -1;}
 std::int32_t result;
 if(!t.geometry_.read(owner,q->other,operation,result,t.error_))return -1;
 std::memcpy(out,&result,4);return 0;
}
int CharacterWorldTargetFrameV2::update(TargetState48& state){
 error_.clear();const TargetUpdateServices16 services{this,query};
 const auto result=dh2_character_target_update(&state,&services);
 if(result&&error_.empty())error_="Invalid source target frame binding";
 return result;
}
}
