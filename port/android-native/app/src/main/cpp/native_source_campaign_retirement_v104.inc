#include "source_campaign_startup_abort_v114.hpp"
#include "source_campaign_release_v88.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "actual_device_android_v54.hpp"
#include <physical_world.hpp>
#include <application_services_owner_v5.hpp>
#include <native_gslevel_runtime_v27.hpp>
#include <gameobject_scene_root_registry_v1.hpp>
#include <event_manager_owner_v12.hpp>
#include "campaign_audio_retirement_v102.hpp"
namespace {
struct NativeMenuRetirementCaptureV104 {
 std::weak_ptr<NativeCharacterMenuV4> native;
 std::weak_ptr<dh2::ui::MenuStackOwnerV1> manager;
 bool detach(const std::shared_ptr<void>& world,std::string& e)const{
  auto expected=native.lock();auto stack=manager.lock();
  if(!expected||expected!=authored_character_menu||!stack||expected->shared_stack_v27!=stack){
   e="Native menu retirement lost the captured process manager";return false;
  }
  return model_renderer::retire_native_menu_campaign_v104(world,e);
 }
};
struct NativeCampaignRetirementV104 {
 std::weak_ptr<void> world;
 model_renderer::NativeCampaignRetirementLeavesV104 leaves;
 bool audio_retired{},menu_retired{},geometry_retired{};
 bool same(const std::shared_ptr<void>& actual,std::string& e)const{
  auto expected=world.lock();
  if(!expected||expected!=actual||expected.owner_before(actual)||actual.owner_before(expected)){
   e="Native retirement lost its captured actual World";return false;
  }
  return true;
 }
 model_renderer::SourceCampaignRetirementResultV88 quiesce(const std::shared_ptr<void>& actual,std::string& e){
  using R=model_renderer::SourceCampaignRetirementResultV88;bool pending{};
  if(!same(actual,e)||!model_renderer::close_source_campaign_admission_v104(actual,pending,e))return R::failed;
  e.clear();return pending?R::pending:R::complete;
 }
 model_renderer::SourceCampaignRetirementResultV88 retire(const std::shared_ptr<void>& actual,std::string& e){
  using R=model_renderer::SourceCampaignRetirementResultV88;
  if(!same(actual,e)||!model_renderer::require_source_campaign_quiescence_v104(actual,e))return R::failed;
  // Source retirement invokes this only after genuine GS Dtor completed its
  // Unload/D1 journals and cleared actual34/s_level. Never borrow currentLevel
  // here: it has intentionally been unpublished by that original body.
  if(!audio_retired){
   const auto result=leaves.audio(actual,e);
   if(result!=R::complete)return result;
   audio_retired=true;
  }
  if(!menu_retired){if(!leaves.menu(actual,e))return R::failed;menu_retired=true;}
  if(!geometry_retired){if(!model_renderer::retire_source_campaign_geometry_v104(actual,e))return R::failed;geometry_retired=true;}
  const auto scene_world=native_source_scene_v67.world.lock();
  if(scene_world&&scene_world!=actual){e="Final native scene unpublication addressed another World";return R::failed;}
  native_source_scene_v67.clear();e.clear();return R::complete;
 }
};
}
bool model_renderer::request_native_source_campaign_retirement_v104(
 const std::shared_ptr<void>& actual,NativeCampaignRetirementLeavesV104 leaves,std::string& e)try{
 std::shared_ptr<void> world;std::shared_ptr<SourceCampaignAdmissionV104> gate;
 if(!leaves.menu){
  if(!authored_character_menu||!authored_character_menu->process_directory_ready_v104||
     !authored_character_menu->shared_stack_v27){e="Required existing process menu owner for campaign retirement";return false;}
  auto captured=std::make_shared<NativeMenuRetirementCaptureV104>();
  captured->native=authored_character_menu;captured->manager=authored_character_menu->shared_stack_v27;
  leaves.owner=captured;
  leaves.menu=[captured](const auto& w,auto& error){return captured->detach(w,error);};
 }
 if(!actual||!leaves.owner||!leaves.menu||
    !borrow_source_campaign_world_owner_v104(world,e)||world!=actual||
    !borrow_source_campaign_admission_v104(actual,gate,e)){
  if(e.empty())e="Required actual retained campaign and complete native retirement providers";return false;
 }
 if(!leaves.owner.owner_before(world)&&!world.owner_before(leaves.owner)){
  e="Native retirement leaves cannot retain their enclosing World";return false;
 }
 if(!leaves.audio){
  std::shared_ptr<CampaignAudioRetirementV102> captured;
  if(!capture_campaign_audio_retirement_v102(actual,captured,e)||!captured)return false;
  leaves.audio=[captured](const auto&,std::string& error){
   bool done{};
   if(!captured->final_detach(done,error))return SourceCampaignRetirementResultV88::failed;
   return done?SourceCampaignRetirementResultV88::complete:SourceCampaignRetirementResultV88::pending;
  };
 }
 auto provider=std::make_shared<NativeCampaignRetirementV104>();provider->world=actual;provider->leaves=std::move(leaves);
 SourceCampaignRetirementServicesV88 services;services.owner=provider;
 services.quiesce=[provider](const auto& w,auto& error){return provider->quiesce(w,error);};
 services.retire_world=[provider](const auto& w,auto& error){return provider->retire(w,error);};
 return begin_source_campaign_retirement_v88(std::move(services),e);
}catch(const std::exception& failure){e=failure.what();return false;}
bool model_renderer::request_native_source_campaign_retirement_v104(const std::shared_ptr<void>& actual,std::string& e){
 return request_native_source_campaign_retirement_v104(actual,NativeCampaignRetirementLeavesV104{},e);
}

namespace {
struct NativeStartupRetirementV115 {
 std::weak_ptr<model_renderer::SourceWorldBorrowV61> world;
 std::weak_ptr<dh2::loader::NativeGSLevelRuntimeV27> gs;
 std::weak_ptr<dh2::loader::NativeGSLevelGlobalsV27> globals;
 bool had_world{},had_gs{},had_globals{},menu_attached{},audio_retired{},menu_retired{},geometry_retired{};
 std::weak_ptr<NativeCharacterMenuV4> menu;
 std::weak_ptr<dh2::ui::MenuStackOwnerV1> stack;
 std::shared_ptr<model_renderer::CampaignAudioRetirementV102> audio;
 bool same(const model_renderer::SourceCampaignStartupPrefixV114& p,std::string& e)const{
  const auto matches=[](const auto& weak,const auto& value,bool positive){
   auto captured=weak.lock();return positive?captured&&value&&captured==value&&!captured.owner_before(value)&&!value.owner_before(captured):!value;
  };
  if(!matches(world,p.world,had_world)||!matches(gs,p.gs,had_gs)||!matches(globals,p.globals,had_globals)){
   e="Native startup recovery addressed a replaced original prefix";return false;
  }
  return true;
 }
 model_renderer::SourceCampaignRetirementResultV88 quiesce(const model_renderer::SourceCampaignStartupPrefixV114& p,std::string& e){
  using R=model_renderer::SourceCampaignRetirementResultV88;if(!same(p,e))return R::failed;
  //Actual bound Device witnesses current GLES ownership even before World C1.
  std::int32_t driver{};if(!model_renderer::borrow_actual_device_driver_type_v55(driver,e))return R::failed;
  bool pending{};if(!dh2::ui::SwfMovie::source_dispatch_pending_v115(pending,e))return R::failed;
  if(p.world){
   if(!p.world->admission_v104){e="Positive startup World lost actual admission owner";return R::failed;}
   bool delivery{};if(!p.world->admission_v104->close(delivery,e))return R::failed;pending=pending||delivery;
   if(p.world->physical_world&&!p.world->physical_world->cleanup_delivery_idle_v106())pending=true;
   if(p.world->canonical_world&&p.world->canonical_world->scene_roots_v20&&
      p.world->canonical_world->scene_roots_v20->source_delivery_busy_v106())pending=true;
   if(p.world->application&&p.world->application->events14()&&p.world->application->events14()->source_delivery_busy_v115())pending=true;
  }
  e.clear();return pending?R::pending:R::complete;
 }
 model_renderer::SourceCampaignRetirementResultV88 release(const model_renderer::SourceCampaignStartupPrefixV114& p,std::string& e){
  using R=model_renderer::SourceCampaignRetirementResultV88;if(!same(p,e))return R::failed;
  if(p.published_level34&&p.world&&!p.world->release_v88){
   if(!model_renderer::prepare_source_campaign_startup_release_v115(p,e))return R::failed;
  }
  return model_renderer::release_source_campaign_startup_prefix_v114(p,e);
 }
 bool detach_menu(const std::shared_ptr<void>& actual,std::string& e){
  if(!menu_attached){e.clear();return true;} //Observed absence at actual request.
  auto native=menu.lock();auto manager=stack.lock();
  if(!native||native!=authored_character_menu||!manager||native->shared_stack_v27!=manager){
   e="Startup menu retirement lost its captured real process owner";return false;
  }
  const auto same=[](const auto& a,const auto& b){return a&&b&&a==b&&!a.owner_before(b)&&!b.owner_before(a);};
  if(!native->leases->world){if(same(native->retired_campaign_v104.lock(),actual)){e.clear();return true;}
   e="Startup menu alias disappeared without actual detach receipt";return false;}
  if(!same(native->leases->world,actual)){e="Startup menu is attached to another World";return false;}
  if(source_postmovie_v62.lock())return model_renderer::retire_native_menu_campaign_v104(actual,e);
  //C1 can attach the World before constructing postmovie/HUD providers. Read
  //their actual absence before removing only that native lease; do not invent
  //a source HUD/menu D0 or clear controller/resource state.
  if(!native->process_directory_ready_v104||!native->process_debug_v115){
   e="Partial process MenuManager C1 requires its qualified native teardown";return false;
  }
  if(!original_ui.require_unbound_campaign_aliases_v115(e)||
     (character_panel&&!character_panel->can_retire_campaign_v104(actual,e)))return false;
  if(character_panel&&!character_panel->retire_campaign_v104(actual,e))return false;
  native->retired_campaign_v104=actual;native->bootstrap_debug_v27=native->process_debug_v115;
  native->leases->world.reset();e.clear();return true;
 }
 model_renderer::SourceCampaignRetirementResultV88 retire(const model_renderer::SourceCampaignStartupPrefixV114& p,std::string& e){
  using R=model_renderer::SourceCampaignRetirementResultV88;if(!same(p,e))return R::failed;
  const auto idle=quiesce(p,e);if(idle!=R::complete)return idle;
  if(!p.world){
   //A World never published: none of its native GPU/menu/audio registries can
   //belong to it. Preserve unrelated process/front/demo owners and counters.
   e.clear();return R::complete;
  }
  auto actual=p.world->owner;
  if(!actual){e="Positive startup World has no actual renderer identity";return R::failed;}
  if(!model_renderer::require_source_campaign_quiescence_v104(actual,e))return R::failed;
  if(!audio_retired&&audio){bool done{};if(!audio->final_detach(done,e))return R::failed;
   if(!done){e.clear();return R::pending;}audio_retired=true;}
  if(!menu_retired){if(!detach_menu(actual,e))return R::failed;menu_retired=true;}
  if(!geometry_retired){if(!model_renderer::retire_source_campaign_geometry_v104(actual,e))return R::failed;geometry_retired=true;}
  auto scene=native_source_scene_v67.world.lock();
  if(scene){if(scene!=actual||scene.owner_before(actual)||actual.owner_before(scene)){e="Startup scene unpublication addresses another World";return R::failed;}
   native_source_scene_v67.clear();
  }else if(native_source_scene_v67.phase!=NativeSourceSceneV67::Phase::empty){e="Positive source scene prefix lost its actual World receipt";return R::failed;}
  e.clear();return R::complete;
 }
};
}
bool model_renderer::request_native_source_campaign_recovery_v115(std::string& e)try{
 SourceCampaignStartupPrefixV114 prefix;if(!borrow_source_campaign_startup_prefix_v114(prefix,e))return false;
 if(prefix.world){
  const bool gs=prefix.world->native_gslevel_v27&&*prefix.world->native_gslevel_v27;
  const bool loading=prefix.world->native_loading_v50&&*prefix.world->native_loading_v50;
  if(gs||loading){
   if(!gs||!loading||prefix.world->native_gslevel_v27->get()!=prefix.gs.get()){
    e="Failed-start recovery found mismatched actually adopted GS/loading slots";return false;
   }
   if(!prefix.world->release_v88&&!prepare_source_campaign_startup_release_v115(prefix,e))return false;
   return request_native_source_campaign_retirement_v104(prefix.world->owner,e);
  }
 }
 auto owner=std::make_shared<NativeStartupRetirementV115>();owner->world=prefix.world;owner->gs=prefix.gs;owner->globals=prefix.globals;
 owner->had_world=bool(prefix.world);owner->had_gs=bool(prefix.gs);owner->had_globals=bool(prefix.globals);
 if(prefix.world&&prefix.world->owner){
  if(!capture_campaign_audio_retirement_v102(prefix.world->owner,owner->audio,e))return false;
  auto native=authored_character_menu;
  if(native&&native->leases->world==prefix.world->owner&&
     !native->leases->world.owner_before(prefix.world->owner)&&!prefix.world->owner.owner_before(native->leases->world)){
   owner->menu_attached=true;owner->menu=native;owner->stack=native->shared_stack_v27;
  }
 }
 SourceCampaignStartupAbortServicesV114 services;services.owner=owner;
 services.quiesce=[owner](const auto& p,auto& e){return owner->quiesce(p,e);};
 services.release_prefix=[owner](const auto& p,auto& e){return owner->release(p,e);};
 services.retire_native=[owner](const auto& p,auto& e){return owner->retire(p,e);};
 return begin_source_campaign_startup_abort_v114(std::move(services),e);
}catch(const std::exception& failure){e=failure.what();return false;}
bool model_renderer::retry_native_source_campaign_recovery_v115(std::string& e){
 return retry_source_campaign_retirement_v114(e);
}
