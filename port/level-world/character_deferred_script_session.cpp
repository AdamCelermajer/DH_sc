#include "character_deferred_script.hpp"
#include "character_script_session.hpp"
#include "character_script_virtual.hpp"
namespace dh2::character {
struct CharacterDeferredScript::Invocation {CharacterDeferredScript* adapter;const CharacterInitServices16* services;};
CharacterDeferredScript::CharacterDeferredScript(CharacterScriptSession& session) noexcept:session_(&session){}
CharacterScriptSession& CharacterDeferredScript::session() const noexcept {return *session_;}
int CharacterDeferredScript::service(void* context,ScriptLifecycleState64* state,
 const ScriptLifecycleRequest32* request,ScriptLifecycleResponse16*){
 auto& invocation=*static_cast<Invocation*>(context);auto& adapter=*invocation.adapter;
 if(state!=&adapter.session_->owner().lifecycle())return -1;
 if(request->service==script_refresh_vitals||request->service==script_configure_skills||request->service==script_update_skills)
  return invocation.services->invoke(invocation.services->context,*adapter.session_,*request);
 if(request->service!=script_ai_init_post&&request->service!=script_ai_init_final)return -1;
 // Execute the real CharAI wrapper projection, which reloads active separately
 // for Post and Final. No retained view/reference survives the Lua callback.
 if(!state->active)return 0;
 ScriptSessionView view{};
 if(!adapter.session_->owner().find(state->active,view))return -1;
 const ScriptTimerCall16 call{view.vm,view.aliases};
 const auto epoch=dh2_script_vm_required_failure_epoch(view.vm);
 const int result=dh2_character_script_initial_virtual(&call,view.kind,request->service==script_ai_init_post?12:16);
 if(dh2_script_vm_required_failure_epoch(view.vm)!=epoch){
  adapter.error_="Required native service failed during deferred initialization";
  adapter.last_virtual_status_=DH2_SCRIPT_REQUIRED_FAILURE_STATUS;return -2;
 }
 adapter.last_virtual_status_=result;
 if(result){adapter.error_=dh2_script_vm_error(view.vm);if(result!=-2)return result;}
 return 0; // Original LuaScript::Call diagnostics do not veto InitProcess.
}
int CharacterDeferredScript::load_and_init(std::uint32_t final,const CharacterInitServices16& services){
 if(final>1||!services.invoke)return -1;
 // Preserve source active guard even after an earlier failed native prefix:
 // active publication is not undone and InitProcess must not be retried.
 auto& state=session_->owner().lifecycle();
 if(state.active)return 0;
 if(failed_)return -2;
 int result=0;
 try{result=session_->advance();}catch(...){result=-2;}
 if(!session_->error().empty())error_=session_->error();
 if(result){failed_=true;error_=session_->error();return -2;}
 if(!state.active)return 0;
 Invocation invocation{this,&services};const DeferredScriptServices16 bridge{&invocation,&service};
 result=dh2_character_deferred_script(&state,script_init_process,final,&bridge);
 if(result<0){failed_=true;if(error_.empty())error_="Required Character InitScriptProcess service failed";return result;}
 return 1;
}
int CharacterDeferredScript::initialize_loaded_v70(std::uint32_t final,const CharacterInitServices16& services){
 if(final>1||!services.invoke)return -1;if(failed_)return -2;
 auto& state=session_->owner().lifecycle();if(!state.active){error_="Required actual loaded selected ScriptOwner before InitScriptProcess";failed_=true;return -2;}
 Invocation invocation{this,&services};const DeferredScriptServices16 bridge{&invocation,&service};
 const int result=dh2_character_deferred_script(&state,script_init_process,final,&bridge);
 if(result<0){failed_=true;if(error_.empty())error_="Required loaded NPC InitScriptProcess continuation";return result;}return 1;
}
}
