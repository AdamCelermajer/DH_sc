#include "character_melee_animation_event_v1.hpp"
#include <cstring>
namespace dh2::character::skills {
namespace {
struct DebugRun {
 const SkillAttackNativeServicesV6& service;
 bool debug(const char* name){
  std::uintptr_t out=0,token=0;
  auto call=[&](unsigned op,std::uintptr_t id,const char* text,std::uintptr_t& value){SkillAttackNativeRequestV6 q{op,0,id,text};return service.invoke(service.context,&q,&value)==0;};
  if(!call(skill_attack_debug_load_v6,0,nullptr,out)||!call(skill_attack_string_construct_v6,0,name,token)||!token||!call(skill_attack_debug_get_v6,token,nullptr,out))return false;
  return call(skill_attack_string_destroy_v6,token,nullptr,out);
 }
 static int calculate(void* p){return static_cast<DebugRun*>(p)->debug("isTracingChar_Attack")?0:-1;}
};
bool valid(const SkillAttackActorV6* a){return a&&a->identity&&a->properties&&a->facts&&a->facts->properties==a->properties->resolved&&!dh2_property_validate(a->properties);}
}
extern "C" int dh2_character_melee_animation_event_v1(MeleeAnimationOutputV1* o,const char* text,const MeleeAnimationServicesV1* s){
 if(!o)return -1;
 *o={};if(!text||!s||!s->invoke)return o->status=-1;
 auto call=[&](unsigned op,const char* name,int index,int step,bool offhand,MeleeAnimationResponseV1& result){MeleeAnimationRequestV1 q{op,name,index,step,offhand};result={};++o->calls;o->phase=op;return s->invoke(s->context,&q,&result)==0;};
 MeleeAnimationResponseV1 r;
 if(!call(melee_event_step_index,nullptr,0,0,false,r))return o->status=-2;
 o->step=r.value;
 if(!call(melee_event_step_count,nullptr,0,0,false,r))return o->status=-2;
 unsigned prefix=0,skip=0;
 if(!std::strncmp(text,"ev_",3)){prefix=melee_event_relay;skip=3;}
 else if(!std::strncmp(text,"an_",3)){prefix=melee_event_object_animation;skip=3;}
 else if(!std::strncmp(text,"fx_",3)){prefix=melee_event_mesh_fx;skip=3;}
 else if(!std::strncmp(text,"sfx_",4)){prefix=melee_event_sound_fx;skip=4;}
 if(prefix)return o->status=call(prefix,text+skip,0,0,false,r)?0:-2;
 if(!call(melee_event_state,nullptr,0,0,false,r))return o->status=-2;
 o->state=r.value;
 unsigned operation=0;bool offhand=false;int projectile=0;
 if(o->state==5){
  if(!call(melee_event_can_range,nullptr,0,0,false,r))return o->status=-2;
  if(r.value){if(std::strcmp(text,"attack_ranged")&&std::strcmp(text,"attack_mainhand")&&std::strcmp(text,"do_skill"))return 0;operation=melee_event_projectile;projectile=r.projectile;}
  else if(!std::strcmp(text,"attack_mainhand"))operation=melee_event_attack;
  else if(!std::strcmp(text,"attack_offhand")){operation=melee_event_attack;offhand=true;}
 }else if(o->state==6&&!std::strcmp(text,"do_skill"))operation=melee_event_skill;
 else if(o->state==7&&!std::strcmp(text,"do_spell"))operation=melee_event_spell;
 else if(o->state==13&&!std::strcmp(text,"interact"))operation=melee_event_interact;
 if(!operation)return 0;
 if(!s->debug||!s->debug->invoke||!DebugRun{*s->debug}.debug("isTracingCharAI_AnimEvent"))return o->status=-2;
 // Attack index is AI+74, supplied by the actual animator callback backend;
 // index here is only the projectile ID. Source step argument is index-1.
 return o->status=call(operation,text,projectile,o->step-1,offhand,r)?0:-2;
}
extern "C" int dh2_character_melee_calculate_v1(data::CombatResult* result,DotCombatContext32* context,data::CombatRandom* random,const SkillAttackActorV6* a,const SkillAttackActorV6* b,const data::FreshInventoryOwnedV4* inventory,bool offhand,bool critical,const SkillAttackNativeServicesV6* debug,const MeleeCriticalClassServicesV1* temporary){
 if(!result||!context||!random||!valid(a)||!valid(b)||!inventory||inventory->character()!=a->identity||!debug||!debug->invoke)return -1;
 DebugRun run{*debug};if(!run.debug("isTracingChar_Attack"))return -2;
 const auto selected=inventory->current_equipment();if(selected<0||std::size_t(selected)>=inventory->equipment().size())return -1;
 const auto slot=inventory->equipment()[selected][offhand?2:1];int category=-1;
 if(slot&&slot->item){const auto row=data::item(inventory->table(),slot->item->id);if(!row)return -2;category=std::int32_t(row->record.words[37]);}
 if(critical&&(!temporary||!temporary->apply||temporary->apply(temporary->context,a->properties)))return -2;
 const unsigned mask=(critical?0x0005554au:0x0022aab5u)|(offhand?0x04000000u:0u);
 const auto delta=std::int32_t(std::uint32_t(a->properties->resolved[19])-std::uint32_t(b->properties->resolved[19]));
 *context={a->identity,b->identity,delta,std::int32_t(0u-std::uint32_t(delta)),-1,std::uint8_t(offhand),1,0,0};
 *result={};result->mask=mask;result->weapon_category=category;result->element=-1;
 data::CombatResultRequest q{a->facts,b->facts,random,mask,category,-1,0};
 return dh2_combat_result_ordered_v6(result,&q,&run,DebugRun::calculate)?-2:0;
}
CharacterWorldMeleeAttackV1::CharacterWorldMeleeAttackV1(CharacterWorldSkillCombatV6& combat,CharacterWorldRuntimeV1& world,DotCombatContext32& context,data::CombatRandom& random,WorldMeleeAttackBorrowV1 borrow,WorldAIAttackServicesV1 queries,WorldMeleeAttackBackendsV1 backends):combat_(combat),world_(world),context_(context),random_(random),borrow_(borrow),queries_(queries),backends_(backends){}
int CharacterWorldMeleeAttackV1::attack(int index,int step,bool offhand){
 error_.clear();bool allowed=false;
 if(!borrow_.owner||!borrow_.target||!borrow_.active_ais||!borrow_.on_attack_address||!borrow_.inventory){error_="Required source melee AI/Character borrow";return -1;}
 if(!world_ai_can_attack_v1(borrow_.owner,0,*borrow_.target,queries_,allowed,error_))return -2;
 if(!allowed||!*borrow_.active_ais)return 0;
 const auto method=*borrow_.on_attack_address;
 if(method!=0x3dc12c&&method!=0x3dd7ec){
  if(backends_.remaining_ais&&backends_.remaining_ais(backends_.context,*borrow_.active_ais,index,step,offhand)==0)return 0;
  error_="Required selected AIS OnAttack provider";return -2;
 }
 WorldTargetActorBorrowV1 actor{};
 // AI_GetTargetAsCharacter resolves the actual registered handle. A known
 // non-character target takes raw Character+408 Activate, never melee.
 const auto target=*borrow_.target;
 if(!target||world_.actor(target,&actor)!=0||!actor.character){
  if(!borrow_.look_at){error_="Required source Character look target field";return -2;}
  if(!*borrow_.look_at)return 0;
  if(backends_.activate&&backends_.activate(backends_.context,*borrow_.look_at,borrow_.owner)==0)return 0;
  error_="Required source non-character Activate";return -2;
 }
 auto native=combat_.native_world();SkillAttackActorV6 *attacker=nullptr,*defender=nullptr;SkillApplyActorV6 *application=nullptr;
 if(!native.actor||native.actor(native.context,borrow_.owner,&attacker,&application)||native.actor(native.context,target,&defender,&application)){error_="Required same-world melee combat actor";return -2;}
 const int calculated=dh2_character_melee_calculate_v1(&result_,&context_,&random_,attacker,defender,borrow_.inventory,offhand,false,combat_.application_services().debug,nullptr);
 if(calculated){error_="Required source F_MeleeAttack calculation provider";return calculated;}
 const int applied=combat_.apply(&application_,&result_,borrow_.owner,target);
 if(applied)error_=combat_.error().empty()?"Required source F_ApplyResult continuation":combat_.error();
 return applied;
}
}
