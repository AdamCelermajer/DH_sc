#include "scoped_skill_callback.hpp"
#include "../../../script-runtime/script_return_observer_v3.h"
#include <cstring>
namespace dh::foundation::enemy_ai {namespace {
using namespace dh2::character;using namespace dh2::character::skills;
struct Delivery {
 CharacterScriptSession& session;std::uint32_t operation;const dh2_script_callback_scope* scope;
 std::uint32_t count{},boolean{};std::uintptr_t token{};std::string& error;
 static int observe(void* p,const dh2_script_first_return_v1* v,char*,std::size_t){
  auto& d=*static_cast<Delivery*>(p);d.count=v->count;
  d.boolean=v->type==1||v->type==3?v->number!=0:v->type==2||v->type==7?v->identity!=0:v->type==4&&v->text;return 0;
 }
 static int invoke(void* p,const SkillCallbackRequest48V3* q,SkillCallbackResponse32V3* r){
  auto& d=*static_cast<Delivery*>(p);if(!q||!r||q->owner!=d.session.timers().owner)return -1;
  if(q->operation==callback_active_v3){ScriptSessionView view{};if(d.session.owner().active(view))r->active=view.identity;return 0;}
  if(q->operation==callback_erase_v3){d.count=d.boolean=0;return 0;}
  if(q->operation==callback_release_v3){d.token=0;d.count=0;return 0;}
  if(q->operation==callback_bool_v3){if(q->results!=d.token)return -1;r->boolean=d.boolean;return 0;}
  ScriptSessionView view{};
  if(!q->active||!d.session.owner().find(q->active,view)||!view.vm||!view.aliases||!q->instance||!q->name)return -1;
  dh2_script_value arguments[2]{};unsigned n=0;
  if(q->operation==callback_set_v3){
   if(std::strcmp(q->name,"SetSkill")||!q->instance->script)return -1;
   arguments[0].type=DH2_SCRIPT_STRING;arguments[0].text=q->instance->script;arguments[0].text_bytes=std::strlen(arguments[0].text);
   arguments[1].type=DH2_SCRIPT_NUMBER;arguments[1].number=q->instance->argument_index;n=2;
  }else if(q->operation!=callback_call_v3)return -1;
  const auto* name=dh2_script_alias_resolve(view.aliases,q->name);d.count=d.boolean=0;
  const unsigned selected=q->operation==callback_call_v3&&d.operation==skill_check_active_v3?1u:0u;
  int status;
  if(d.scope){
   if(d.scope->vm!=view.vm||!dh2_script_callback_scope_valid(d.scope)){d.error="Required actual same-VM native skill scope";return -1;}
   status=dh2_script_callback_call_indexed_source_v112(d.scope,name,arguments,n,selected,observe,&d);
  }else status=dh2_script_vm_call_indexed_source_v3(view.vm,name,arguments,n,selected,observe,&d);
  if(status<0){d.error=dh2_script_vm_error(view.vm);return -1;}
  r->source_error=status!=0;r->count=d.count;r->results=++d.token;
  if(status)d.error=dh2_script_vm_error(view.vm);return 0;
 }
};
}
int scoped_skill_callback(dh2::character::CharacterScriptSession& session,
 const dh2::character::skills::Instance32* instance,std::uint32_t op,std::uint32_t* result,
 const dh2_script_callback_scope* scope,std::string& error){
 error.clear();Delivery d{session,op,scope,0,0,0,error};
 const dh2::character::skills::SkillCallbackServices16V3 services{&d,Delivery::invoke};
 return dh2_character_skill_callback_v3(result,instance,op,&services);
}
}
