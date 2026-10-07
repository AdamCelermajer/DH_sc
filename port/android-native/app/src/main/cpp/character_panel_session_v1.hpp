#pragma once
#include "original_cache_assets_v1.hpp"
#include "hud_text_v1.hpp"
#include "player_gameplay_binding.hpp"
#include "character_panel_presentation_v1.hpp"
#include "character_menu_queries_owner_v1.hpp"
#include "authored_character_panel_v2.hpp"
#include "character_menu_reload_action_v1.hpp"
namespace dh2::android_ui {
bool initialize_player_skill_slots_v1(const model_renderer::PlayerGameplayBinding&,std::string&);
// Complete reached source item/Save/faery/HUD endpoints using existing live
// owners. Hooks run for each synchronous dispatch; no profile copy is retained.
struct CharacterPanelGameplayServicesV2 {
 std::shared_ptr<void> owner;
 std::function<bool(const model_renderer::PlayerGameplayBinding&,
                    ui::CharacterMenuActionsGraphV1&,std::string&)> actions;
 std::function<bool(const model_renderer::PlayerGameplayBinding&,
                    ui::CharacterMenuActionsOwnerV1&,
                    ui::CharacterMenuQueriesGraphV1&,std::string&)> queries;
 // Whole Character::ReloadSkills, with fresh SAME Character/Save/file/VM
 // services. Startup AS reaches this before the original first screen opens.
 std::function<bool(const model_renderer::PlayerGameplayBinding&,
                    ui::CharacterMenuReloadActionGraphV1&,std::string&)> reload;
};
// Android view transport only. Every query/action borrows the sole live World
// authority on the GL thread; this owner never stores player/gear/Save copies.
class CharacterPanelSessionV1 {
  AAssetManager* manager_{};
 OriginalCacheAssetsV1 assets_;
 CharacterPanelPresentationV1 presentation_;
  ui::HudTextV1 text_;
  ui::CharacterMenuFontPaletteV1 palette_;
  bool palette_ready_=false;
 bool ready_=false;
  std::unique_ptr<ui::AuthoredCharacterPanelV2> authored_;
  std::unique_ptr<ui::AuthoredCharacterPanelV2> failed_authored_prefix_v98_;
  std::uintptr_t failed_authored_identity_v98_{};
  std::string failed_authored_error_v98_;
  ui::AuthoredCharacterPanelV2* initializing_authored_{};
 const model_renderer::PlayerGameplayBinding* authored_player_{};
 CharacterPanelGameplayServicesV2 gameplay_services_;
 std::weak_ptr<void> gameplay_world_v104_;
 bool campaign_detached_v104_{};
 bool source_loading_profile_scope_v98_{};
 bool prepare(std::string&);
 bool initialize_authored_impl_v98(const model_renderer::PlayerGameplayBinding&,const ui::AuthoredCharacterPanelServicesV2&,
   const char* source_uri,std::function<bool(model_renderer::PlayerGameplayBinding&,std::string&)> live_binding,std::string&);
public:
  explicit CharacterPanelSessionV1(AAssetManager* assets):manager_(assets),assets_(assets),presentation_(assets_){}
 bool font_palette_borrow_v4(const ui::CharacterMenuFontPaletteV1*& out,std::string& error){
  out=nullptr;if(!prepare(error))return false;
  if(!palette_ready_){error="Required actual loaded CharacterMenu font palette";return false;}
  out=&palette_;return true;
 }
 std::string snapshot(const model_renderer::PlayerGameplayBinding&);
  std::string action(const model_renderer::PlayerGameplayBinding&,int operation,int index,int slot);
  // The authored movies use the real protected AS transport and mutate their
  // own receivers directly. JSON is only the development widget fallback.
  bool dispatch(const model_renderer::PlayerGameplayBinding&,const char*,ui::CharacterMenuCallV1&,std::string&);
 bool dispatch_source_loading_v98(const model_renderer::PlayerGameplayBinding&,const char*,ui::CharacterMenuCallV1&,std::string&);
 bool adopt_source_gameplay_v109(const model_renderer::PlayerGameplayBinding&,const ui::AuthoredCharacterPanelServicesV2&,std::string&);
 void bind_gameplay_services(CharacterPanelGameplayServicesV2 services,std::shared_ptr<void> actual_world={}){
  gameplay_services_=std::move(services);gameplay_world_v104_=std::move(actual_world);
 }
 bool can_retire_campaign_v104(const std::shared_ptr<void>&,std::string&)const;
 bool retire_campaign_v104(const std::shared_ptr<void>&,std::string&);
 bool campaign_rebind_required_v104()const noexcept{return campaign_detached_v104_;}
 // ONE authored owner retained by this same panel session. Each entry borrows
 // the current live profile only for the complete synchronous movie operation.
 bool initialize_authored(const model_renderer::PlayerGameplayBinding&,
                         const ui::AuthoredCharacterPanelServicesV2&,std::string&);
 bool retire_failed_authored_prefix_v98(std::string&);
 bool load_source_primary1_v98(const model_renderer::PlayerGameplayBinding&,const ui::AuthoredCharacterPanelServicesV2&,
   const char* actual_uri,std::function<bool(model_renderer::PlayerGameplayBinding&,std::string&)> live_binding,std::string&);
 bool authored_open(const model_renderer::PlayerGameplayBinding&,std::string&);
 bool authored_tab(const model_renderer::PlayerGameplayBinding&,unsigned,std::string&);
 bool authored_release(const model_renderer::PlayerGameplayBinding&,const char*,std::string&);
 bool authored_geometry(const model_renderer::PlayerGameplayBinding&,const char*,float,float,ui::AuthoredHudGeometryV1&,std::string&);
 bool authored_pointer(const model_renderer::PlayerGameplayBinding&,int,int,float,float,std::string&);
 bool authored_back(const model_renderer::PlayerGameplayBinding&,std::string&);
 bool authored_frame(const model_renderer::PlayerGameplayBinding&,float,std::string&);
  bool authored_display(const model_renderer::PlayerGameplayBinding&,int,int,int,int,std::string&);
  bool authored_source_display_v4(const model_renderer::PlayerGameplayBinding&,std::string&);
 bool authored_viewport(const model_renderer::PlayerGameplayBinding&,const ui::ViewportState64&,const ui::SwfViewportDriver&,std::string&);
  bool authored_bound()const noexcept{return !initializing_authored_&&!failed_authored_prefix_v98_&&authored_&&authored_->bound();}
  void reset_authored_v4(){if(authored_player_||initializing_authored_||failed_authored_prefix_v98_)return;authored_.reset();initializing_authored_=nullptr;gameplay_services_={};gameplay_world_v104_.reset();campaign_detached_v104_=false;}
  ui::AuthoredCharacterPanelV2* authored_native_v4()noexcept{return initializing_authored_?initializing_authored_:(failed_authored_prefix_v98_?failed_authored_prefix_v98_.get():authored_.get());}
  bool authored_graph_scope_v4(void*,bool(*)(void*,ui::SwfAsGraph&,std::string&),std::string&);
};
}
