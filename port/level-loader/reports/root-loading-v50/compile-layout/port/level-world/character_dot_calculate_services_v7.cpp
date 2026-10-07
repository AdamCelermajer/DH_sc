#include "character_dot_calculate_services_v7.hpp"
namespace dh2::character::skills {
namespace {
int invoke(void* p,DotActor32*,const DotRequest40* q,DotResponse8* out,data::CombatResult*) {
 if(!p||!q||!out)return -1;
 const auto& service=*static_cast<const SkillAttackNativeServicesV6*>(p);
 if(!service.invoke)return -1;
 auto call=[&](unsigned op,std::uintptr_t subject,const char* name,std::uintptr_t& result){
  SkillAttackNativeRequestV6 request{op,0,subject,name};
  return service.invoke(service.context,&request,&result);
 };
 std::uintptr_t result{},token{};
 switch(q->service) {
 case dot_debug_load:
  return call(skill_attack_debug_load_v6,0,nullptr,result);
 case dot_debug_query:
  if(call(skill_attack_string_construct_v6,0,q->name,token)||!token)return -1;
  if(call(skill_attack_debug_get_v6,token,nullptr,result))return -1;
  out->word=std::int32_t(result);
  return call(skill_attack_string_destroy_v6,token,nullptr,result);
 case dot_profile_begin:return dh2_skill_combat_empty_native_v6(0x3136b4);
 case dot_profile_end:return dh2_skill_combat_empty_native_v6(0x3136b8);
 default:return -1; // Calculation adapter cannot fabricate application services.
 }
}
}
DotServices16 dot_calculate_services_v7(const SkillAttackNativeServicesV6& debug) {
 return {const_cast<SkillAttackNativeServicesV6*>(&debug),invoke};
}
}
