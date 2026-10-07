#include "character_skill_native_v5.hpp"
#include <cstring>
namespace {using namespace dh2::character::skills;
struct ManaRun {
 SkillManaOwnerV5& owner;const SkillManaServicesV5& services;
 bool call(unsigned op,const char* name,SkillManaResponseV5& answer,std::uintptr_t subject=0){
  const SkillManaRequestV5 request{op,0,subject?subject:owner.character,name};answer={};
  return !services.invoke(services.context,&request,&answer)&&!answer.reserved;
 }
 int bypass(){SkillManaResponseV5 r;if(!call(mana_application_v5,nullptr,r))return -1;
  if(!r.word)return 0;if(!call(mana_player_v5,nullptr,r))return -1;return r.word?1:0;}
 bool query(const char* name,std::uint32_t& out){SkillManaResponseV5 r;
  if(!call(mana_debug_load_v5,nullptr,r)||!call(mana_debug_construct_v5,name,r)||!r.identity)return false;
  const auto identity=r.identity;
  if(!call(mana_debug_get_v5,nullptr,r,identity))return false;out=r.word;
  return call(mana_debug_destroy_v5,nullptr,r,identity);
 }
 int has(std::int32_t cost,std::uint32_t& out){const int bypassed=bypass();if(bypassed<0)return -2;
  if(bypassed){out=1;return 0;}if(cost<0)return -2;
  std::int32_t mana;if(dh2_property_resolve(owner.properties,41,&mana))return -2;
  out=cost<=mana;return 0;}
 int use(std::int32_t cost,std::uint32_t& out){const int bypassed=bypass();if(bypassed<0)return -2;
  if(bypassed){out=1;return 0;}if(cost<0)return -2;
  SkillManaResponseV5 r;if(!call(mana_debug_contains_v5,"GOD_MANA",r))return -2;
  if(r.word){out=1;return 0;}
  std::uint32_t debug;if(!query("GOD_MANA",debug))return -2;
  if(debug||owner.byte14f0){out=1;return 0;}
  auto status=has(cost,out);if(status||!out)return status;
  const auto delta=static_cast<std::int32_t>(0u-static_cast<std::uint32_t>(cost));
  if(dh2_property_add(owner.properties,41,delta))return -2;
  if(!query("isTracingChar_Stats",debug))return -2;out=1;return 0;
 }
};}
extern "C" int dh2_character_skill_mana_v5(std::uint32_t* out,SkillManaOwnerV5* owner,
 std::uint32_t use,std::int32_t cost,const SkillManaServicesV5* services){
 if(!out||!owner||!owner->character||!owner->properties||dh2_property_validate(owner->properties)||
  !services||!services->invoke||use>1)return -1;
 for(auto byte:owner->reserved)if(byte)return -1;
 ManaRun run{*owner,*services};std::uint32_t result=0;
 const auto status=use?run.use(cost,result):run.has(cost,result);
 if(!status)*out=result;return status;
}
