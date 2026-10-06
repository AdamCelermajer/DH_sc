#include "npc_recurring_effects_v1.hpp"
namespace dh2::character {
NpcRecurringEffectsV1::NpcRecurringEffectsV1(NpcRecurringBorrowV1 b):b_(b){services_={this,invoke};}
bool NpcRecurringEffectsV1::valid()const noexcept{
 return b_.identity&&b_.properties&&b_.network_id&&b_.remote_update&&b_.dead&&b_.state&&
 b_.outgoing&&b_.incoming&&b_.combat&&b_.shared_cf&&b_.debug&&b_.debug->invoke;
}
int NpcRecurringEffectsV1::invoke(void* p,TimerEffectState32* s,const TimerEffectRequest40* q,std::int32_t* out,data::CombatResult* result){
 auto& t=*static_cast<NpcRecurringEffectsV1*>(p);
 if(!s||s!=&t.state_||!q||!out||q->subject!=t.b_.identity)return -1;
 const auto status=t.service(*q,*out,result);
 if(status)t.error_="Required NPC recurring source service "+std::to_string(q->service);
 return status;
}
int NpcRecurringEffectsV1::service(const TimerEffectRequest40& q,std::int32_t& out,data::CombatResult* result){
 using namespace skills;
 switch(q.service){
 case effect_remote_update:out=*b_.network_id!=-1?1:*b_.remote_update;return 0;
 case effect_is_dead:out=*b_.dead;return 0;
 case effect_current_state:out=b_.state->current;return 0;
 case effect_debug_load:{std::uintptr_t ignored{};const SkillAttackNativeRequestV6 request{skill_attack_debug_load_v6,0,0,nullptr};return b_.debug->invoke(b_.debug->context,&request,&ignored);}
 case effect_debug_query:{
  auto debug=dot_calculate_services_v7(*b_.debug);DotActor32 actor{};DotResponse8 response{};
  const DotRequest40 request{dot_debug_query,0,b_.identity,0,q.name,0,0};
  const auto status=debug.invoke(debug.context,&actor,&request,&response,nullptr);out=response.word;return status;
 }
 case effect_dot_calculate:{
  if(!result||q.target!=b_.identity)return -1;
  auto world=b_.combat->native_world();SkillAttackActorV6* attack{};SkillApplyActorV6* apply{};
  if(!world.actor||world.actor(world.context,b_.identity,&attack,&apply)||!apply||
   apply->identity!=b_.identity||apply->properties!=b_.properties||!apply->network_id||
   !apply->combo||!apply->push_death||!apply->invulnerable)return -1;
  DotActor32 actor{b_.identity,b_.properties,*apply->network_id,*apply->combo,*apply->push_death,*apply->invulnerable,0};
  auto debug=dot_calculate_services_v7(*b_.debug);DotResult24 output{};
  return dh2_character_dot_calculate_result(&output,result,b_.shared_cf,&actor,q.amount,q.element,&debug)==1?0:-1;
 }
 case effect_dot_apply:{
  if(!result||q.target!=b_.identity)return -1;
  SkillApplyOutputV6 output{};
  // Whole SAME registered World application: HitFor, status/FSM, text, sound,
  // AI and required lethal continuation remain mandatory. No direct HP write.
  return b_.combat->apply(&output,result,b_.identity,b_.identity)==1?0:-1;
 }
 default:return -1;
 }
}
int NpcRecurringEffectsV1::event(std::uint32_t event,TimerEffectResult24& out){
 if(!valid()){error_="Required SAME NPC recurring backing";return -1;}
 state_={b_.identity,b_.properties,b_.outgoing->count,b_.incoming->count};
 return dh2_character_timer_effect(&out,&state_,event,&services_);
}
}
