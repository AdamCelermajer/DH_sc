#include "npc_attack_command_owner_v1.hpp"
#include <stdexcept>
namespace dh2::character {
NpcAttackCommandOwnerV1::NpcAttackCommandOwnerV1(NpcAttackCommandBorrowV1 b,
 NpcAttackCommandServicesV1 s,std::uint32_t capacity):b_(b),s_(s){
 if(!b.world||!b.target||!b.target->state||!b.target->state->owner||!b.machine||!b.controller||!b.ai||!s.online)
  throw std::invalid_argument("Required same NPC attack World/AI/controller/Target/FSM");
 const auto id=b.target->state->owner->identity;
 if(!id||b.controller->owner!=id||b.ai->owner!=id||b.machine->native_fsm().character!=id)
  throw std::invalid_argument("NPC attack identity mismatch");
 projection_.owner=id;controller_.controllable=b.controller->controller;controller_.character=id;
 algorithm_=std::make_unique<skills::CharacterWorldPlayerAttackOwnerV1>(*b.world,
  projection_,*b.target->state,controller_,b.machine->state(),b.target->services,
  skills::PlayerAttackBackendsV1{this,backend,s.geometry,radius},capacity);
}
void NpcAttackCommandOwnerV1::read(){
 auto& a=*b_.ai;projection_.last=a.attack_last;projection_.index=a.attack_index;
 projection_.finisher=a.attack_finisher;projection_.continued=a.attack_continued;
 projection_.seeking=a.seeking;projection_.object_of_interest_type=a.owner_byte14a8;
 projection_.object_of_interest=b_.object_of_interest14a4?*b_.object_of_interest14a4:0;
 controller_.blocked=b_.controller->global_blocked;controller_.locked=b_.controller->locked;
 controller_.forced=b_.controller->forced;
 controller_.network_enabled=b_.network_enabled_a?*b_.network_enabled_a:0;
}
void NpcAttackCommandOwnerV1::write(){
 // Only source AI_DoMeleeAttack writes these fields. Animator fields index/
 // last/finisher and raw OOI are never synthesized by an attack command.
 b_.ai->attack_continued=projection_.continued;b_.ai->seeking=projection_.seeking;
 b_.ai->target=b_.target->state->target;b_.ai->target_sticky=b_.target->state->changed;
}
bool NpcAttackCommandOwnerV1::radius(void* p,std::uintptr_t id,float& value,std::string& error){
 auto& t=*static_cast<NpcAttackCommandOwnerV1*>(p);
 return t.s_.melee_radius&&t.s_.melee_radius(t.s_.context,id,value,error);
}
int NpcAttackCommandOwnerV1::backend(void* p,const AttackRequest32* q,AttackResponse16* out){
 auto& t=*static_cast<NpcAttackCommandOwnerV1*>(p);if(!q||!out)return -1;
 t.write();
 if(q->service==attack_network_mode){
  bool online{};if(!t.s_.online(t.s_.context,online,t.error_))return -1;
  if(online&&!t.b_.network_enabled_a){t.error_="Required actual NPC controller network byte+a";return -1;}
  out->word=online;return 0;
 }
 if(q->service==attack_set_attack_state){
  if(q->subject!=t.projection_.owner||q->argument0){t.error_="NPC attack state source receiver/mode mismatch";return -1;}
  // Source3c6488 false branch tail-calls native FSM SM_OnEvent(c354,target).
  const int result=t.b_.machine->event(0xc354,q->payload);
  if(result<0){t.error_=t.b_.machine->error();return -1;}
  out->word=static_cast<std::uint32_t>(result);t.read();return 0;
 }
 if(!t.s_.backend||t.s_.backend(t.s_.context,q,out,t.scope_,t.error_)){
  if(t.error_.empty())t.error_="Required source NPC attack backend "+std::to_string(q->service);
  return -1;
 }
 t.read();return 0;
}
int NpcAttackCommandOwnerV1::command(std::uintptr_t target,const dh2_script_callback_scope* scope){
 if(active_){error_="Nested same NPC attack command requires source reentry continuation";return -1;}
 error_.clear();read();
 if(b_.ai->owner_byte14a8==8&&!b_.object_of_interest14a4){error_="Required actual NPC object-of-interest14a4";return -1;}
 struct Guard{NpcAttackCommandOwnerV1& t;const dh2_script_callback_scope* old;
  const dh2_script_callback_scope* target_old;
  Guard(NpcAttackCommandOwnerV1& v,const dh2_script_callback_scope* s):t(v),old(v.scope_),target_old(v.b_.target->scope){t.active_=true;t.scope_=s;t.b_.target->scope=s;}
  ~Guard(){t.b_.target->scope=target_old;t.scope_=old;t.active_=false;}
 }guard(*this,scope);
 const int result=algorithm_->command(target);write();
 if(result&&error_.empty())error_=algorithm_->error();return result? -1:0;
}
}
