#include "character_ai_attack.hpp"
#include <cstring>
namespace {
using namespace dh2::character;
bool aligned(const void* p,std::uintptr_t n){return p&&reinterpret_cast<std::uintptr_t>(p)%n==0;}
bool valid(const AttackState64* s){return aligned(s,alignof(AttackState64))&&s->owner&&s->heading_active<=255&&s->continued<=255&&s->last<=255&&s->finisher<=255&&s->seeking<=255&&s->object_of_interest_type>=-128&&s->object_of_interest_type<=127;}
bool services_valid(const AttackServices16* p){return aligned(p,alignof(AttackServices16))&&p->invoke;}
float floating(std::uint32_t x){float v;std::memcpy(&v,&x,4);return v;}
std::uint32_t bits(float x){std::uint32_t v;std::memcpy(&v,&x,4);return v;}
struct Calls {
 AttackState64* state;ControllerAttackState32* controller;const AttackServices16* services;
 AttackResponse16 call(AttackService op,std::uintptr_t subject=0,std::uintptr_t payload=0,std::uint32_t a=0,std::uint32_t b=0){
  const AttackRequest32 request{op,a,b,0,subject,payload};AttackResponse16 out{};
  services->invoke(services->context,state,controller,&request,&out);return out;
 }
 std::uint32_t query(AttackService op,std::uintptr_t target=0){return call(op,state->owner,target).word;}
 std::uint32_t log(std::uint32_t site){return call(attack_diagnostic,state->owner,0,site).word;}
 void set_target(std::uintptr_t target,std::uint32_t mode){call(attack_set_target,state->owner,target,mode);}
};
bool list_valid(const AttackTargetList24* list){return aligned(list,alignof(AttackTargetList24))&&list->count<=65536&&list->cursor<=list->count&&(!list->count||aligned(list->entries,alignof(std::uintptr_t)));}
bool pop_all(Calls& c,AttackTargetList24* list){
 while(list->cursor<list->count){const auto before=list->cursor;c.call(attack_list_pop,list->token,reinterpret_cast<std::uintptr_t>(list));if(!list_valid(list)||list->cursor<=before)return false;}
 return true;
}
int search(Calls& c,AttackTargetList24* list,bool narrow){
 std::uint32_t angle=0x40c90fdb;
 if(narrow){
  const auto degrees=static_cast<std::int32_t>(c.call(attack_frontal_angle,c.state->owner).word);
  const auto half=degrees/2-(degrees<0&&(degrees%2)!=0); // ARM arithmetic shift.
  angle=bits(static_cast<float>(half)*floating(0x3c8efa35));
 }else c.call(attack_list_reset_sort,list->token,reinterpret_cast<std::uintptr_t>(list));
 c.call(attack_list_search,list->token,reinterpret_cast<std::uintptr_t>(list),0,angle);
 return list_valid(list)?0:2;
}
int melee(Calls& c,std::uintptr_t requested,std::uint32_t speculative){
 auto& s=*c.state;
 if(c.query(attack_owner_dead))return 0;
 if(s.owner_flags528&1)return 0;
 if(c.query(attack_owner_ranged)){c.call(attack_range_redirect,s.owner,requested,speculative);return 0;}
 const bool continued=c.query(attack_is_attacking)!=0;
 if(continued&&s.last)return 0;
 c.log(continued?0x3d0294:0x3d03c0);s.continued=continued?1:0;
 if(continued||!requested){
  auto* list=reinterpret_cast<AttackTargetList24*>(c.call(attack_list_create,s.owner,0,1,1).identity);
  if(!list_valid(list))return 2;
  const bool narrow=s.heading_active!=0;
  bool retain=false;
  if(continued&&!narrow&&s.target&&c.query(attack_can_attack_current)){
   if(!s.target){c.call(attack_list_destroy,list->token,reinterpret_cast<std::uintptr_t>(list));return 2;}
   if(!c.query(attack_target_dead,s.target)){
    retain=true;if(c.query(attack_owner_player))c.log(0x3d06b4);
   }
  }
  int result=0;
  if(!retain)result=search(c,list,narrow);
  if(!result){
   if(!list_valid(list))result=2;
   else if(list->cursor<list->count){
    c.set_target(list->entries[list->cursor],0);
    if(c.log(continued?0x3d0354:0x3d0630)&&c.query(attack_owner_player))if(!pop_all(c,list))result=2;
   }
  }
  c.call(attack_list_destroy,list->token,reinterpret_cast<std::uintptr_t>(list));
  if(result)return result;
  if(continued)return 0;
 }else c.set_target(requested,speculative);
 if(!s.target&&s.object_of_interest_type==8&&!s.heading_active){
  if(c.query(attack_owner_player))c.log(0x3d0538);
  s.seeking=1;c.set_target(s.object_of_interest,0);c.call(attack_sync_last_target,s.owner);
 }
 if(speculative)return 0;
 if(s.target&&!c.query(attack_current_in_melee))return 0;
 c.call(attack_set_attack_state,s.owner,s.target,0);return 0;
}
}
extern "C" int dh2_character_ai_melee_attack(dh2::character::AttackState64* state,std::uintptr_t target,std::uint32_t speculative,const dh2::character::AttackServices16* services){
 if(!valid(state)||!services_valid(services)||speculative>255)return 1;
 Calls calls{state,nullptr,services};return melee(calls,target,speculative);
}
extern "C" int dh2_character_cmd_attack(dh2::character::ControllerAttackState32* controller,dh2::character::AttackState64* state,std::uintptr_t target,const dh2::character::AttackServices16* services){
 using namespace dh2::character;
 if(!aligned(controller,alignof(ControllerAttackState32))||!controller->controllable||!services_valid(services)||controller->blocked>255||controller->locked>255||controller->forced>255||controller->network_enabled>255)return 1;
 if(!controller->forced&&(controller->blocked||controller->locked))return 0;
 Calls calls{state,controller,services};
 if(calls.call(attack_network_mode).word&&controller->network_enabled&&controller->character){
  if(!valid(state)||state->owner!=controller->character)return 2;
  const auto old_target=state->target,old_last=state->last_target;const auto old_continued=state->continued;
  const int result=melee(calls,target,1);if(result)return result;
  bool send=state->target!=old_target||state->continued!=old_continued;
  if(!send)send=!calls.query(attack_is_attacking)&&!state->continued;
  calls.set_target(old_last,1);calls.call(attack_sync_last_target,state->owner);calls.set_target(old_target,1);state->continued=old_continued;
  if(send)calls.call(attack_network_send,controller->character,target,0);
 }
 calls.call(attack_controllable_dispatch,controller->controllable,target);return 0;
}
