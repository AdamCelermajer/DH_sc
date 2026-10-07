#include "character_skill_combat_v6.hpp"
#include <cmath>
#include <limits>
namespace dh2::character::skills {
namespace {
std::uint32_t unsigned_number(double n){
 if(std::isnan(n)||n<=0)return 0;
 if(n>=4294967295.0)return UINT32_MAX;
 return std::uint32_t(n);
}
struct Run {
 SkillCombatOutputV6& out;std::uintptr_t attacker;const SkillCombatServicesV6& services;
 bool call(std::uint32_t service,SkillCombatResponseV6& response,
  std::uint32_t index=0,std::uintptr_t target=0,std::uint32_t mask=0,
  std::int32_t element=0,data::CombatResult* result=nullptr){
  ++out.calls;out.phase=service;response={};
  SkillCombatRequestV6 request{service,index,mask,0,attacker,target,element,0};
  return services.invoke(services.context,&request,&response,result)==0;
 }
 bool attack(const SkillCombatRowV6* row,std::uintptr_t target,std::uint32_t mask){
  SkillCombatResponseV6 response{};data::CombatResult result{};
  if(!call(skill_combat_calculate_v6,response,0,target,mask,row->element,&result)||
     !call(skill_combat_apply_v6,response,0,target,0,0,&result))return false;
  out.amount[out.count++]=result.amount;return true;
 }
};
}
extern "C" int dh2_character_skill_combat_roll_v6(SkillCombatOutputV6* out,
 std::uintptr_t attacker,const dh2_script_value* args,std::uint32_t count,
 const SkillCombatServicesV6* services){
 if(!out||!attacker||!services||!services->invoke||(count&&!args))return -1;
 *out={};
 if(count<2||args[0].type!=DH2_SCRIPT_NUMBER)return 0;
 const auto index=unsigned_number(args[0].number);Run run{*out,attacker,*services};
 SkillCombatResponseV6 response{};
 if(!run.call(skill_combat_list_v6,response))return -2;
 if(index>=response.word)return 0;
 if(args[1].type!=DH2_SCRIPT_IDENTITY&&args[1].type!=DH2_SCRIPT_SOURCE_OBJECT)return 0;
 const auto object=args[1].identity;if(!object)return 0;
 if(!run.call(skill_combat_handle_v6,response,0,object))return -2;
 const auto handle=response.identity;
 if(!run.call(skill_combat_character_v6,response,0,handle))return -2;
 const auto target=response.identity;
 if(!target){
  if(!run.call(skill_combat_target_kind_v6,response,0,object))return -2;
  if(response.word!=8)return 0;
  if(!run.call(skill_combat_target_activate_v6,response,0,object))return -2;
  out->boolean_count=1;return 0; // original pushes false, not damage0
 }
 if(!run.call(skill_combat_row_v6,response,index)||!response.row)return -2;
 const auto row=response.row;const auto mask=row->mask;
 if(!(mask&0x800000u))return run.attack(row,target,mask)?0:-2;
 if(!run.call(skill_combat_main_hand_v6,response))return -2;
 if(response.word&&!run.attack(row,target,mask))return -2;
 if(!run.call(skill_combat_off_hand_v6,response))return -2;
 if(response.word&&!run.attack(row,target,mask|0x4000000u))return -2;
 return 0;
}
}
