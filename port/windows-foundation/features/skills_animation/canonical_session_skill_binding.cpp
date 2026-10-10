#include "canonical_session_skill_binding.hpp"

#include "../enemy_ai/scoped_skill_callback.hpp"
#include "../../retained_pose_playback.hpp"
#include "../../../script-runtime/script_return_observer_v3.h"

#include <cstring>

namespace dh::foundation::skills_animation {
namespace {
using namespace dh2::character;
using namespace dh2::character::skills;

struct V3Delivery {
 CharacterScriptSessionV3& session;
 std::uint32_t operation;
 const dh2_script_callback_scope* scope;
 std::uint32_t count{},boolean{};
 std::uintptr_t token{};
 std::string& error;

 static int observe(void* raw,const dh2_script_first_return_v1* value,char*,std::size_t){
  auto& self=*static_cast<V3Delivery*>(raw);
  self.count=value->count;
  self.boolean=value->type==1||value->type==3?value->number!=0:
   value->type==2||value->type==7?value->identity!=0:
   value->type==4&&value->text;
  return 0;
 }
 static int invoke(void* raw,const SkillCallbackRequest48V3* request,
  SkillCallbackResponse32V3* response){
  auto& self=*static_cast<V3Delivery*>(raw);
  if(!request||!response||request->owner!=self.session.timers().owner)return -1;
  if(request->operation==callback_active_v3){
   ScriptSessionView active{};
   if(self.session.owner().active(active))response->active=active.identity;
   return 0;
  }
  if(request->operation==callback_erase_v3){self.count=self.boolean=0;return 0;}
  if(request->operation==callback_release_v3){self.token=0;self.count=0;return 0;}
  if(request->operation==callback_bool_v3){
   if(request->results!=self.token)return -1;
   response->boolean=self.boolean;return 0;
  }
  ScriptSessionView active{};
  if(!request->active||!self.session.owner().find(request->active,active)||
   !active.vm||!active.aliases||!request->instance||!request->name)return -1;
  dh2_script_value arguments[2]{};
  std::uint32_t argument_count=0;
  if(request->operation==callback_set_v3){
   if(std::strcmp(request->name,"SetSkill")||!request->instance->script)return -1;
   arguments[0].type=DH2_SCRIPT_STRING;
   arguments[0].text=request->instance->script;
   arguments[0].text_bytes=std::strlen(arguments[0].text);
   arguments[1].type=DH2_SCRIPT_NUMBER;
   arguments[1].number=request->instance->argument_index;
   argument_count=2;
  }else if(request->operation!=callback_call_v3)return -1;
  const auto* name=dh2_script_alias_resolve(active.aliases,request->name);
  self.count=self.boolean=0;
  const std::uint32_t selected=request->operation==callback_call_v3&&
   self.operation==skill_check_active_v3?1u:0u;
  int status;
  if(self.scope){
   if(self.scope->vm!=active.vm||!dh2_script_callback_scope_valid(self.scope)){
    self.error="Required actual same-VM player DoSkill scope";return -1;
   }
   status=dh2_script_callback_call_indexed_source_v112(self.scope,name,arguments,
    argument_count,selected,observe,&self);
  }else{
   status=dh2_script_vm_call_indexed_source_v3(active.vm,name,arguments,
    argument_count,selected,observe,&self);
  }
  if(status<0){self.error=dh2_script_vm_error(active.vm);return -1;}
  response->source_error=status!=0;
  response->count=self.count;
  response->results=++self.token;
  if(status)self.error=dh2_script_vm_error(active.vm);
  return 0;
 }
};

}

int scoped_canonical_skill_callback(CharacterScriptSessionV3& session,
 const Instance32* instance,std::uint32_t operation,std::uint32_t* result,
 const dh2_script_callback_scope* scope,std::string& error){
 error.clear();
 V3Delivery delivery{session,operation,scope,0,0,0,error};
 const SkillCallbackServices16V3 services{&delivery,V3Delivery::invoke};
 return dh2_character_skill_callback_v3(result,instance,operation,&services);
}

void bind_canonical_skill_callbacks(NativeSkillLuaServicesV1& native,
 CharacterScriptSession& session){
 native.set_callback_transport([&session](const Instance32* instance,
  std::uint32_t operation,std::uint32_t& result,std::string& error){
   return enemy_ai::scoped_skill_callback(session,instance,operation,&result,
    session.current_skill_callback_scope(),error);
  });
}

void bind_canonical_skill_callbacks(NativeSkillLuaServices& native,
 CharacterScriptSessionV3& session){
 native.set_callback_transport([&session](const Instance32* instance,
  std::uint32_t operation,std::uint32_t& result,std::string& error){
   return scoped_canonical_skill_callback(session,instance,operation,&result,
    session.current_skill_callback_scope(),error);
  });
}

void bind_canonical_session_skill_services(SessionSkillServices& services,
 NativeSkillLuaServicesV1& native,CharacterScriptSession& session){
 bind_canonical_skill_callbacks(native,session);
 services.ai=native.ai_services();
 services.state=native.state_services();
}

void bind_canonical_session_skill_services(SessionSkillServices& services,
 NativeSkillLuaServices& native,CharacterScriptSessionV3& session){
 bind_canonical_skill_callbacks(native,session);
 services.ai=native.ai_services();
 services.state=native.state_services();
}

void bind_session_skill_pose_services(SessionSkillServices& services){
 services.play=[](CombatSession& session,ActorId actor,
  const OriginalCombatVisualPlan& plan,const OriginalSequencePolicies& policies,
  const OriginalAttackSelection& selection,
  CombatSessionStateAnimationServices events,std::string& error){
   return session.play_actor_source_sequence(actor,plan,policies,selection,
    std::move(events),error);
  };
 services.finished=[](CombatSession& session,ActorId actor,std::string& error){
  const auto* retained=session.retained_actor_pose(actor);
  if(!retained){error="Source skill completion lost the Session retained actor owner";return false;}
  if(!retained->current_ended()){
   error="Source skill completion arrived before the retained timeline ended";
   return false;
  }
  error.clear();return true;
 };
}

}
