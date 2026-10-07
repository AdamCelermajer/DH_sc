#include "character_death_owner_v56.hpp"
#include <cstring>
namespace dh2::character {
bool CharacterDeathOwnerV56::coherent()const noexcept{
 const auto* d=b_.death;const auto* a=b_.events;
 return d&&a&&d->owner&&a->owner&&b_.targets&&b_.targets->state&&b_.targets->services.invoke&&
  b_.lifecycle&&s_.set_dead&&s_.cleanup_skills&&s_.cleanup_spells&&
  d->ai==a->ai&&d->target==b_.targets->state&&d->owner->character==a->owner->owner&&
  d->owner->character==b_.aggro.identity&&d->owner->character==b_.lifecycle->owner&&
  d->owner->state_machine==a->owner->state_machine&&d->owner->timers&&
  d->owner->timers->owner==d->owner->character&&b_.aggro.target==b_.targets&&
  s_.aggro.actor&&s_.aggro.on_deaggro;
}
void CharacterDeathOwnerV56::refresh()noexcept{
 b_.events->active=b_.lifecycle->active;
 b_.death->active=b_.events->active;b_.death->ais_virtuals=b_.events->ais_virtuals;
 std::memcpy(&b_.death->timer0,&b_.lifecycle->timer33,4);
 std::memcpy(&b_.death->timer1,&b_.lifecycle->timer34,4);
}
void CharacterDeathOwnerV56::publish_timers()noexcept{
 std::memcpy(&b_.lifecycle->timer33,&b_.death->timer0,4);
 std::memcpy(&b_.lifecycle->timer34,&b_.death->timer1,4);
}
int CharacterDeathOwnerV56::invoke(void* p,AIDeathState64* d,const AIDeathRequest48* q){
 auto& t=*static_cast<CharacterDeathOwnerV56*>(p);
 if(!q||d!=t.b_.death||!t.coherent()){t.error_="Required SAME Character death/AI/FSM/target/timer graph";return -1;}
 // Native kernel resets both IDs immediately before the first aggro call.
 // Publish there, before source callbacks can inspect/reenter the ScriptOwner.
 if(q->service==ai_death_clear_all_aggro)t.publish_timers();
 bool okay=false;
 switch(q->service){
 case ai_death_group:okay=t.s_.group&&t.s_.group(q->subject,q->owner,q->payload,t.error_);break;
 case ai_death_ais:
  if(q->callee==0x3dbe90&&q->operation==0x24){okay=true;break;} // Exact inherited bx lr.
  okay=t.s_.ais&&t.s_.ais(q->subject,q->callee,q->payload,t.scope_,t.error_);break;
 case ai_death_state:
  if(q->callee==0x3c58c8&&q->subject==d->owner->state_machine&&q->argument==1&&!q->owner&&!q->payload)
   okay=t.s_.set_dead(false,0,true,t.error_);
  break;
 case ai_death_clear_all_aggro:{AggroClearAllResultV2 r{};okay=character_aggro_clear_all_v2(r,t.b_.aggro,t.s_.aggro,t.error_,t.scope_);break;}
 case ai_death_clear_aggro_toward_me:{AggroClearAllResultV2 r{};okay=!q->argument&&character_aggro_clear_all_toward_me_v2(r,t.b_.aggro,false,t.s_.aggro,t.error_,t.scope_);break;}
 case ai_death_skill_cleanup:okay=t.s_.cleanup_skills(t.error_);break;
 case ai_death_spell_cleanup:okay=t.s_.cleanup_spells(t.error_);break;
 default:break;
 }
 // Selected AIS and timer IDs can change synchronously in group/state/skill
 // callbacks. Observe actual owner stores at the next original reload.
 t.refresh();
 if(!okay&&t.error_.empty())t.error_="Required original Character death service "+std::to_string(q->service);
 return okay?0:-1;
}
int CharacterDeathOwnerV56::on_died(AIDeathResult24& out,std::uintptr_t attacker,const dh2_script_callback_scope* scope){
 error_.clear();if(!coherent()||(scope&&!dh2_script_callback_scope_valid(scope))){error_="Required SAME Character death graph/private callback capability";return -1;}
 const auto* previous=scope_;scope_=scope;
 struct Restore{const dh2_script_callback_scope*& slot;const dh2_script_callback_scope* old;~Restore(){slot=old;}} restore{scope_,previous};
 refresh();const AIDeathServices16 services{this,invoke};
 const int status=dh2_character_ai_on_died(&out,b_.death,attacker,&b_.targets->services,&services);
 publish_timers();
 if(status!=1&&error_.empty())error_="Required original Character OnDied phase "+std::to_string(out.phase);
 return status;
}
int CharacterDeathOwnerV56::set_dead(AIDeathResult24& out){
 error_.clear();if(!coherent()){error_="Required SAME Character AI_SetDead graph";return -1;}
 refresh();const AIDeathServices16 services{this,invoke};
 const int status=dh2_character_ai_set_dead(&out,b_.death,&b_.targets->services,&services);
 publish_timers();
 if(status!=1&&error_.empty())error_="Required original AI_SetDead phase "+std::to_string(out.phase);
 return status;
}
}
