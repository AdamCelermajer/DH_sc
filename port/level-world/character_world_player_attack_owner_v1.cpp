#include "character_world_player_attack_owner_v1.hpp"
#include <cstring>
namespace dh2::character::skills {
CharacterWorldPlayerAttackOwnerV1::CharacterWorldPlayerAttackOwnerV1(
 CharacterWorldRuntimeV1& w,AttackState64& f,TargetState48& t,
 ControllerAttackState32& c,State& m,TargetServices16 ts,
 PlayerAttackBackendsV1 b,std::uint32_t capacity):world_(w),fields_(f),target_(t),
 controller_(c),machine_(m),target_services_(ts),backends_(b),heap_(capacity){
 ordered_.reserve(capacity);
}
void CharacterWorldPlayerAttackOwnerV1::refresh(AttackState64& s){
 s.target=target_.target;s.last_target=target_.last_target;s.owner_flags528=machine_.attack_gate;
 s.heading_active=machine_.heading_active;s.last=fields_.last;s.index=fields_.index;
 s.finisher=fields_.finisher;s.object_of_interest=fields_.object_of_interest;
 s.object_of_interest_type=fields_.object_of_interest_type;
 s.continued=fields_.continued;s.seeking=fields_.seeking;
}
void CharacterWorldPlayerAttackOwnerV1::publish(const AttackState64& s){
 fields_.continued=s.continued;fields_.seeking=s.seeking;
 refresh(fields_);
}
void CharacterWorldPlayerAttackOwnerV1::fail(const char* what,std::uint32_t op){
 if(error_.empty())error_=std::string(what)+" (attack service "+std::to_string(op)+")";
}
bool CharacterWorldPlayerAttackOwnerV1::valid_binding(){
 if(!fields_.owner||!target_.owner||target_.owner->identity!=fields_.owner||
  controller_.character!=fields_.owner||world_.state_borrow(fields_.owner)!=&machine_||
  heap_.empty()||heap_.size()>65536){
  error_="Player attack binding requires the same registered Character, Target owner, controller and FSM";
  return false;
 }
 return true;
}
bool CharacterWorldPlayerAttackOwnerV1::query(void* p,std::uintptr_t owner,
 std::uintptr_t target,WorldAIAttackQueryV1 op,std::int32_t& value,std::string& error){
 auto& self=*static_cast<CharacterWorldPlayerAttackOwnerV1*>(p);
 if(op==WorldAIAttackQueryV1::IsEnemy){
  std::uintptr_t result{};
  if(self.world_.relationship(owner,target,true,&result)){
   error="Required whole source world enemy query failed";return false;
  }
  value=static_cast<std::int32_t>(result);return true;
 }
 const auto& services=self.backends_.queries;
 if(!services.query){error="Required actual equipment/attack geometry producer missing";return false;}
 return services.query(services.context,owner,target,op,value,error);
}
int CharacterWorldPlayerAttackOwnerV1::search_service(void* p,
 const target_search::Request24* q,target_search::Response16* out){
 auto& self=*static_cast<CharacterWorldPlayerAttackOwnerV1*>(p);
 if(!q||!out)return -1;
 if(q->service==target_search::melee_radius){
  std::string error;
  if(!self.backends_.melee_radius||!self.backends_.melee_radius(self.backends_.context,q->subject,out->number,error)){
   self.fail(error.empty()?"Required source inventory plus AI search radius":error.c_str(),attack_list_search);return -1;
  }
  return 0;
 }
 auto services=self.world_.targets().search_services();
 return services.invoke(services.context,q,out);
}
void CharacterWorldPlayerAttackOwnerV1::invoke(void* p,AttackState64* s,
 ControllerAttackState32*,const AttackRequest32* q,AttackResponse16* out){
 auto& self=*static_cast<CharacterWorldPlayerAttackOwnerV1*>(p);
 if(!s||!q||!out)return;
 // The recovered ABI is void: failures are latched, never thrown across it.
 // Stop subsequent source mutations; the caller reports the executed prefix.
 *out={};if(!self.error_.empty())return;
 self.publish(*s);self.call(*s,*q,*out);self.refresh(*s);
}
void CharacterWorldPlayerAttackOwnerV1::call(AttackState64& s,
 const AttackRequest32& q,AttackResponse16& out){
 target_search::Services16 services{this,search_service};
 switch(q.service){
 case attack_owner_ranged:case attack_current_in_melee:{
  std::int32_t result{};std::string error;
  if(!query(this,s.owner,s.target,q.service==attack_owner_ranged?
    WorldAIAttackQueryV1::CharacterCanRangeAttack:WorldAIAttackQueryV1::IsInMeleeRange,result,error)){
   fail(error.empty()?"Required attack predicate failed":error.c_str(),q.service);
  }else out.word=static_cast<std::uint32_t>(result);
  break;
 }
 case attack_can_attack_current:{
  bool result{};std::string error;
  if(!world_ai_can_attack_v1(s.owner,q.payload,target_.target,{this,query},result,error)){
   fail(error.empty()?"Required whole AI_CanAttack failed":error.c_str(),q.service);
  }else out.word=result?1:0;
  break;
 }
 case attack_owner_dead:case attack_target_dead:case attack_owner_player:{
  target_search::Response16 result{};
  const auto id=q.service==attack_target_dead?q.payload:s.owner;
  const target_search::Request24 query{q.service==attack_owner_player?
   target_search::is_player:target_search::is_dead,0,id,0};
  if(services.invoke(services.context,&query,&result))fail("Required world predicate failed",q.service);
  else out.word=static_cast<std::uint32_t>(result.word);
  break;
 }
 case attack_is_attacking:
  // Original IsAttacking tests the SAME current source state ID5.
  out.word=machine_.current==5;break;
 case attack_list_create:{
  WorldTargetActorBorrowV1 actor{};
  if(world_.refresh()||world_.actor(s.owner,&actor)||!actor.search||heap_.empty()||
   dh2_target_list_init(&list_,heap_.data(),static_cast<std::uint32_t>(heap_.size()),
    actor.search,1,&services)){fail("Required source target list initialization failed",q.service);break;}
  ordered_.clear();view_={reinterpret_cast<std::uintptr_t>(this),nullptr,0,0};
  out.identity=reinterpret_cast<std::uintptr_t>(&view_);break;
 }
 case attack_list_reset_sort:list_.count=0;list_.sort=0;break;
 case attack_list_search:{
  float radius{},angle{};std::memcpy(&radius,&q.argument0,4);std::memcpy(&angle,&q.argument1,4);
  if(dh2_target_search(&list_,&world_.registry(),radius,angle,&services)){
   fail("Required source world target search failed",q.service);break;
  }
  ordered_.clear();target_search::Target24 selected{};
  while(list_.count){if(dh2_target_pop(&list_,&selected)){fail("Required target heap pop failed",q.service);break;}
   ordered_.push_back(selected.identity);}
  view_.entries=ordered_.data();view_.count=static_cast<std::uint32_t>(ordered_.size());view_.cursor=0;break;
 }
 case attack_list_pop:if(view_.cursor<view_.count)++view_.cursor;break;
 case attack_list_destroy:ordered_.clear();view_={};list_.count=0;break;
 case attack_set_target:
  if(dh2_character_ai_set_target(&target_,q.payload,q.argument0,&target_services_))
   fail("Required source SetTarget provider failed",q.service);
  break;
 case attack_sync_last_target:
  if(dh2_character_ai_sync_last_target(&target_))fail("Source BackupTarget failed",q.service);
  break;
 case attack_controllable_dispatch:
  if(controller_.character==s.owner){
   AttackServices16 nested{this,invoke};
   if(dh2_character_ai_melee_attack(&s,q.payload,0,&nested))fail("Source melee dispatch failed",q.service);
   break;
  }
  [[fallthrough]];
 default:
  if(!backends_.invoke||backends_.invoke(backends_.context,&q,&out))
   fail("Required player attack backend unavailable",q.service);
  break;
 }
}
int CharacterWorldPlayerAttackOwnerV1::command(std::uintptr_t requested){
 if(active_)return 1;
 error_.clear();if(!valid_binding())return 1;
 active_=true;AttackState64 projection=fields_;refresh(projection);
 AttackServices16 services{this,invoke};
 const int rc=dh2_character_cmd_attack(&controller_,&projection,requested,&services);
 if(error_.empty())publish(projection);
 active_=false;return !error_.empty()?2:rc;
}
int CharacterWorldPlayerAttackOwnerV1::melee(std::uintptr_t requested,std::uint32_t speculative){
 if(active_)return 1;
 error_.clear();if(!valid_binding())return 1;
 active_=true;AttackState64 projection=fields_;refresh(projection);
 AttackServices16 services{this,invoke};
 const int rc=dh2_character_ai_melee_attack(&projection,requested,speculative,&services);
 if(error_.empty())publish(projection);
 active_=false;return !error_.empty()?2:rc;
}
}
