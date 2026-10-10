#include "player_injury_runtime_v7.hpp"
namespace dh2::character {
PlayerInjuryRuntimeV7::PlayerInjuryRuntimeV7(PlayerInjuryRuntimeBorrowV7 borrow,PlayerInjuryRuntimeServicesV7 services):borrow_(borrow),services_(services),kernel_{this,is_player,animation_table,injure_animation,constant,stance,event,transition,debug,animation,cancel,look}{}
bool PlayerInjuryRuntimeV7::valid()const noexcept {
 if(!borrow_.state_owner||!borrow_.player||!borrow_.properties||!borrow_.animations||!borrow_.target||!borrow_.fields||!borrow_.sneaking_415)return false;
 const auto id=borrow_.state_owner->native_fsm().character;
 const auto& same=borrow_.player->session().property_view();const auto& live=*borrow_.properties;
 return id&&borrow_.target->owner&&borrow_.target->owner->identity==id&&
  borrow_.player->session().timers().owner==id&&
  same.defaults==live.defaults&&same.types==live.types&&same.base==live.base&&
  same.saved==live.saved&&same.gear==live.gear&&same.resolved==live.resolved;
}
PlayerInjureBorrowV7 PlayerInjuryRuntimeV7::kernel_borrow()const {
 return {borrow_.state_owner->native_fsm().character,&borrow_.state_owner->state(),&borrow_.fields->gate_14fc};
}
int PlayerInjuryRuntimeV7::is_player(void* p,std::uintptr_t id,bool* out){
 auto& self=*static_cast<PlayerInjuryRuntimeV7*>(p);const auto& q=self.services_.queries;
 if(!out||!q.invoke)return -1;
 target_providers::Request24 request{target_providers::virtual_player,0,id,0};std::uintptr_t answer{};
 if(q.invoke(q.context,&request,&answer))return -1;*out=answer!=0;return 0;
}
int PlayerInjuryRuntimeV7::animation_table(void* p,std::uintptr_t,int* out){
 auto& self=*static_cast<PlayerInjuryRuntimeV7*>(p);if(!out)return -1;
 // SM_SetInjureState calls GetCharAnimTableId and does nothing when that
 // returned row is outside Arrays::CharAnimTable. Do not turn a missing or
 // malformed selected-character row into an unrelated row 17.
 *out=self.borrow_.properties->resolved[2];return 0;
}
int PlayerInjuryRuntimeV7::injure_animation(void* p,int table,bool* found,int* out){
 auto& self=*static_cast<PlayerInjuryRuntimeV7*>(p);if(!found||!out)return -1;
 *found=table>=0&&std::size_t(table)<self.borrow_.animations->characters.size();
 if(!*found)return 0;
 // Parsed fields omit the original row-ID header: raw +0x3c is Injured[14].
 const auto& field=self.borrow_.animations->characters[table].fields[14];
 if(field.size()!=1)return -1;*out=field.front();return 0;
}
int PlayerInjuryRuntimeV7::constant(void* p,const char* key,const char* group,int* out){
 if(!out)return -1;auto& self=*static_cast<PlayerInjuryRuntimeV7*>(p);
 return self.borrow_.player->session().constant(key,group,*out);
}
int PlayerInjuryRuntimeV7::stance(void* p,std::uintptr_t id,int* out){auto& self=*static_cast<PlayerInjuryRuntimeV7*>(p);return self.services_.stance?self.services_.stance(self.services_.context,id,out):-1;}
int PlayerInjuryRuntimeV7::event(void* p,int event,std::uintptr_t payload){auto& self=*static_cast<PlayerInjuryRuntimeV7*>(p);if(!self.services_.outer)return -1;return self.borrow_.state_owner->event(event,payload,*self.services_.outer)<0?-1:0;}
int PlayerInjuryRuntimeV7::transition(void* p,int state,int event,std::uintptr_t payload){auto& self=*static_cast<PlayerInjuryRuntimeV7*>(p);if(!self.services_.outer)return -1;return self.borrow_.state_owner->transition(state,event,payload,*self.services_.outer)<0?-1:0;}
int PlayerInjuryRuntimeV7::debug(void* p,const char* name){
 auto& self=*static_cast<PlayerInjuryRuntimeV7*>(p);const auto* s=self.services_.debug;if(!s||!s->invoke)return -1;
 auto call=[&](unsigned op,std::uintptr_t subject,const char* text,std::uintptr_t& out){skills::SkillAttackNativeRequestV6 q{op,0,subject,text};return s->invoke(s->context,&q,&out);};
 std::uintptr_t ignored{},token{};
 if(call(skills::skill_attack_debug_load_v6,0,nullptr,ignored)||call(skills::skill_attack_string_construct_v6,0,name,token)||!token||call(skills::skill_attack_debug_get_v6,token,nullptr,ignored))return -1;
 return call(skills::skill_attack_string_destroy_v6,token,nullptr,ignored);
}
int PlayerInjuryRuntimeV7::animation(void* p,int animation){auto& self=*static_cast<PlayerInjuryRuntimeV7*>(p);return self.services_.set_animation?self.services_.set_animation(self.services_.context,animation):-1;}
int PlayerInjuryRuntimeV7::cancel(void* p,std::uintptr_t id){auto& self=*static_cast<PlayerInjuryRuntimeV7*>(p);if(id!=self.borrow_.state_owner->native_fsm().character)return -1;return self.borrow_.player->native_cancel_sneaking(self.borrow_.sneaking_415);}
int PlayerInjuryRuntimeV7::look(void* p,std::uintptr_t id){auto& self=*static_cast<PlayerInjuryRuntimeV7*>(p);return self.services_.look_at?self.services_.look_at(self.services_.context,id,self.borrow_.target->target):-1;}
int PlayerInjuryRuntimeV7::injure(std::uintptr_t attacker,bool direct){if(!valid())return -1;const auto b=kernel_borrow();return player_set_injure_v7(&b,attacker,direct,&kernel_);}
int PlayerInjuryRuntimeV7::tick(std::uint32_t dt){return valid()?player_injure_gate_tick_v7(&borrow_.fields->gate_14fc,dt):-1;}
int PlayerInjuryRuntimeV7::body(StateOwnerMachine40* machine,const StateOwnerRequest48& q){
 if(!valid()||machine!=&borrow_.state_owner->machine()||q.character!=borrow_.state_owner->native_fsm().character)return -1;
 if(q.state!=11)return 0;
 const auto b=kernel_borrow();
 if(q.operation==state_owner_focus&&q.source_function==0x3c33e8)return player_injure_focus_v7(&b,&kernel_);
 if(q.operation==state_owner_blur&&q.source_function==0x3c4ba4)return player_injure_blur_v7(&b,&kernel_);
 if(q.operation==state_owner_event&&q.source_function==0x3c0044)return player_injure_empty_v7(q.source_function);
 return 0;
}
int PlayerInjuryRuntimeV7::is_idle(std::uintptr_t id,bool* out)const {if(!valid()||!out||id!=borrow_.state_owner->native_fsm().character)return -1;*out=dh2_character_state_is_idle(borrow_.state_owner->state().current,0)!=0;return 0;}
int PlayerInjuryRuntimeV7::application(const skills::SkillApplyRequestV6& q,skills::SkillApplyResponseV6* out){
 if(q.service!=skills::skill_apply_injure_v6&&q.service!=skills::skill_apply_cancel_sneaking_v6)return 0;
 if(!valid()||!out||q.subject!=borrow_.state_owner->native_fsm().character||q.target!=q.subject)return -1;
 const auto result=q.service==skills::skill_apply_injure_v6?injure(q.attacker,q.flags!=0):cancel(this,q.subject);
 if((q.service==skills::skill_apply_injure_v6&&result!=1)||(q.service==skills::skill_apply_cancel_sneaking_v6&&result!=0))return -2;
 *out={};return 1;
}
}
