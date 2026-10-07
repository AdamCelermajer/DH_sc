#include "character_combat_results_ai_v1.hpp"
namespace dh2::character {
int character_combat_results_ai_v1(const CombatResultsAiBorrowV1& borrow,
 std::uintptr_t attacker,std::uintptr_t target,std::uint8_t outcomes,std::string& diagnostic){
 if(!borrow.owner&&!borrow.owner_v2)return 0;
 if(borrow.owner&&borrow.owner_v2){diagnostic="Ambiguous AIS owner authority";return -1;}
 ScriptSessionView view{};if(!(borrow.owner_v2?borrow.owner_v2->active(view):borrow.owner->active(view)))return 0;
 if(borrow.source_character_callback!=0x3dc9a8){diagnostic="Required actual selected AIS Character OnCombatResults virtual+b4";return DH2_SCRIPT_REQUIRED_FAILURE_STATUS;}
 const char* callback=nullptr;
 if(outcomes&3){if(view.callback_flags&0x1000)callback="OnTargetMissed";}
 else if(view.callback_flags&0x800)callback="OnTargetHit";
 if(!callback)return 0;
 if(!view.vm||!view.aliases){diagnostic="Required selected AIS combat-result VM/aliases";return -1;}
 dh2_script_value args[2]{};args[0].type=args[1].type=DH2_SCRIPT_SOURCE_OBJECT;
 args[0].identity=attacker;args[1].identity=target;
 const char* resolved=dh2_script_alias_resolve(view.aliases,callback);
 int status;
 if(borrow.scope&&borrow.scope->vm==view.vm){
  if(!dh2_script_callback_scope_valid(borrow.scope)){diagnostic="Expired/shadowed SAME AIS combat-result callback scope";return -1;}
  const auto epoch=dh2_script_vm_required_failure_epoch(view.vm);
  status=dh2_script_callback_call_discard_source_objects(borrow.scope,resolved,args,2);
  if(dh2_script_vm_required_failure_epoch(view.vm)!=epoch)status=DH2_SCRIPT_REQUIRED_FAILURE_STATUS;
  if(status){const char* e=dh2_script_callback_error(borrow.scope);diagnostic=e?e:"Combat-result scoped VM failure";}
 }else{
  status=dh2_script_vm_call_source_status_objects(view.vm,resolved,args,2);
  if(status){const char* e=dh2_script_vm_error(view.vm);diagnostic=e?e:"Combat-result VM failure";}
 }
 return status;
}
int character_combat_results_pair_v1(const CombatResultsAiBorrowV1& a,const CombatResultsAiBorrowV1& b,
 std::uintptr_t attacker,std::uintptr_t target,std::uint8_t outcomes,std::string& diagnostic){
 int status=character_combat_results_ai_v1(a,attacker,target,outcomes,diagnostic);
 if(status<0)return status;
 // Original void callback dispatch reaches target even after ordinary Lua
 // diagnostics. Preserve the first diagnostic rather than erase it on success.
 std::string next;int second=character_combat_results_ai_v1(b,attacker,target,outcomes,next);
 if(second){if(diagnostic.empty())diagnostic=next;return second;}return status;
}
}
