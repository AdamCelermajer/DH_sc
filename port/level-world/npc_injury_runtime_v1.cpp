#include "npc_injury_runtime_v1.hpp"
namespace dh2::character {
NpcInjuryRuntimeV1::NpcInjuryRuntimeV1(NpcInjuryBorrowV1 b,NpcInjuryServicesV1 s):borrow_(b),services_(s),kernel_{this,is_player,table,animation_id,constant,stance,event,transition,debug,animation,cancel,look}{}
bool NpcInjuryRuntimeV1::valid()const noexcept {
 if(!borrow_.machine||!borrow_.session||!borrow_.properties||!borrow_.animations||!borrow_.design||!borrow_.target||!borrow_.target->owner||!borrow_.character||!borrow_.gate_14fc||!borrow_.life||borrow_.session->combat_state().get()!=borrow_.life)return false;
 const auto id=borrow_.machine->native_fsm().character;
 const auto& live=*borrow_.properties;const auto& same=borrow_.session->property_view();
 return id&&borrow_.target->owner->identity==id&&borrow_.character->identity==id&&borrow_.character->resolved==live.resolved&&borrow_.session->timers().owner==id&&same.defaults==live.defaults&&same.types==live.types&&same.base==live.base&&same.saved==live.saved&&same.gear==live.gear&&same.resolved==live.resolved;
}
PlayerInjureBorrowV7 NpcInjuryRuntimeV1::kernel_borrow()const{return {borrow_.machine->native_fsm().character,&borrow_.machine->state(),borrow_.gate_14fc};}
int NpcInjuryRuntimeV1::is_player(void* p,std::uintptr_t id,bool* out){auto& t=*static_cast<NpcInjuryRuntimeV1*>(p);const auto& s=t.services_.queries;if(!out||!s.invoke)return -1;target_providers::Request24 q{target_providers::virtual_player,0,id,0};std::uintptr_t v{};if(s.invoke(s.context,&q,&v))return -1;*out=v!=0;return 0;}
int NpcInjuryRuntimeV1::table(void* p,std::uintptr_t,int* out){auto& t=*static_cast<NpcInjuryRuntimeV1*>(p);if(!out)return -1;const auto raw=t.borrow_.properties->resolved[2];*out=raw>=0&&std::size_t(raw)<t.borrow_.animations->characters.size()?raw:17;return 0;}
int NpcInjuryRuntimeV1::animation_id(void* p,int table,bool* found,int* out){auto& t=*static_cast<NpcInjuryRuntimeV1*>(p);if(!found||!out)return -1;*found=table>=0&&std::size_t(table)<t.borrow_.animations->characters.size();if(!*found)return 0;const auto& field=t.borrow_.animations->characters[table].fields[14];if(field.size()!=1)return -1;*out=field.front();return 0;}
int NpcInjuryRuntimeV1::constant(void* p,const char* key,const char* group,int* out){auto& t=*static_cast<NpcInjuryRuntimeV1*>(p);const auto* d=t.borrow_.design->design();return d&&d->lookup&&out?d->lookup(d->context,0,key,group,out):-1;}
int NpcInjuryRuntimeV1::stance(void* p,std::uintptr_t id,int* out){
 auto& t=*static_cast<NpcInjuryRuntimeV1*>(p);if(!out)return -1;bool player{};
 // Original Character.GetAnimStance3a53e0 queries IsPlayer. Nonplayers set
 // candidate0, still query AnimStances/COUNT_IPHONE, then return0 for every
 // signed count. No fabricated empty inventory or player Gear is reached.
 if(is_player(p,id,&player))return -1;
 if(!player){int count{};if(constant(p,"AnimStances","COUNT_IPHONE",&count))return -1;*out=0;return 0;}
 return t.services_.stance?t.services_.stance(t.services_.context,id,out):-1;
}
int NpcInjuryRuntimeV1::event(void* p,int event,std::uintptr_t payload){return static_cast<NpcInjuryRuntimeV1*>(p)->borrow_.machine->event(event,payload)<0?-1:0;}
int NpcInjuryRuntimeV1::transition(void* p,int state,int event,std::uintptr_t payload){return static_cast<NpcInjuryRuntimeV1*>(p)->borrow_.machine->transition(state,event,payload)<0?-1:0;}
int NpcInjuryRuntimeV1::debug(void* p,const char* name){
 const auto* s=static_cast<NpcInjuryRuntimeV1*>(p)->services_.debug;if(!s||!s->invoke)return -1;
 auto call=[&](unsigned op,std::uintptr_t subject,const char* text,std::uintptr_t& out){skills::SkillAttackNativeRequestV6 q{op,0,subject,text};return s->invoke(s->context,&q,&out);};
 std::uintptr_t v{},token{};if(call(skills::skill_attack_debug_load_v6,0,nullptr,v)||call(skills::skill_attack_string_construct_v6,0,name,token)||!token||call(skills::skill_attack_debug_get_v6,token,nullptr,v))return -1;return call(skills::skill_attack_string_destroy_v6,token,nullptr,v);
}
int NpcInjuryRuntimeV1::animation(void* p,int value){auto& t=*static_cast<NpcInjuryRuntimeV1*>(p);return t.services_.set_animation?t.services_.set_animation(t.services_.context,&t.borrow_.machine->state(),value):-1;}
int NpcInjuryRuntimeV1::cancel(void* p,std::uintptr_t id){auto& t=*static_cast<NpcInjuryRuntimeV1*>(p);return id==t.borrow_.machine->native_fsm().character?t.cancel_sneaking():-1;}
int NpcInjuryRuntimeV1::look(void* p,std::uintptr_t id){auto& t=*static_cast<NpcInjuryRuntimeV1*>(p);return t.services_.look_at?t.services_.look_at(t.services_.context,id,t.borrow_.target->target):-1;}
int NpcInjuryRuntimeV1::injure(std::uintptr_t attacker,bool direct){if(!valid())return -1;const auto b=kernel_borrow();return player_set_injure_v7(&b,attacker,direct,&kernel_);}
int NpcInjuryRuntimeV1::cancel_sneaking(){
 if(!valid())return -1;
 struct Bridge {NpcInjuryRuntimeV1* owner;static int invoke(void* p,const sneaking::Request24* q,std::uint32_t* out){auto& t=*static_cast<Bridge*>(p)->owner;if(q->operation==sneaking::is_player){bool player{};if(is_player(&t,q->receiver,&player))return -1;*out=player;return 0;}const auto* s=t.borrow_.skill_services;return s&&s->invoke?s->invoke(s->context,q,out):-1;}} bridge{this};
 sneaking::Services16 services{&bridge,Bridge::invoke};
 sneaking::Character48 projection{};projection.identity=borrow_.character->identity;projection.resolved={borrow_.properties->resolved,224,0};projection.changed415=borrow_.character->interactive415;
 auto* actual=borrow_.skill_character;
 if(actual){if(actual->identity!=projection.identity||actual->resolved.words!=projection.resolved.words||actual->changed415!=projection.changed415)return -1;}
 else actual=&projection;
 const auto status=dh2_character_cancel_sneaking(actual,&services);
 borrow_.character->interactive415=actual->changed415;
 return status==0?0:-2;
}
int NpcInjuryRuntimeV1::tick(std::uint32_t dt){return valid()?player_injure_gate_tick_v7(borrow_.gate_14fc,dt):-1;}
int NpcInjuryRuntimeV1::body(StateOwnerMachine40* machine,const StateOwnerRequest48& q){
 if(!valid()||machine!=&borrow_.machine->owner().machine()||q.character!=borrow_.character->identity)return -1;if(q.state!=11)return 0;const auto b=kernel_borrow();
 if(q.operation==state_owner_focus&&q.source_function==0x3c33e8)return player_injure_focus_v7(&b,&kernel_);
 if(q.operation==state_owner_blur&&q.source_function==0x3c4ba4)return player_injure_blur_v7(&b,&kernel_);
 if(q.operation==state_owner_event&&q.source_function==0x3c0044)return player_injure_empty_v7(q.source_function);return 0;
}
int NpcInjuryRuntimeV1::application(const skills::SkillApplyRequestV6& q,skills::SkillApplyResponseV6* out){
 if(q.service!=skills::skill_apply_injure_v6&&q.service!=skills::skill_apply_cancel_sneaking_v6)return 0;
 if(!valid()||!out||q.subject!=borrow_.character->identity||q.target!=q.subject)return -1;
 const auto status=q.service==skills::skill_apply_injure_v6?injure(q.attacker,q.flags!=0):cancel_sneaking();
 if((q.service==skills::skill_apply_injure_v6&&status!=1)||(q.service==skills::skill_apply_cancel_sneaking_v6&&status!=0))return -2;*out={};return 1;
}
}
