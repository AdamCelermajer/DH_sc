#include "character_skill_combat_v6.hpp"
#include <cstring>
namespace dh2::character::skills {
namespace {
std::int32_t bits(std::uint32_t word){std::int32_t value;std::memcpy(&value,&word,4);return value;}
bool valid(SkillApplyActorV6* a){return a&&a->identity&&a->properties&&a->combo&&a->invulnerable&&a->push_death&&a->network_id&&!dh2_property_validate(a->properties);}
struct DebugRun {
 const SkillAttackNativeServicesV6* services;
 bool call(std::uint32_t op,std::uintptr_t subject,const char* name,std::uintptr_t& value){
  if(!services||!services->invoke)return false;
  SkillAttackNativeRequestV6 request{op,0,subject,name};value=0;
  return services->invoke(services->context,&request,&value)==0;
 }
 bool begin(const char* name,std::uintptr_t& token,std::uintptr_t& value){
  return call(skill_attack_debug_load_v6,0,nullptr,value)&&call(skill_attack_string_construct_v6,0,name,token)&&token&&call(skill_attack_debug_get_v6,token,nullptr,value);
 }
 bool destroy(std::uintptr_t token){std::uintptr_t value;return call(skill_attack_string_destroy_v6,token,nullptr,value);}
 bool query(const char* name){std::uintptr_t token=0,value=0;return begin(name,token,value)&&destroy(token);}
};
struct Run {
 SkillApplyOutputV6 result{};data::CombatResult* attack;SkillApplyActorV6 *attacker,*target;const SkillApplyServicesV6& services;DebugRun debug;
 bool call(std::uint32_t operation,SkillApplyResponseV6& value,std::uintptr_t subject,std::uint32_t word=0,std::uint32_t flags=0,float number=0,const char* name=nullptr){
  result.phase=operation;++result.calls;value={};
  SkillApplyRequestV6 request{operation,word,flags,0,subject,attacker->identity,target->identity,name,number,0};
  return services.invoke(services.context,&request,&value,attack)==0;
 }
 bool tracing(){return debug.query("isTracingChar_Attack");}
 int end(SkillApplyOutputV6* out,int status){result.status=status;*out=result;return status;}
 bool reaction(std::uint32_t op,std::uint32_t word,std::uint32_t flags=0){SkillApplyResponseV6 value;return call(op,value,target->identity,word,flags)&&tracing();}
};
}
extern "C" int dh2_character_skill_regen_v6(data::PropertyView* properties,std::uint32_t mp,std::int32_t amount,const SkillAttackNativeServicesV6* services){
 if(!properties||mp>1||dh2_property_validate(properties))return -1;
 const auto current=properties->resolved[mp?41:36],maximum=properties->resolved[mp?43:38];
 if(amount<0)amount=maximum;
 if(bits(std::uint32_t(amount)+std::uint32_t(current))>maximum)amount=bits(std::uint32_t(maximum)-std::uint32_t(current));
 if(amount<=0)return 0;
 DebugRun debug{services};if(!debug.query("isTracingChar_Stats"))return -2;
 return dh2_property_add(properties,mp?41:36,amount)?-2:0;
}
extern "C" int dh2_character_skill_apply_result_v6(SkillApplyOutputV6* out,data::CombatResult* attack,
 SkillApplyActorV6* attacker,SkillApplyActorV6* target,const SkillApplyServicesV6* services){
 if(!out||!attack||!valid(attacker)||!valid(target)||!services||!services->invoke)return -1;
 Run run{{},attack,attacker,target,*services,{services->debug}};SkillApplyResponseV6 value;
 if(!run.call(skill_apply_online_v6,value,0))return run.end(out,-2);
 if(value.word)return run.end(out,-3);
 *attacker->combo=(attack->outcomes&3)?0:std::uint16_t(*attacker->combo+1);
 std::uintptr_t no_damage=0,god=0,answer=0;
 if(!run.debug.begin("NoDamages",no_damage,answer))return run.end(out,-2);
 bool suppressed=answer!=0;
 if(!suppressed){
  if(!run.debug.begin("GOD",god,answer))return run.end(out,-2);
  if(!answer){if(!run.call(skill_apply_saved_option_v6,value,0,0,0,0,"GOD"))return run.end(out,-2);answer=value.word;}
  if(answer){if(!run.call(skill_apply_is_player_v6,value,target->identity))return run.end(out,-2);suppressed=value.word!=0;}
  if(!suppressed)suppressed=*target->invulnerable!=0;
  if(!run.debug.destroy(god))return run.end(out,-2);
 }
 if(!run.debug.destroy(no_damage))return run.end(out,-2);
 if(!suppressed){
  if(attack->mask&0x400000)return run.end(out,-3);
  if(attack->amount>0){
   const auto captured_amount=attack->amount;
   if(!run.call(skill_apply_party_count_v6,value,0))return run.end(out,-2);
   if(bits(value.word)>1)return run.end(out,-3);
   const auto threat=float(attacker->properties->resolved[204])*(1.0f/256.0f);
   const auto amount=float(attack->amount)*(1.0f/256.0f);
   if(!run.call(skill_apply_aggro_v6,value,target->identity,0,0,threat*amount))return run.end(out,-2);
   if(value.number>0&&!run.debug.query("isTracingThreatChange"))return run.end(out,-2);
   *target->push_death=(attack->outcomes&128)?std::uint8_t((attack->mask>>20)&1):0;
   if(*target->network_id==-1){
    if(!run.tracing())return run.end(out,-2);
    if(!target->hit||target->hit->identity!=target->identity||target->hit->properties!=target->properties||!attacker->attacker_handle||attacker->attacker_handle->identity!=attacker->identity||!target->hit_services)return run.end(out,-2);
    if(dh2_character_hit_for_v6(&run.result.hit,target->hit,std::uint32_t(captured_amount),attacker->attacker_handle,target->hit_services)!=1)return run.end(out,-2);
   }
   if(attack->mask&0x200000){if(!run.call(skill_apply_hit_fx_v6,value,target->identity))return run.end(out,-2);}
   if(!run.call(skill_apply_is_dead_v6,value,target->identity))return run.end(out,-2);
   if(value.word)attack->outcomes&=~0x160u;
   if(attack->outcomes&8){
    if(!run.call(skill_apply_is_player_v6,value,attacker->identity))return run.end(out,-2);
    if(value.word&&!run.call(skill_apply_critical_camera_v6,value,attacker->identity))return run.end(out,-2);
   }
  }
 }
 if(dh2_character_skill_regen_v6(attacker->properties,0,attack->hp_leech,services->debug)||dh2_character_skill_regen_v6(attacker->properties,1,attack->mp_leech,services->debug))return run.end(out,-2);
 if(!run.call(skill_apply_is_dead_v6,value,target->identity))return run.end(out,-2);
 if(!value.word){
  if(attack->dot_duration>0&&attack->dot_amount>0){
   if(!target->buffs||!target->dot_ids||!target->dot_fx_ids)return run.end(out,-2);
   if(dh2_character_buff_add_dot(&run.result.dot,target->buffs,attack->dot_duration>>8,attack->dot_amount,attack->dot_element,target->dot_ids,target->dot_fx_ids)!=1)return run.end(out,-2);
  }
  const auto flags=(attack->mask&0x18000000)?1u:0u;
  if(!run.call(skill_apply_is_player_v6,value,target->identity))return run.end(out,-2);
  if(value.word)return run.end(out,-3);
  if(attack->outcomes&2){
   if(!run.call(skill_apply_dodge_v6,value,target->identity))return run.end(out,-2);
   if(!run.call(skill_apply_is_player_v6,value,target->identity))return run.end(out,-2);
   if(value.word)return run.end(out,-3);
   if(!run.tracing())return run.end(out,-2);
  }
  if(attack->outcomes&4){
   if(!run.call(skill_apply_block_v6,value,target->identity))return run.end(out,-2);
   if(!run.call(skill_apply_is_player_v6,value,target->identity))return run.end(out,-2);
   if(value.word)return run.end(out,-3);
   if(!run.tracing())return run.end(out,-2);
  }
  if((attack->outcomes&16)&&!run.reaction(skill_apply_injure_v6,0,flags))return run.end(out,-2);
  if(attack->outcomes&128){
   if(!run.call(skill_apply_push_v6,value,target->identity,(attack->mask>>20)&1,flags))return run.end(out,-2);
   if(!run.call(skill_apply_is_player_v6,value,target->identity))return run.end(out,-2);
   if(value.word)return run.end(out,-3);
   if(!run.tracing())return run.end(out,-2);
  }
  if(attack->outcomes&64){const auto duration=attacker->properties->resolved[(attack->mask&0x1000)?185:140]>>8;if(!run.reaction(skill_apply_stun_v6,std::uint32_t(duration)))return run.end(out,-2);}
  if(attack->outcomes&32){
   const auto duration=attacker->properties->resolved[(attack->mask&0x4000)?187:143]>>8;
   if(duration&&!run.call(skill_apply_scare_v6,value,target->identity,std::uint32_t(duration),flags))return run.end(out,-2);
   if(!run.tracing())return run.end(out,-2);
  }
  if(attack->outcomes&256){const auto duration=attacker->properties->resolved[(attack->mask&0x10000)?189:146]>>8;if(!run.reaction(skill_apply_slow_v6,std::uint32_t(duration)))return run.end(out,-2);}
 }
 if(!run.call(skill_apply_cancel_sneaking_v6,value,target->identity)||!run.call(skill_apply_scrolling_text_v6,value,0)||!run.call(skill_apply_sound_v6,value,0))return run.end(out,-2);
 if(!(attack->mask&0x20000000)){
  if(!run.call(skill_apply_ai_combat_v6,value,attacker->identity)||!run.call(skill_apply_ai_combat_v6,value,target->identity))return run.end(out,-2);
 }
 if(!run.call(skill_apply_is_player_v6,value,target->identity))return run.end(out,-2);
 if(value.word){if(!run.call(skill_apply_player_lookup_v6,value,target->identity))return run.end(out,-2);dh2_skill_combat_empty_native_v6(0x3790e0);}
 if(!run.call(skill_apply_is_player_v6,value,attacker->identity))return run.end(out,-2);
 if(value.word){
  if(!run.call(skill_apply_player_lookup_v6,value,attacker->identity))return run.end(out,-2);
  if(attack->outcomes&0x160)dh2_skill_combat_empty_native_v6(0x3790e0);
  if(!run.call(skill_apply_is_dead_v6,value,target->identity))return run.end(out,-2);
  if(value.word)dh2_skill_combat_empty_native_v6(0x3790e0);
  dh2_skill_combat_empty_native_v6(0x3790e0);
 }
 return run.end(out,0);
}
}
