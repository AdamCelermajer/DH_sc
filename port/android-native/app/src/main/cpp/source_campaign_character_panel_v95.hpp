#pragma once
#include "character_panel_session_v1.hpp"
#include "character_panel_runtime_v3.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "character_menu_campaign_save_v50.hpp"
#include "character_menu_faery_connection_v8.hpp"
#include "character_menu_mutations_v4.hpp"
#include "application_player_manager_bootstrap_v59.hpp"

namespace dh2::android_ui {
// Bind after the actual selected Character's script, Save and Gear are ready.
// Root/combat supply the current gameplay projection; this adapter never
// constructs a player, inventory, profile, settings or temporary skill sheet.
struct SourceCampaignCharacterPanelServicesV95 {
 std::shared_ptr<void> owner;
 std::shared_ptr<player::ApplicationPlayerManagerBootstrapV59> players;
 std::function<bool(model_renderer::PlayerGameplayBinding&,std::string&)> player;
 // Existing registered writer for the SAME SaveLoad/profile. Do not register
 // another writer on every menu open or replace the source reader coordinator.
 std::shared_ptr<character::CharacterMenuCampaignSaveV50> writer;
 character::CharacterMenuMutationServicesV4 mutations;
 // Fresh actor-owned services, including delegates attached after composition.
 std::function<bool(const model_renderer::PlayerGameplayBinding&,
  character::CharacterMenuMutationServicesV4&,
  character::CharacterMenuFaeryServicesV8&,std::string&)> continuations;
 // Reached HUD DisplayRightHud/FillActionIcon, on the actual HUD AS receiver.
 std::function<bool(const char*,std::string&)> swap_hud;
 // Borrow actual CharAI+58/Character+420 and current Level each dispatch.
 std::function<bool(const model_renderer::PlayerGameplayBinding&,
  character::CharacterMenuFaeryServicesV8&,std::string&)> faery;
};

class SourceCampaignCharacterPanelV95 final:
 public std::enable_shared_from_this<SourceCampaignCharacterPanelV95> {
 model_renderer::SourceCampaignCharacterBorrowV62 selected_;
 SourceCampaignCharacterPanelServicesV95 services_;
 std::unique_ptr<CharacterPanelRuntimeV3> reload_;
 bool check(const model_renderer::PlayerGameplayBinding&,std::string&)const;
 bool action_graph(const model_renderer::PlayerGameplayBinding&,
  ui::CharacterMenuActionsGraphV1&,std::string&);
 bool query_graph(const model_renderer::PlayerGameplayBinding&,
  ui::CharacterMenuActionsOwnerV1&,ui::CharacterMenuQueriesGraphV1&,std::string&);
 SourceCampaignCharacterPanelV95(model_renderer::SourceCampaignCharacterBorrowV62,
  SourceCampaignCharacterPanelServicesV95);
public:
 // Startup-only composition if root has not yet registered its campaign
 // writer. Call once after profile publication, never from connect/open.
 // existing_readers is the source bootstrap's full retained coordinator;
 // only PROP is intercepted for menu ReloadSkills, all other masks delegate.
 // out retains even a failed registration prefix; do not retry that profile.
 static bool bind_campaign_save(model_renderer::SourceCampaignCharacterBorrowV62,
  std::function<bool(model_renderer::PlayerGameplayBinding&,std::string&)>,
  std::shared_ptr<level::CampaignSaveProfileV45>,
  character::CharacterMenuCampaignSaveServicesV50,
  data::PlayerSaveLoadServicesV1 existing_readers,
  std::shared_ptr<character::CharacterMenuCampaignSaveV50>& out,std::string&);
 static bool connect(model_renderer::SourceCampaignCharacterBorrowV62,
  SourceCampaignCharacterPanelServicesV95,CharacterPanelMovieRuntimeV3,
  std::shared_ptr<SourceCampaignCharacterPanelV95>&,std::string&);
 // Reject stale selection/world even if old owners remain pinned for teardown.
 bool player(model_renderer::PlayerGameplayBinding&,std::string&)const;
 CharacterPanelGameplayServicesV2 gameplay_services();
 bool bind_session(CharacterPanelSessionV1&,std::string&);
 bool dispatch(CharacterPanelSessionV1&,const char*,ui::CharacterMenuCallV1&,std::string&);
 bool initialize_authored(CharacterPanelSessionV1&,
  const ui::AuthoredCharacterPanelServicesV2&,std::string&);
 bool open(CharacterPanelSessionV1&,std::string&);
 bool back(CharacterPanelSessionV1&,std::string&);
 bool tab(CharacterPanelSessionV1&,unsigned,std::string&);
 bool pointer(CharacterPanelSessionV1&,int,int,float,float,std::string&);
 bool release(CharacterPanelSessionV1&,const char*,std::string&);
 // Root's renderer submits these SAME live Gear views. No preview inventory,
 // class/default armor, visual reconstruction or simulation update occurs.
 bool inventory_views(const std::vector<skinning::VisualDrawViewV32>*&,
  model_renderer::PlayerGameplayBinding& lease,std::string&);
 bool hud_view(const std::int32_t*&,std::size_t&,std::uintptr_t&,bool&,std::string&)const;
 // Existing native bridge operation numbers:0 auto,1 equip,2 unequip,3 swap.
 bool equipment_action(int operation,int index,int slot,std::string&);
 // CharacterPanelSession's development action bridge operation numbers:
 //0auto-equip,1unequip,2stat,3train,4skill-slot,5swap. Source AS dispatch is
 //preferred; this route avoids its legacy global equipment-action fallback.
 bool character_action(int operation,int index,int slot,std::string&);
 // Root attaches its retained original HUD AS transport; combat attaches the
 // genuine actor-owned positive trophy/drop/faery continuations. These do
 // not replace player/Save/Gear or the bootstrap's campaign writer.
 void bind_hud_swap(std::function<bool(const char*,std::string&)> callback){services_.swap_hud=std::move(callback);}
 void bind_actor_menu_continuations(character::CharacterMenuMutationServicesV4,
  std::function<bool(const model_renderer::PlayerGameplayBinding&,
   character::CharacterMenuFaeryServicesV8&,std::string&)>);
};
// Production composition: current campaign Application/PlayerManager, fresh
// combat V67 binding and the selected Character's existing profile bootstrap.
// Does not inspect/use the Crypt globals or start/reload any World.
bool connect_source_campaign_character_panel_v95(
 model_renderer::SourceCampaignCharacterBorrowV62,CharacterPanelMovieRuntimeV3,
 std::shared_ptr<SourceCampaignCharacterPanelV95>&,std::string&);
}
