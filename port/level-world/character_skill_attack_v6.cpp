#include "character_skill_combat_v6.hpp"
namespace dh2::character::skills {
namespace {
struct NativeRun {
 const SkillAttackNativeServicesV6& services;
 bool call(std::uint32_t operation,std::uintptr_t subject,const char* name,std::uintptr_t& out){
  SkillAttackNativeRequestV6 request{operation,0,subject,name};out=0;
  return services.invoke(services.context,&request,&out)==0;
 }
 bool debug(){
  std::uintptr_t out=0,token=0;
  if(!call(skill_attack_debug_load_v6,0,nullptr,out)||
     !call(skill_attack_string_construct_v6,0,"isTracingChar_Attack",token)||!token||
     !call(skill_attack_debug_get_v6,token,nullptr,out))return false;
  return call(skill_attack_string_destroy_v6,token,nullptr,out);
 }
 static int continuation(void* p){return static_cast<NativeRun*>(p)->debug()?0:-1;}
};
bool actor(const SkillAttackActorV6* a){return a&&a->identity&&a->properties&&a->facts&&a->facts->properties==a->properties->resolved&&!dh2_property_validate(a->properties);}
}
extern "C" int dh2_character_skill_attack_calculate_v6(data::CombatResult* result,
 DotCombatContext32* context,data::CombatRandom* random,const SkillAttackActorV6* attacker,
 const SkillAttackActorV6* defender,const data::FreshInventoryOwnedV4* inventory,
 std::uint32_t mask,std::int32_t element,const SkillAttackNativeServicesV6* services){
 if(!result||!context||!random||!actor(attacker)||!actor(defender)||!inventory||
    inventory->character()!=attacker->identity||!services||!services->invoke)return -1;
 NativeRun run{*services};if(!run.debug())return -2;
 mask|=0x8000000u;const bool offhand=mask&0x4000000u;
 // Original GetEquippedItem then nullable ItemInstance::GetItem word37.
 const auto selected=inventory->current_equipment();const auto slot=inventory->equipment()[selected][offhand?2:1];
 std::int32_t category=-1;
 if(slot&&slot->item){const auto record=data::item(inventory->table(),slot->item->id);if(!record)return -2;category=record->record.words[37];}
 dh2_skill_combat_empty_native_v6(0x3136b4);
 *result=data::CombatResult{};result->mask=mask;result->weapon_category=category;result->element=element;
 const auto level_delta=std::int32_t(std::uint32_t(attacker->properties->resolved[19])-std::uint32_t(defender->properties->resolved[19]));
 *context={attacker->identity,defender->identity,level_delta,std::int32_t(0u-std::uint32_t(level_delta)),element,std::uint8_t(offhand),1,0,0};
 data::CombatResultRequest request{attacker->facts,defender->facts,random,mask,category,element,0};
 if(dh2_combat_result_ordered_v6(result,&request,&run,NativeRun::continuation))return -2;
 dh2_skill_combat_empty_native_v6(0x3136b8);
 return 0;
}
extern "C" int dh2_skill_combat_empty_native_v6(std::uint32_t address){
 switch(address){case 0x3136b4:case 0x3136b8:case 0x3790e0:case 0x379104:
 case 0x3790d4:case 0x3790d8:case 0x3790dc:return 0;default:return -1;}
}
}
