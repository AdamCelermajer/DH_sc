#include "character_skill_reload_v6.hpp"
namespace dh2::character::skills {
extern "C" int dh2_character_skill_reload_v6(SkillReloadOutputV6* out,State40* state,const SkillReloadServicesV6* services){
 if(!out||!state||!state->owner||state->skills.reserved||state->skills.count>65536||(state->skills.count&&!state->skills.items)||!services||!services->invoke)return -1;
 SkillReloadOutputV6 result{};auto end=[&](int status){*out=result;return status;};
 auto call=[&](unsigned op,unsigned index,std::uintptr_t instance){result.phase=op;++result.calls;SkillReloadRequestV6 request{op,index,state->owner,instance};return services->invoke(services->context,state,&request)==0;};
 auto* captured=state->skills.items;const auto count=state->skills.count;
 for(unsigned i=0;i<count;++i){
  if(state->skills.items!=captured||state->skills.count!=count)return end(-2);
  const auto* instance=captured[i];if(!instance)continue;
  if(!call(skill_reload_delete_v6,i,reinterpret_cast<std::uintptr_t>(instance)))return end(-2);
  if(state->skills.items!=captured||state->skills.count!=count)return end(-2);
  const_cast<const Instance32**>(captured)[i]=nullptr;++result.deleted;
 }
 if(count&&!call(skill_reload_reset_end_v6,0,0))return end(-2);
 if(state->skills.count)return end(-2);
 if(!call(skill_reload_save_v6,0,0)||!call(skill_reload_configure_v6,0,0)||!call(skill_reload_update_v6,0,0))return end(-2);
 return end(1);
}
}
