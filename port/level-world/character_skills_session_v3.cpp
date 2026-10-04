#include "character_skills_session_v3.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::character::skills {
struct CharacterSkillSessionServicesV3::Impl {
 CharacterScriptSessionV3& session;const fx::PreloadServices16& debug;const NativeFsm24& fsm;std::string path,error;Row16 path_span{};
 Impl(CharacterScriptSessionV3& s,const fx::PreloadServices16& d,const NativeFsm24& f):session(s),debug(d),fsm(f){if(!debug.call||!fsm.state||fsm.reserved||fsm.character!=session.timers().owner)throw std::invalid_argument("Invalid skill Session services");}
 int call(const Request48& r,Response32& out){
  if(r.operation==debug_load||r.operation==debug_switch){unsigned value=0;return debug.call(debug.context,r.operation==debug_load?fx::debug_load:fx::debug_switch,r.name,&value);}
  if(r.operation==active_script){ScriptSessionView v{};if(r.payload!=session.timers().owner)return -1;out.object=session.owner().active(v)?v.identity:0;return 0;}
  if(r.operation==state_predicate){std::int32_t id;if(dh2_character_native_fsm_get_integer(&id,&fsm,0)!=1)return -1;out.count=id==static_cast<std::int32_t>(r.index);return 0;}
  if(r.operation==release_results)return 0; // root Call already projected/destroyed every Value
  if(r.operation==erase_results)return -1; // no retained return vector in discard provider
  ScriptSessionView v{};if(!r.script||!session.owner().find(r.script,v))return -1;
  switch(r.operation){
   case capture_path:{auto code=session.owner().copy_path(r.script,path);if(code)return code;if(path.size()>1048576)return -1;path_span={path.data(),static_cast<unsigned>(path.size()),0};out.row=&path_span;return 0;}
   case assign_path:return session.owner().assign_path(r.script,r.name,r.count);
   case load_file:{auto epoch=dh2_script_vm_required_failure_epoch(v.vm);bool loaded=false;auto code=session.load_file(r.script,r.name,loaded);if(code)return code;if(epoch!=dh2_script_vm_required_failure_epoch(v.vm))return -1;out.count=loaded;return 0;}
   case init_vcb:return session.init_vcb(r.script);
   case declare_skill:case reset_declaration:case set_skill:case call_update:case call_cleanup:{dh2_script_value args[2]{};unsigned count=0;if(r.operation==declare_skill||r.operation==set_skill){if(!r.instance||r.instance->owner!=session.timers().owner||!r.instance->script||r.instance->reserved)return -1;args[0].type=DH2_SCRIPT_STRING;args[0].text=r.instance->script;args[0].text_bytes=std::strlen(r.instance->script);args[1].type=DH2_SCRIPT_NUMBER;args[1].number=r.instance->argument_index;count=2;}return session.owner().call_discard(r.script,r.name,args,count,out.source_error);}
   default:return -1;
  }
 }
 static int invoke(void* p,State40* state,const Request48* r,Response32* out){auto& t=*static_cast<Impl*>(p);if(!state||state->owner!=t.session.timers().owner||!r||!out)return -1;try{auto result=t.call(*r,*out);if(result)t.error="Required skill Session delivery failed at operation "+std::to_string(r->operation)+": "+t.session.owner().error();return result;}catch(const std::exception& e){t.error=e.what();return -1;}}
};
CharacterSkillSessionServicesV3::CharacterSkillSessionServicesV3(CharacterScriptSessionV3& s,const fx::PreloadServices16& d,const NativeFsm24& f):impl_(std::make_unique<Impl>(s,d,f)){}
CharacterSkillSessionServicesV3::~CharacterSkillSessionServicesV3()=default;
Services16 CharacterSkillSessionServicesV3::services()noexcept{return {impl_.get(),Impl::invoke};}
const std::string& CharacterSkillSessionServicesV3::error()const noexcept{return impl_->error;}
}
