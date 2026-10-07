#include "character_panel_reload_v3.hpp"
namespace dh2::android_ui {
namespace {
struct Invocation {
 const model_renderer::PlayerGameplayBinding& player;
 CharacterPanelReloadServicesV3 services;
 std::string error;
 Invocation(const model_renderer::PlayerGameplayBinding& p,const CharacterPanelReloadServicesV3& s):player(p),services(s){}
 static int invoke(void* raw,const ui::MenuReloadRequest32V1* request,ui::MenuReloadResponse16V1* response){
  auto& c=*static_cast<Invocation*>(raw);if(!request||!response)return -1;
  const auto& p=c.player;*response={};
  if(!p.world_owner||!p.skills||!p.save||!p.gear||p.save!=p.skills->native_savegame()||p.save->character()!=p.character||p.gear->properties()!=p.skills->session().properties()){
   c.error="Reload callback lost same retained Character/Save/skill/property graph";return -1;
  }
  if(request->service!=ui::reload_spec_prompt_v1&&request->subject!=p.character){c.error="Reload callback changed selected Character";return -1;}
  switch(request->service){
   case ui::reload_saved_level_v1:response->value=p.save->level();return 0;
   case ui::reload_saved_class_v1:response->value=p.save->class_id();return 0;
   case ui::reload_update_skills_v1:
    if(p.skills->update()<0){c.error=p.skills->error();return -1;}return 0;
   default:break;
  }
  if(!c.services.invoke||!c.services.invoke(p,*request,*response,c.error)){if(c.error.empty())c.error="Required actual Character reload source service "+std::to_string(request->service);return -1;}
  return response->reserved?-1:0;
 }
};
}
bool character_panel_reload_binding_v3(const model_renderer::PlayerGameplayBinding& p,const CharacterPanelReloadServicesV3& services,ui::CharacterMenuReloadActionGraphV1& out,std::string& error){
 if(!p.active||!p.world_owner||!p.skills||!p.save||!p.gear||!services.owner||!services.player||!services.player_index||!services.invoke){error="Required source Character reload/runtime/menu binding";return false;}
 auto call=std::make_shared<Invocation>(p,services);
 out.owner=call;out.player=[call](auto index,bool remote,auto& character,auto& e){
  if(!call->services.player(index,remote,character,e))return false;
  if(character&&character!=call->player.character){e="Reload must select this panel's same current Character";return false;}return true;
 };
 out.player_index=services.player_index;out.reload={call.get(),Invocation::invoke};
 out.failure_detail=[call]{return call->error;};error.clear();return true;
}
}
