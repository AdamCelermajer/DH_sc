#include "character_ai_state_changed_vm.hpp"
#include "character_ai_state_changed.hpp"
namespace dh2::character {
namespace {
struct Delivery {ScriptSessionView view;const dh2_script_callback_scope* scope;int status=0;};
int call(void* context,const dh2::object_identity::TargetCall32* request){
 auto& d=*static_cast<Delivery*>(context);
 const char* resolved=dh2_script_alias_resolve(d.view.aliases,request->callback);
 d.status=d.scope?dh2_script_callback_call_discard_source(d.scope,resolved,nullptr,0):
  dh2_script_vm_call_discard_source(d.view.vm,resolved,nullptr,0);
 return d.status;
}
}
int character_ais_external_end_anim(ScriptOwner& owner,std::uintptr_t identity,const dh2_script_callback_scope* scope){
 Delivery delivery{};if(!owner.find(identity,delivery.view)||delivery.view.kind!=script_external||
  !delivery.view.vm||!delivery.view.aliases||
  (scope&&(scope->vm!=delivery.view.vm||!dh2_script_callback_scope_valid(scope))))return -1;
 delivery.scope=scope;
 const dh2::object_identity::TargetScript16 script{identity,delivery.view.callback_flags,0};
 const dh2::object_identity::TargetServices16 services{&delivery,&call};
 const int status=dh2_character_ais_external_end_anim(&script,&services);
 return status==1?-1:delivery.status;
}
}
