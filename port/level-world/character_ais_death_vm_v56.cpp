#include "character_ais_death_vm_v56.hpp"
namespace dh2::character {
int character_ais_death_vm_v56(ScriptOwnerV2& owner,std::uintptr_t receiver,
 std::uintptr_t method,std::uintptr_t attacker,const dh2_script_callback_scope* scope,std::string& diagnostic){
 ScriptSessionView view{};
 if(!owner.find(receiver,view)||!view.vm||!view.aliases){diagnostic="Required retained selected V2 AIS death receiver";return -1;}
 if(method==0x3dbe90)return 0;
 if(method!=0x3dd440||view.kind!=script_external){diagnostic="Required original V2 selected AIS OnDied virtual+24";return DH2_SCRIPT_REQUIRED_FAILURE_STATUS;}
 dh2_script_value arg{};arg.type=DH2_SCRIPT_SOURCE_OBJECT;arg.identity=attacker;
 const auto* callback=dh2_script_alias_resolve(view.aliases,"OnDied");
 const auto epoch=dh2_script_vm_required_failure_epoch(view.vm);int status{};
 if(scope&&scope->vm==view.vm){
  if(!dh2_script_callback_scope_valid(scope)){diagnostic="Expired same-V2-AIS death callback scope";return -1;}
  status=dh2_script_callback_call_discard_source_objects(scope,callback,&arg,1);
  if(status){const auto* e=dh2_script_callback_error(scope);diagnostic=e?e:"V2 AIS OnDied scoped failure";}
 }else{
  status=dh2_script_vm_call_source_status_objects(view.vm,callback,&arg,1);
  if(status){const auto* e=dh2_script_vm_error(view.vm);diagnostic=e?e:"V2 AIS OnDied VM failure";}
 }
 if(dh2_script_vm_required_failure_epoch(view.vm)!=epoch)return DH2_SCRIPT_REQUIRED_FAILURE_STATUS;
 return status<0?status:0; // Original ordinary protected Lua errors are void returns.
}
}
