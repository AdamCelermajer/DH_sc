#include "character_skill_info_session_v3.hpp"
#include "../script-runtime/script_return_observer_v1.h"
#include <cstring>
namespace dh2::character::skills {
namespace {
int first(void* p,const dh2_script_first_return_v1* value,char*,std::size_t){
 auto& r=*static_cast<SkillInfoResponseV1*>(p);r.return_count=value->count;
 r.first_type=value->type;r.first_number=value->number;return 0;
}
}
int skill_info_session_invoke_v3(void* opaque,const SkillInfoRequestV1* q,SkillInfoResponseV1* r){
 if(!opaque||!q||!r)return -1;
 auto& s=*static_cast<SkillInfoSessionsV3*>(opaque);s.error.clear();*r={};
 if(!s.resolve||!q->owner||q->reserved){s.error="SkillInfo Character provider missing";return -1;}
 auto* session=s.resolve(s.context,q->owner);if(!session){s.error="SkillInfo Character identity unavailable";return -1;}
 if(q->operation==skill_info_timer_v1){
  const auto status=dh2_character_timer_time_left(&r->elapsed,&r->duration,&session->timers(),static_cast<std::uint32_t>(q->timer));
  if(status<0){s.error="SkillInfo timer ownership malformed";return -1;}r->timer_found=status!=0;return 0;
 }
 ScriptSessionView view{};
 if(q->operation==skill_info_active_v1){if(session->owner().active(view))r->active=view.identity;return 0;}
 // Source captures active for each Call. It need not still be the published
 // active by the time callbacks finish; owner.find retains that exact identity.
 if(!q->active||!session->owner().find(q->active,view)||!view.vm||!view.aliases||!q->instance){s.error="SkillInfo captured script unavailable";return -1;}
 dh2_script_value arguments[2]{};std::uint32_t count=0;
 if(q->operation==skill_info_set_v1){
  if(!q->instance->script||!q->name||std::strcmp(q->name,"SetSkill")){s.error="SkillInfo SetSkill request malformed";return -1;}
  arguments[0].type=DH2_SCRIPT_STRING;arguments[0].text=q->instance->script;arguments[0].text_bytes=std::strlen(q->instance->script);
  arguments[1].type=DH2_SCRIPT_NUMBER;arguments[1].number=q->instance->argument_index;count=2;
 }else if(q->operation==skill_info_call_v1){
  if(!q->name||std::strcmp(q->name,"OnSkillInfo")){s.error="SkillInfo info request malformed";return -1;}
  std::int32_t signed_level;std::memcpy(&signed_level,&q->level,4);
  arguments[0].type=DH2_SCRIPT_NUMBER;arguments[0].number=static_cast<float>(signed_level);count=1;
 }else {s.error="SkillInfo unknown operation";return -1;}
 const auto* name=dh2_script_alias_resolve(view.aliases,q->name);
 const int status=dh2_script_vm_call_first_source_v1(view.vm,name,arguments,count,first,r);
 if(status<0){s.error=dh2_script_vm_error(view.vm);return -1;}
 r->source_error=status!=0;if(status)s.error=dh2_script_vm_error(view.vm);return 0;
}
}
