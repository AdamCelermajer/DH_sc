#include "npc_death_owner_v2.hpp"
namespace dh2::character {
bool NpcDeathOwnerV2::coherent()const noexcept{
 const auto* d=b_.death;const auto* a=b_.events;
 return d&&a&&d->owner&&a->owner&&b_.targets&&b_.targets->state&&
  b_.targets->services.invoke&&b_.dead_state&&b_.skills&&
  d->ai==a->ai&&d->target==b_.targets->state&&
  d->owner->character==a->owner->owner&&
  d->owner->character==b_.aggro.identity&&
  d->owner->character==b_.skills->state().owner&&
  d->owner->state_machine==a->owner->state_machine&&
  d->owner->timers&&d->owner->timers->owner==d->owner->character&&
  b_.aggro.target==b_.targets&&s_.aggro.actor&&s_.aggro.on_deaggro;
}
void NpcDeathOwnerV2::refresh_dispatch()noexcept{
 b_.death->active=b_.events->active;
 b_.death->ais_virtuals=b_.events->ais_virtuals;
}
int NpcDeathOwnerV2::invoke(void* p,AIDeathState64* d,const AIDeathRequest48* q){
 auto& t=*static_cast<NpcDeathOwnerV2*>(p);
 if(!q||d!=t.b_.death||!t.coherent()){t.error_="Required SAME NPC death/AI/FSM/target/timer/skill graph";return -1;}
 bool okay=false;
 switch(q->service){
 case ai_death_group:
  okay=t.s_.group&&t.s_.group(t.s_.context,q->subject,q->owner,q->payload,t.error_);break;
 case ai_death_ais:
  if(q->callee==0x3dbe90&&q->operation==0x24){okay=true;break;} // Whole inherited bx-lr.
  okay=t.s_.ais&&t.s_.ais(t.s_.context,q->subject,q->callee,q->payload,t.scope_,t.error_);break;
 case ai_death_state:
  if(q->callee==0x3c58c8&&q->subject==d->owner->state_machine&&
     q->argument==1&&!q->owner&&!q->payload)
   okay=t.b_.dead_state->set(false,0,true)==1;
  if(!okay)t.error_=t.b_.dead_state->error();break;
 case ai_death_clear_all_aggro:{
  AggroClearAllResultV2 r{};okay=character_aggro_clear_all_v2(r,t.b_.aggro,t.s_.aggro,t.error_,t.scope_);break;
 }
 case ai_death_clear_aggro_toward_me:{
  AggroClearAllResultV2 r{};okay=!q->argument&&character_aggro_clear_all_toward_me_v2(r,t.b_.aggro,false,t.s_.aggro,t.error_,t.scope_);break;
 }
 case ai_death_skill_cleanup:okay=t.b_.skills->cleanup_skills()==1;if(!okay)t.error_=t.b_.skills->error();break;
 case ai_death_spell_cleanup:okay=t.b_.skills->cleanup_spells()==1;if(!okay)t.error_=t.b_.skills->error();break;
 default:break;
 }
 // Original reloads selected AIS after group delivery. Event projection is its
 // sole active authority; a callback can change selection synchronously.
 t.refresh_dispatch();
 if(!okay&&t.error_.empty())t.error_="Required original NPC death service "+std::to_string(q->service);
 return okay?0:-1;
}
int NpcDeathOwnerV2::on_died(AIDeathResult24& out,std::uintptr_t attacker,
 const dh2_script_callback_scope* scope){
 error_.clear();if(!coherent()||(scope&&!dh2_script_callback_scope_valid(scope))){
  error_="Required SAME NPC death graph/private callback capability";return -1;
 }
 const auto* previous=scope_;scope_=scope;
 struct Restore {const dh2_script_callback_scope*& slot;const dh2_script_callback_scope* old;~Restore(){slot=old;}}restore{scope_,previous};
 refresh_dispatch();const AIDeathServices16 services{this,invoke};
 const int status=dh2_character_ai_on_died(&out,b_.death,attacker,
  &b_.targets->services,&services);
 if(status!=1&&error_.empty())error_="Required original NPC OnDied phase "+std::to_string(out.phase);
 return status;
}
}
