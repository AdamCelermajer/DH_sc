#pragma once

// Optional record adapter. Kept separate from the portable binder header
// because SourceCharacterOwnerFactory's canonical record imports the native
// scene/physics graph. Platform code includes this header when it has those
// dependencies and passes its typed FSM-loan type as a template argument.
#include "canonical_session_skill_binding.hpp"
#include "../actor_frame/source_character_owner_factory.hpp"

#include <exception>
#include <memory>

namespace dh::foundation::skills_animation {

template<class Loan>
bool validate_canonical_session_skill_loan(
 const dh::foundation::features::SourceCharacterOwnerAliases& aliases,
 const Loan& loan,std::string& error){
 using namespace dh2::character::skills;
 if(!aliases.validate(error))return false;
 const bool hasV1=loan.instances_v1!=nullptr,hasV3=loan.instances_v3!=nullptr;
 const bool sessionV1=loan.session_v1!=nullptr,sessionV3=loan.session_v3!=nullptr;
 if(!loan.record_lease||loan.record_lease.get()!=aliases.lifetime.get()||
    !loan.context_lease||!loan.character||loan.character!=aliases.identity||
    !loan.machine||loan.machine!=aliases.fsm||loan.machine->character!=aliases.identity||
    loan.machine->reserved||!loan.machine->state||
    !loan.property_view||loan.property_view!=aliases.property_view||
    !loan.skill_tables||!loan.ai||!loan.state||
    hasV1==hasV3||sessionV1==sessionV3||hasV1!=sessionV1||hasV3!=sessionV3||
    !loan.ai_services.invoke||!loan.state_services.invoke){
  error="Required same completed Character/Session/FSM/skill loan";return false;
 }
 State40* slots{};
 if(hasV3){
  if(!aliases.player_skills||loan.instances_v3!=aliases.player_skills->native_skill_owner()||
     loan.session_v3!=&aliases.player_skills->session()||
     !aliases.player_skills->ready()||
     loan.session_v3->properties().get()!=aliases.properties||
     &loan.session_v3->property_view()!=aliases.property_view){
   error="Required same ready player Session/SkillOwner/property graph";return false;
  }
  slots=const_cast<State40*>(&loan.instances_v3->state());
 }else{
  if(aliases.player_skills||!aliases.lifetime->npc_skills_v84||
     !aliases.lifetime->actor||!aliases.lifetime->actor->session||
     loan.instances_v1!=&aliases.lifetime->npc_skills_v84->owner()||
     loan.session_v1!=aliases.lifetime->actor->session.get()||
     loan.session_v1->properties().get()!=aliases.properties||
     &loan.session_v1->property_view()!=aliases.property_view){
   error="Required same NPC Session/SkillOwner/property graph";return false;
  }
  slots=const_cast<State40*>(&loan.instances_v1->state());
 }
 dh2::character::ScriptSessionView active{};
 const bool activeSession=hasV3?loan.session_v3->owner().active(active):
  loan.session_v1->owner().active(active);
 if(!activeSession||loan.machine->state!=loan.state->state||
    loan.state->character!=aliases.identity||
    loan.state->physical!=aliases.character->position_fields_v7().physical2dc||
    loan.state->target!=aliases.character->object->target.target||
    loan.state->last_target!=aliases.character->object->target.last_target||
    loan.ai->owner!=&aliases.lifetime->skill_owner_view_v68||
    loan.ai->owner->character!=aliases.identity||loan.ai->owner->reserved||
    loan.ai->slots!=slots||loan.ai->fields==nullptr||slots->owner!=aliases.identity||
    loan.ai->owner->flags!=loan.machine->state->flags){
  error="Skill context projections do not match the same live Character/FSM";return false;
 }
 error.clear();return true;
}

template<class ResolveIdentity,class Loan>
bool make_canonical_session_skill_actor(CombatSession& host,ActorId hostActor,
 const dh::foundation::features::SourceCharacterOwnerAliases& aliases,
 const Loan& loan,ResolveIdentity resolveIdentity,SkillActorBorrow& out,
 std::string& error){
 std::uintptr_t mapped{};
 if(!resolveIdentity(hostActor,mapped,error)||mapped!=aliases.identity||
    !validate_canonical_session_skill_loan(aliases,loan,error)){
  if(error.empty())error="Host ActorId mapping does not select the completed canonical Character";
  return false;
 }
 auto* actor=host.actor(hostActor);
 if(!actor||!host.retained_actor_pose(hostActor)){
  error="CombatSession lacks the mapped actor's retained pose owner";return false;
 }
 out={actor,aliases.property_view,loan.ai,loan.state};
 error.clear();return true;
}

// Build NativeSkillLuaServices directly from an authenticated completed
// source loan. Keep the input loan pinned while the returned adapter is in use;
// the refresher overload below retains fresh loans during each callback.
template<class Loan>
bool bind_canonical_native_skill_services(
 SessionSkillServices& services,
 const dh::foundation::features::SourceCharacterOwnerAliases& aliases,
 const Loan& loan,
 std::unique_ptr<NativeSkillLuaServicesV1>& npc,
 std::unique_ptr<NativeSkillLuaServices>& player,
 std::string& error){
 npc.reset();player.reset();
 if(!validate_canonical_session_skill_loan(aliases,loan,error))return false;
 auto* machine=loan.machine;auto* owner=loan.ai->owner;
 auto flags=[machine,owner](std::uint32_t& out){
  if(!machine||!machine->state||machine->reserved||!owner||
     machine->character!=owner->character||owner->flags!=machine->state->flags)return false;
  out=machine->state->flags;return true;
 };
 try{
  if(loan.instances_v1){
   npc=std::make_unique<NativeSkillLuaServicesV1>(*loan.session_v1,
    *loan.instances_v1,loan.skill_tables,*loan.machine,*loan.ai,*loan.state,
    loan.ai_services,loan.state_services,flags);
   bind_canonical_session_skill_services(services,*npc,*loan.session_v1);
  }else{
   player=std::make_unique<NativeSkillLuaServices>(*loan.session_v3,
    *loan.instances_v3,*loan.machine,*loan.ai,*loan.state,
    loan.ai_services,loan.state_services,flags);
   bind_canonical_session_skill_services(services,*player,*loan.session_v3);
  }
  bind_session_skill_pose_services(services);
 }catch(const std::exception& exception){
  npc.reset();player.reset();error=exception.what();return false;
 }
 error.clear();return true;
}

// Fresh typed context borrowing for SessionSkillAnimation. The mapping
// callback explicitly translates host ActorId to native Character identity;
// no integer-domain equality is assumed. Completed InitPost/Final and the
// current owner graph are authenticated on every borrow. The closure retains
// the newest record/FSM pins for synchronous use.
template<class Loan,class ResolveIdentity,class BorrowContext>
std::function<bool(CombatSession&,ActorId,SkillActorBorrow&,std::string&)>
make_canonical_session_skill_borrower(CombatSession& host,ActorId hostActor,
 const dh::foundation::features::SourceCharacterOwnerFactory& factory,
 ResolveIdentity resolveIdentity,BorrowContext borrowContext){
 struct Pins {Loan loan{};std::uintptr_t identity{};};
 auto pins=std::make_shared<Pins>();
 return [&host,hostActor,&factory,pins,
  resolveIdentity=std::move(resolveIdentity),borrowContext=std::move(borrowContext)]
  (CombatSession& current,ActorId actor,SkillActorBorrow& out,std::string& error)mutable{
   if(&current!=&host||actor!=hostActor){error="Foreign CombatSession skill actor";return false;}
   std::uintptr_t identity{};
   if(!resolveIdentity(actor,identity,error)||!identity)return false;
   if(pins->identity&&pins->identity!=identity){
    error="Host ActorId now maps to another canonical Character; explicit rebind required";
    return false;
   }
   dh::foundation::features::SourceCharacterOwnerAliases aliases;
   if(!factory.borrow_completed_character(identity,aliases,error))return false;
   Loan refreshed{};
   if(!borrowContext(aliases.lifetime,refreshed,error)||
      !validate_canonical_session_skill_loan(aliases,refreshed,error))return false;
   auto* sessionActor=host.actor(actor);
   if(!sessionActor||!host.retained_actor_pose(actor)){
    error="CombatSession lacks the mapped actor's retained pose owner";return false;
   }
   pins->loan=std::move(refreshed);pins->identity=identity;
   out={sessionActor,aliases.property_view,pins->loan.ai,pins->loan.state};
   error.clear();return true;
  };
}

}
