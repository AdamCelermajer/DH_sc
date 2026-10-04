#include "character_skill_callback_session_v3.hpp"
#include "../script-runtime/script_return_observer_v3.h"
#include <cstring>
namespace dh2::character::skills {namespace {
struct Delivery {
 CharacterScriptSessionV3& session;std::uint32_t operation;
 std::uint32_t count=0,boolean=0;std::uintptr_t token=0;std::string& error;
 static int observe(void* p,const dh2_script_first_return_v1* v,char*,std::size_t){
  auto& d=*static_cast<Delivery*>(p);d.count=v->count;
  // Actual Value::getBool 31bc80: numeric !=0 (NaN included), pointer !=0;
  // strings are lua_pushstring followed by lua_toboolean in a fresh state.
  d.boolean=v->type==1||v->type==3?v->number!=0:
   v->type==2||v->type==7?v->identity!=0:v->type==4&&v->text;
  return 0;
 }
 static int invoke(void* p,const SkillCallbackRequest48V3* q,SkillCallbackResponse32V3* r){
  auto& d=*static_cast<Delivery*>(p);if(!q||!r||q->owner!=d.session.timers().owner)return -1;
  if(q->operation==callback_active_v3){ScriptSessionView v{};if(d.session.owner().active(v))r->active=v.identity;return 0;}
  // Complete ordered Value projections/destruction were performed inside the
  // actual protected Call. Tokens preserve caller choreography only; they do
  // not retain Lua Values or execute a second Call during erase/destruction.
  if(q->operation==callback_erase_v3){d.count=0;d.boolean=0;return 0;}
  if(q->operation==callback_release_v3){d.token=0;d.count=0;return 0;}
  if(q->operation==callback_bool_v3){if(q->results!=d.token)return -1;r->boolean=d.boolean;return 0;}
  ScriptSessionView v{};if(!q->active||!d.session.owner().find(q->active,v)||!v.vm||!v.aliases||!q->instance||!q->name)return -1;
  dh2_script_value a[2]{};unsigned n=0;
  if(q->operation==callback_set_v3){
   if(std::strcmp(q->name,"SetSkill")||!q->instance->script)return -1;
   a[0].type=DH2_SCRIPT_STRING;a[0].text=q->instance->script;a[0].text_bytes=std::strlen(a[0].text);
   a[1].type=DH2_SCRIPT_NUMBER;a[1].number=q->instance->argument_index;n=2;
  }else if(q->operation!=callback_call_v3)return -1;
  const auto* name=dh2_script_alias_resolve(v.aliases,q->name);d.count=d.boolean=0;
  const unsigned selected=q->operation==callback_call_v3&&d.operation==skill_check_active_v3?1u:0u;
  const int status=dh2_script_vm_call_indexed_source_v3(v.vm,name,a,n,selected,observe,&d);
  if(status<0){d.error=dh2_script_vm_error(v.vm);return -1;}
  r->source_error=status!=0;r->count=d.count;r->results=++d.token;
  if(status)d.error=dh2_script_vm_error(v.vm);return 0;
 }
};
}
int skill_callback_session_v3(CharacterScriptSessionV3& s,const Instance32* instance,
 std::uint32_t op,std::uint32_t* result,std::string& error){
 error.clear();Delivery d{s,op,0,0,0,error};const SkillCallbackServices16V3 services{&d,Delivery::invoke};
 return dh2_character_skill_callback_v3(result,instance,op,&services);
}
}
