// Included after NativeMenuPostMovie/Load1. This lends existing owners only;
// the loader retains failure prefixes and owns camera/virtual10/PostLoad order.
#include "native_process_primary1_binding_v98.hpp"
#include "swf_text_font_platform_v1.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_language_refresh_v109.hpp"
#include "source_campaign_faery_v109.hpp"
#include "source_campaign_retirement_v88.hpp"
bool model_renderer::enroll_native_process_primary1_v98(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,std::string& e){
 if(surface_width<=1||surface_height<=1){e="Primary1 layout requires real NativeBridge.resize";return false;}
 // Native display-backend adaptation: the live resized Android cells.
 // This does not assert recovery of the original Width_Screen global writer.
 struct DisplayLoan {};auto loan=std::make_shared<DisplayLoan>();
 return enroll_native_process_primary1_v98(app,loan,&surface_width,&surface_height,e);
}
bool model_renderer::enroll_native_process_primary1_v98(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 std::shared_ptr<void> layout_owner,const std::int32_t* width_screen,
 const std::int32_t* height_screen,std::string& e){
 auto native=authored_character_menu;
 if(!app||!native||!native->process_directory_ready_v104||native->process_application_v104.lock()!=app||
    !layout_owner||!width_screen||!height_screen||!character_panel){
  e="Required SAME process menu/CharacterPanelSession and actual layout cells";return false;
 }
 std::shared_ptr<void> device_owner;const std::uint8_t* htc{};const std::uint8_t* no_igp{};
 if(!front_ui.menu_device_cells_v98(device_owner,htc,no_igp,e))return false;
 struct Inputs {std::shared_ptr<void> layout,device;};
 auto inputs=std::make_shared<Inputs>();inputs->layout=std::move(layout_owner);inputs->device=std::move(device_owner);
 std::weak_ptr<NativeCharacterMenuV4> weak=native;
 NativePrimary1LoadServicesV98 services;services.owner=inputs;
 services.facts=[weak,inputs,width_screen,height_screen,htc,no_igp](auto& out,auto& error){
  auto self=weak.lock();
  if(!self||authored_character_menu!=self||!self->process_directory_ready_v104){error="Expired actual process primary1 facts borrower";return false;}
  out={inputs,width_screen,height_screen,htc,no_igp};error.clear();return true;
 };
 services.load_primary_scoped=[weak](const char* uri,std::uint32_t slot,const NativePrimary1ContinueV98& continuation,std::string& error){
  auto self=weak.lock();
  if(!self||authored_character_menu!=self||slot!=1||!uri||!*uri||!character_panel||
     !self->shared_stack_v27||!self->platform||!self->leases->world){
   error="Required SAME process primary1 session/platform/current campaign";return false;
  }
  const std::weak_ptr<void> campaign=self->leases->world;
  if(!enroll_native_source_campaign_faery_v109(self->leases->world,error))return false;
  front_ui.bind_campaign_language_scene_v109(campaign,[campaign](auto& e){auto world=campaign.lock();if(!world){e="Retired source campaign language scene";return false;}return refresh_source_campaign_language_v109(world,e);});
  auto live_borrower=[weak,campaign](PlayerGameplayBinding& out,std::string& e){
   auto n=weak.lock();auto world=campaign.lock();
   if(!n||authored_character_menu!=n||!world||n->leases->world!=world){e="Primary1 selected borrower belongs to retired campaign";return false;}
   if(source_campaign_loading_complete_runtime_v64())return borrow_source_campaign_player_gameplay_v67(world,out,e);
   return borrow_source_campaign_player_pre_gameplay_v68(world,out,e);
  };
  PlayerGameplayBinding selected;if(!live_borrower(selected,error))return false;
  auto platform_services=self->bootstrap_services_v27;
  dh2::loader::CanonicalCurrentLevelBorrowV1 loading_level;
  if(!borrow_current_native_level_v27(loading_level,error)||!loading_level||loading_level.level()->constructor_fields_v3().field130!=12){
   if(error.empty())error="Primary1 Load requires current source Level12";return false;
  }
  const auto expected_level=std::weak_ptr<dh2::loader::CanonicalLevelContextV1>(loading_level.level());
  platform_services.source_continue_v98=[weak,campaign,expected_level](auto& e){
   auto native=weak.lock();auto world=campaign.lock();auto level=expected_level.lock();
   dh2::loader::CanonicalCurrentLevelBorrowV1 actual;
   if(!native||authored_character_menu!=native||!world||native->leases->world!=world||!level||
      source_campaign_retirement_requested_v88()||!borrow_current_native_level_v27(actual,e)||
      actual.level()!=level||actual.level().owner_before(level)||level.owner_before(actual.level())){
    if(e.empty())e="Primary1 callback belongs to retired or replaced source campaign/Level";return false;
   }
   return true;
  };
  if(!compose_native_primary1_scope_v98(platform_services,continuation,error))return false;
  dh2::ui::SwfServices application;application.context=self.get();application.native_owner=self->leases;
  application.native_action=NativeCharacterMenuV4::native;
  // These are actual function-call operands in dqcharmenu_droid, not every
  // Native* string in its constant pools. Query/action calls go through the
  // same CharacterPanelSession; navigation uses the process MenuManager.
  application.native_actions={
   "NativeReloadSkills","NativeChangeRolloverInputBehavior",
   "NativePushMenu","NativePopMenu","NativePopAllAbove","NativePopAllMenus",
   "NativeEquipSkill","NativeGetPlayerStats","NativeGetSkillDetails",
   "NativeHUDGetActiveFaery","NativeHUDGetIsFaeryUnlocked","NativeHUDSetActiveFaery",
   "NativeInvAutoEquipSlot","NativeInvDropItem","NativeInvEquipItem",
   "NativeInvGetEquipedItem","NativeInvGetHasOffHandWeapon","NativeInvGetHasTwoHandedWeapon",
   "NativeInvGetItemDetails","NativeInvGetItemsListForSlot","NativeInvGetPlayerGold",
   "NativeInvTransmuteItem","NativeInvUnequipItem","NativeSaveGame",
   "NativeSkillGetEquipedSkillsIDs","NativeSkillsGetSkillPointsLeft",
   "NativeSkillsTrainSkill","NativeStatsAssignPoint","NativeSwapEquipment",
   "NativeGetNumPotions","NativeGetStringNumPotions","NativeGetParsedString",
   "NativePlaySoundFX","NativeBackToHud"
  };
  // No HUD load: source primary3 can genuinely be NULL at Level stage12.
  if(!original_ui.character_panel_services(application,platform_services,error,false))return false;
  platform_services.shared_stack_v27=self->shared_stack_v27;platform_services.maximum_occurrences=256;
  auto* directory=self->shared_stack_v27->view();
  if(!directory||!directory->globals||!directory->base_render||!directory->base_render->root){error="Required actual process base projection before primary1";return false;}
  platform_services.globals=directory->globals;
  auto* base=directory->base_render;
  platform_services.base={base->identity,base->flags,base->root,base->controller_focus};
  platform_services.hud={};
  if(directory->hud_root){auto* hud=directory->hud_root;platform_services.hud={hud->identity,hud->flags,hud->root,hud->controller_focus};}
  platform_services.character=[](gameswf::as_object* object,auto& out,auto& e){return dh2::ui::authored_menu_stack_character_v4(object,0x84,{},out,e);};
  platform_services.publish_render=[weak](auto id,auto& e){
   auto n=weak.lock();auto* panel=character_panel?character_panel->authored_native_v4():nullptr;
   if(!n||authored_character_menu!=n||!panel||panel->source_stack_v4()!=n->shared_stack_v27.get()||
      !panel->source_movie_v91()||panel->source_render_identity_v91()!=id||!n->shared_stack_v27->render(id)||n->character_camera_v93){
    e="Required SAME pre-Load primary1 RenderFX prefix without paired camera";return false;
   }
   n->current_render=id;e.clear();return true;
  };
  platform_services.movie.source_text_buffering_v98=dh2::ui::SwfTextFontPlatformV1::set_source_text_buffering_v98;
  platform_services.localization.strings.player_character=[live_borrower](void*,auto& id,auto& e){PlayerGameplayBinding p;if(!live_borrower(p,e))return false;id=p.character;return true;};
  platform_services.localization.strings.player_name=[live_borrower](void*,auto id,auto& name,auto& e){PlayerGameplayBinding p;
   if(!live_borrower(p,e)||!p.save||p.character!=id||p.save->character()!=id){if(e.empty())e="Required SAME selected Save name";return false;}name=p.save->name();return true;
  };
  // Startup NativeReloadSkills borrows real initialized fields before gameplay
  // admission. It cannot use the global gameplay-only V95 connector.
  SourceCampaignCandidateBorrowV55 current;SourceCampaignCharacterBorrowV62 actor;
  if(!borrow_source_campaign_candidate_v55(current,error)||current.actual_world!=selected.world_owner||!current.application||
     !borrow_source_campaign_character_v62(selected.world_owner,selected.character,actor,error))return false;
  auto players=current.application->source_player_manager_v59();auto record=actor.character;
  if(!players||!players->manager()||!record||!record->services.design||!record->load||
     record->player_script_owner_v62.get()!=selected.skills||record->equipment.get()!=selected.gear||record->save.get()!=selected.save){
   error="Required SAME initialized source primary1 reload owners";return false;
  }
  dh2::android_ui::CharacterPanelMovieRuntimeV3 movie;movie.owner=self->leases;
  movie.player_index=[](double value,auto& out,auto& e){
   if(!std::isfinite(value)||value<double(INT32_MIN)||value>double(INT32_MAX)){e="Source primary1 player index outside native conversion";return false;}
   out=static_cast<std::int32_t>(value);return true;
  };
  movie.current_menu_fx=[weak](auto& id,auto& e){auto n=weak.lock();if(!n||authored_character_menu!=n||!n->current_render){e="Required actual published primary1 RenderFX";return false;}id=n->current_render;return true;};
  movie.invoke=[weak](auto id,const char* path,const char* method,bool optional,auto& e){
   auto n=weak.lock();if(!n||id!=n->current_render||!character_panel){e="Primary1 reload callback changed actual movie";return false;}
   struct Call {const char* path;const char* method;bool optional;} call{path,method,optional};
   return character_panel->authored_graph_scope_v4(&call,[](void* raw,auto& graph,auto& e){auto& c=*static_cast<Call*>(raw);dh2::ui::SwfAsValue root,receiver,result;bool callable{};
    if(!graph.root_value(root,e)||!graph.find_target(root,c.path,receiver,e)||!graph.invoke(receiver,receiver,c.method,{},result,callable,e))return false;
    if(!callable&&!c.optional){e="Required actual primary1 reload callback";return false;}return true;
   },e);
  };
  struct ReloadOwner {
   std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> record;
   std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59> players;
   std::unique_ptr<dh2::android_ui::CharacterPanelRuntimeV3> runtime;
  };
  auto reload_owner=std::make_shared<ReloadOwner>();reload_owner->record=record;reload_owner->players=players;
  auto design=record->services.design->borrow();
  reload_owner->runtime=std::make_unique<dh2::android_ui::CharacterPanelRuntimeV3>(selected.world_owner,*selected.skills,*selected.gear,*players->manager(),*record->load,std::move(design),movie);
  dh2::android_ui::CharacterPanelGameplayServicesV2 gameplay;gameplay.owner=reload_owner;
  gameplay.reload=[reload_owner,live_borrower,index=movie.player_index](const auto& requested,auto& graph,auto& e){
   PlayerGameplayBinding live;
   if(!live_borrower(live,e)||live.character!=requested.character||live.save!=requested.save||live.skills!=requested.skills||live.gear!=requested.gear){if(e.empty())e="Startup reload replaced selected source Character";return false;}
   struct Invocation {
    std::shared_ptr<ReloadOwner> owner;decltype(live_borrower) borrow;std::uintptr_t character;std::string error;
    static int invoke(void* raw,const dh2::ui::MenuReloadRequest32V1* q,dh2::ui::MenuReloadResponse16V1* out){
     auto& c=*static_cast<Invocation*>(raw);PlayerGameplayBinding p;
     if(!q||!out||!c.borrow(p,c.error)||p.character!=c.character||!p.save||!p.skills)return -1;
     *out={};
     if(q->service!=dh2::ui::reload_spec_prompt_v1&&q->subject!=p.character){c.error="Startup reload changed selected Character";return -1;}
     switch(q->service){
     case dh2::ui::reload_saved_level_v1:out->value=p.save->level();return 0;
     case dh2::ui::reload_saved_class_v1:out->value=p.save->class_id();return 0;
     case dh2::ui::reload_update_skills_v1:if(p.skills->update()<0){c.error=p.skills->error();return -1;}return 0;
     default:return c.owner->runtime->source(p,*q,*out,c.error)?0:-1;
     }
    }
   };
   auto invocation=std::make_shared<Invocation>(Invocation{reload_owner,live_borrower,live.character,{}});
   graph.owner=invocation;graph.player_index=index;
   graph.player=[invocation](auto index,bool remote,auto& character,auto& e){dh2::player::PlayerInfoFieldsV1* info{};
     auto* manager=invocation->owner->players->manager();
     if(!manager||!manager->get_player(index,remote,info,e))return false;
    character=info?info->character660:0;
    if(character&&character!=invocation->character){e="Startup reload addressed another actual Character";return false;}return true;
   };
   graph.reload={invocation.get(),Invocation::invoke};graph.failure_detail=[invocation]{return invocation->error;};e.clear();return true;
  };
  character_panel->bind_gameplay_services(std::move(gameplay),selected.world_owner);
  return character_panel->load_source_primary1_v98(selected,platform_services,uri,std::move(live_borrower),error);
 };
 return bind_native_primary1_loader_v98(app,std::move(services),e);
}
