#include "character_skill_projectile_callbacks_v112.hpp"
#include "../script-runtime/script_return_observer_v3.h"
#include "../script-runtime/script_object_bridge.h"
#include <cstring>
namespace dh2::character::skills {namespace {
struct ReturnV112 {
 std::uint32_t count{},type{};float number{};
 static int observe(void* opaque,const dh2_script_first_return_v1* value,char*,std::size_t){
  if(!opaque||!value)return -1;
  auto& out=*static_cast<ReturnV112*>(opaque);
  out.count=value->count;out.type=value->type;out.number=value->number;return 0;
 }
};
bool valid(const ProjectileSkillScriptLoanV112& loan,std::string& error){
 if(!loan.owner||!loan.script.identity||!loan.script.vm||!loan.script.aliases||
    (loan.scope&&(loan.scope->vm!=loan.script.vm||!dh2_script_callback_scope_valid(loan.scope)))){
  error="Required SAME Skill projectile callback main Script receiver/capability";return false;
 }
 return true;
}
int call(const ProjectileSkillScriptLoanV112& loan,const char* name,
 const dh2_script_value* args,std::uint32_t count,ReturnV112& returns){
 // Alias/global resolution is repeated after SetSkill, which may synchronously
 // mutate the owner's selected Script and the alias recording dictionaries.
 const char* resolved=dh2_script_alias_resolve(loan.script.aliases,name);
 return loan.scope?dh2_script_callback_call_indexed_source_v112(loan.scope,resolved,
  args,count,0,ReturnV112::observe,&returns):dh2_script_vm_call_indexed_source_v3(
  loan.script.vm,resolved,args,count,0,ReturnV112::observe,&returns);
}
}
bool skill_projectile_callback_v112(ProjectileSkillCallbackV112 kind,const Instance32& instance,
 std::uintptr_t projectile,std::uintptr_t collision,const ProjectileSkillCallbackServicesV112& services,
 std::int32_t& result,std::string& error){
 if(!services.owner||!services.current_instance||!services.main_script||!instance.owner||
    !instance.script||!projectile){error="Required actual Skill instance/projectile callback owners";return false;}
 if(!services.current_instance(instance,error))return false;
 ProjectileSkillScriptLoanV112 first;
 if(!services.main_script(instance.owner,first,error))return false;
 if(!first.script.identity){result=1;error.clear();return true;}
 if(!valid(first,error))return false;
 dh2_script_value setup[2]{};
 setup[0].type=DH2_SCRIPT_STRING;setup[0].text=instance.script;
 setup[0].text_bytes=std::strlen(instance.script);
 setup[1].type=DH2_SCRIPT_NUMBER;setup[1].number=instance.argument_index;
 ReturnV112 ignored;const int setup_status=call(first,"SetSkill",setup,2,ignored);
 if(setup_status<0){error=dh2_script_vm_error(first.script.vm);return false;}
 if(setup_status){result=0;error.clear();return true;}
 // Full ReturnValues projection/destruction occurs inside the protected call.
 // Keep the captured first receiver pin alive through the reloaded call.
 ProjectileSkillScriptLoanV112 second;
 if(!services.main_script(instance.owner,second,error)||!valid(second,error))return false;
 dh2_script_value args[2]{};
 args[0].type=DH2_SCRIPT_SOURCE_OBJECT;args[0].identity=collision;
 args[1].type=DH2_SCRIPT_IDENTITY;args[1].identity=projectile;
 ReturnV112 returned;const int status=call(second,kind==ProjectileSkillCallbackV112::hit?
  "OnDelayedSkill":"OnDelayedSkillCheck",args,2,returned);
 if(status<0){error=dh2_script_vm_error(second.script.vm);return false;}
 result=!status&&returned.count&&returned.type==DH2_SCRIPT_BOOLEAN&&returned.number==0?1:0;
 error.clear();return true;
}
}
