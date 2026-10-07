#include "character_ais_kill_vm_v23.hpp"
namespace dh2::character {
bool character_ais_kill_method_v23(const ScriptSessionView& view,std::uintptr_t& out)noexcept{
 out=0;if(!view.identity||!view.constructor_fields)return false;
 switch(view.kind){case script_default:case script_monster:case script_faery:case script_external:out=0x3dbf00;return true;
 case script_player:case script_player_iphone:out=0x3ddb10;return true;default:return false;}
}
namespace {template<class Owner>int invoke(Owner& owner,std::uintptr_t receiver,std::uintptr_t method,std::uintptr_t killed,const dh2_script_callback_scope* scope,std::string& error){
 ScriptSessionView view{};std::uintptr_t actual{};
 if(!owner.find(receiver,view)||!character_ais_kill_method_v23(view,actual)||actual!=method){error="Required SAME selected AIS OnKill+b0 source receiver";return DH2_SCRIPT_REQUIRED_FAILURE_STATUS;}
 if(method==0x3dbf00)return 0; // whole inherited Default::OnKill bx lr.
 if(!(view.callback_flags&0x400u))return 0; // actual AISPlayer b8 producer gate.
 if(!view.vm||!view.aliases){error="Required actual player OnKill VM/alias owner";return DH2_SCRIPT_REQUIRED_FAILURE_STATUS;}
 dh2_script_value argument{};argument.type=DH2_SCRIPT_SOURCE_OBJECT;argument.identity=killed;
 const auto* name=dh2_script_alias_resolve(view.aliases,"OnKill");const auto epoch=dh2_script_vm_required_failure_epoch(view.vm);int status{};
 if(scope&&scope->vm==view.vm){if(!dh2_script_callback_scope_valid(scope)){error="Expired SAME skill VM OnKill callback capability";return -1;}status=dh2_script_callback_call_discard_source_objects(scope,name,&argument,1);if(status){const auto* e=dh2_script_callback_error(scope);error=e?e:"OnKill scoped Lua diagnostic";}}
 else{status=dh2_script_vm_call_source_status_objects(view.vm,name,&argument,1);if(status){const auto* e=dh2_script_vm_error(view.vm);error=e?e:"OnKill Lua diagnostic";}}
 if(dh2_script_vm_required_failure_epoch(view.vm)!=epoch)return DH2_SCRIPT_REQUIRED_FAILURE_STATUS;
 return status<0?status:0; // ordinary protected Lua errors are source void.
}
}
int character_ais_kill_vm_v23(ScriptOwner& owner,std::uintptr_t receiver,std::uintptr_t method,std::uintptr_t killed,const dh2_script_callback_scope* scope,std::string& error){return invoke(owner,receiver,method,killed,scope,error);}
int character_ais_kill_vm_v23(ScriptOwnerV2& owner,std::uintptr_t receiver,std::uintptr_t method,std::uintptr_t killed,const dh2_script_callback_scope* scope,std::string& error){return invoke(owner,receiver,method,killed,scope,error);}
}
