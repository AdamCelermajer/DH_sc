#include "character_skill_callbacks_v3.hpp"
namespace {
using namespace dh2::character::skills;
bool aligned(const void* p,std::size_t a){return p&&reinterpret_cast<std::uintptr_t>(p)%a==0;}
struct Kernel {
 const Instance32* instance;const SkillCallbackServices16V3* service;
 std::uintptr_t token=0;
 int query(std::uint32_t op,SkillCallbackResponse32V3& r,std::uintptr_t active=0,const char* name=nullptr,std::uint32_t index=0){
  SkillCallbackRequest48V3 q{op,index,instance->owner,active,instance,name,token};r={};
  if(service->invoke(service->context,&q,&r)||r.reserved)return -2;
  if(op==callback_set_v3||op==callback_call_v3)token=r.results;
  return 0;
 }
 int run(std::uint32_t op,std::uint32_t& answer){
  SkillCallbackResponse32V3 r;answer=0;int code=query(callback_active_v3,r);if(code)return code;
  if(!r.active)return query(callback_release_v3,r);
  code=query(callback_set_v3,r,r.active,"SetSkill");if(code)return code;
  if(r.source_error)return query(callback_release_v3,r);
  if(r.count&&(code=query(callback_erase_v3,r)))return code;
  if((code=query(callback_active_v3,r)))return code;
  if(!r.active)return -2;
  const char* name=op==skill_pre_v3?"OnPreSkill":op==skill_use_v3?"OnSkill":op==skill_post_v3?"OnPostSkill":"OnSkillCheck";
  if((code=query(callback_call_v3,r,r.active,name)))return code;
  if(op!=skill_post_v3&&!r.source_error){
   const auto selected=op==skill_check_active_v3?1u:0u;
   if(r.count>selected){if((code=query(callback_bool_v3,r,0,nullptr,selected)))return code;answer=r.boolean?1u:0u;}
   else if(op==skill_pre_v3||op==skill_use_v3)answer=1;
  }
  return query(callback_release_v3,r);
 }
};
}
extern "C" int dh2_character_skill_callback_v3(std::uint32_t* out,const dh2::character::skills::Instance32* instance,std::uint32_t op,const dh2::character::skills::SkillCallbackServices16V3* svc){
 if(!aligned(out,alignof(std::uint32_t))||!aligned(instance,alignof(dh2::character::skills::Instance32))||!aligned(svc,alignof(dh2::character::skills::SkillCallbackServices16V3))||!svc->invoke||!instance->owner||!instance->script||instance->reserved||op>dh2::character::skills::skill_check_active_v3)return -1;
 std::uint32_t result;auto code=Kernel{instance,svc}.run(op,result);if(!code)*out=result;return code;
}
