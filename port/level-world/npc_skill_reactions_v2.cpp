#include "npc_skill_reactions_v2.hpp"
namespace dh2::character {
NpcSkillReactionsV2::NpcSkillReactionsV2(NpcSkillReactionsBorrowV2 b,NpcSkillReactionsServicesV2 s):b_(b),s_(s){
 if(b.animations)for(const auto& row:b.animations->characters){
  if(row.fields[32].size()!=1||row.fields[29].size()!=1){error_="Required original scalar Stunned/Scared rows";return;}
  stunned_.push_back(row.fields[32].front());scared_.push_back(row.fields[29].front());
 }
 effects_={b.machine?&b.machine->native_fsm():nullptr,stunned_.data(),scared_.data(),static_cast<std::uint32_t>(stunned_.size()),0};effect_services_={this,effect};
}
bool NpcSkillReactionsV2::valid()const noexcept{return b_.machine&&b_.injury&&b_.injury->valid()&&b_.properties&&!dh2_property_validate(b_.properties)&&b_.ai&&b_.animations&&b_.design&&b_.timers&&b_.timer_services&&b_.timers->owner==b_.machine->native_fsm().character&&stunned_.size()==b_.animations->characters.size();}
KnockbackReactionBorrowV1 NpcSkillReactionsV2::knockback_borrow()const{return {&b_.machine->native_fsm(),b_.properties,b_.ai,b_.animations};}
int NpcSkillReactionsV2::effect(void* p,NativeEffects32* effects,const NativeEffectRequest32* q,std::uint32_t* out){
 auto& t=*static_cast<NpcSkillReactionsV2*>(p);if(!q||!out||effects!=&t.effects_||!t.valid()||q->character!=t.b_.machine->native_fsm().character)return -1;
 const auto& k=t.s_.knockback;
 switch(q->service){
 case effect_ai_flags:{const auto* row=data::ai_props(*t.b_.ai,t.b_.properties->resolved[1]);if(!row)return -1;*out=row->flags;return 0;}
 case effect_animation_index:*out=std::uint32_t(t.b_.properties->resolved[2]);return 0;
 case effect_start_timer:{const auto id=dh2_character_timer_start(t.b_.timers,q->argument0,std::int32_t(q->argument1),std::int32_t(q->argument2),q->payload,t.b_.timer_services);if(id< -1)return -1;*out=std::uint32_t(id);return 0;}
 case effect_stance_bits:{const auto* d=t.b_.design->design();int value{};if(!d||!d->lookup||d->lookup(d->context,0,"AnimStancedAnim","SL__LIST_IPHONE",&value))return -1;*out=std::uint32_t(value);return 0;}
 case effect_anim_stance:{int value{};if(!k.stance||k.stance(k.context,q->character,&value))return -1;*out=std::uint32_t(value);return 0;}
 case effect_force_state:return t.b_.machine->transition(int(q->argument0),int(q->argument1),q->payload)<0?-1:0;
 case effect_state_event:return t.b_.machine->event(int(q->argument0),q->payload)<0?-1:0;
 case effect_set_animation:return k.animation?k.animation(k.context,int(q->argument0)):-1;
 case effect_cancel_sneaking:return t.b_.injury->cancel_sneaking();
 case effect_pin:return k.pin?k.pin(k.context):-1;
 case effect_unpin:return k.unpin?k.unpin(k.context):-1;
 default:return t.s_.remaining.invoke?t.s_.remaining.invoke(t.s_.remaining.context,effects,q,out):-1;
 }
}
int NpcSkillReactionsV2::application(const skills::SkillApplyRequestV6& q,skills::SkillApplyResponseV6* out){
 using namespace skills;
 if(q.service==skill_apply_injure_v6||q.service==skill_apply_cancel_sneaking_v6)return b_.injury?b_.injury->application(q,out):-1;
 if(q.service<skill_apply_push_v6||q.service>skill_apply_slow_v6)return 0;
 if(!valid()||!out||q.subject!=b_.machine->native_fsm().character||q.target!=q.subject)return -1;
 int status=-2;
 if(q.service==skill_apply_push_v6)status=character_set_knockback_v1(knockback_borrow(),q.word!=0,q.attacker,q.flags!=0,s_.knockback);
 else if(q.service==skill_apply_stun_v6)status=dh2_character_native_effect_set(&effects_,0,q.word,1,q.attacker,0,&effect_services_);
 else if(q.service==skill_apply_scare_v6)status=dh2_character_native_effect_set(&effects_,1,q.word,1,q.attacker,q.flags,&effect_services_);
 else{BuffResult24 result{};if(b_.slow.properties!=b_.properties){error_="Slow reaction properties must borrow SAME NPC property owner";return -1;}status=character_slow_reaction_v1(b_.slow,q.word,result,error_);}
 if(status<0)return status;*out={};return 1;
}
int NpcSkillReactionsV2::body(StateOwnerMachine40* machine,const StateOwnerRequest48& q){
 if(!valid()||machine!=&b_.machine->owner().machine()||q.character!=b_.machine->native_fsm().character)return -1;
 if(q.state==11)return b_.injury->body(machine,q);
 if(q.state==8||q.state==9){const unsigned kind=q.state==8?1:0;
  if(q.operation==state_owner_focus&&q.source_function==(kind?0x3c45c4u:0x3c3cc0u))return dh2_character_native_effect_focus(&effects_,kind,&effect_services_);
  if(q.operation==state_owner_blur&&q.source_function==(kind?0x3c4834u:0x3c3b4cu))return dh2_character_native_effect_body(&effects_,kind,1,&effect_services_);
  if(q.operation==state_owner_event&&q.source_function==(kind?0x3c2b28u:0x3c0030u))return dh2_character_native_effect_event(&effects_,kind,q.event,&effect_services_);
 }
 if(q.state==10){const auto b=knockback_borrow();
  if(q.operation==state_owner_focus&&q.source_function==0x3c4a48)return character_knockback_focus_v1(b,q.payload,s_.knockback);
  if(q.operation==state_owner_blur&&q.source_function==0x3c48e0)return character_knockback_blur_v1(b,s_.knockback);
  if(q.operation==state_owner_event&&q.source_function==0x3c5ab0)return character_knockback_event_v1(b,q.event,s_.knockback);
 }
 return 0;
}
int NpcSkillReactionsV2::update(StateOwnerMachine40* machine,const StateOwnerUpdateRequest24& q){
 if(!valid()||machine!=&b_.machine->owner().machine())return -1;
 if(q.state==8&&q.source_function==0x3c4788)return dh2_character_native_effect_body(&effects_,1,0,&effect_services_);
 if(q.state==9&&q.source_function==0x3c549c)return dh2_character_native_effect_body(&effects_,0,0,&effect_services_);
 if(q.state==10&&q.source_function==0x3c0038)return 1;return 0;
}
int NpcSkillReactionsV2::timer_event(std::uint32_t event,std::uintptr_t payload){if(!valid()||(event!=0x2b&&event!=0x2c))return -1;return b_.machine->event(int(event),payload);}
int NpcSkillReactionsV2::frame_effect(const NativeFsmRequest32& q){
 if(!valid()||(q.service!=fsm_set_stun&&q.service!=fsm_set_scare)||q.subject!=b_.machine->native_fsm().character)return -1;
 return dh2_character_native_effect_set(&effects_,q.service==fsm_set_scare?1:0,q.argument0,q.argument1,q.payload,q.force,&effect_services_);
}
}
