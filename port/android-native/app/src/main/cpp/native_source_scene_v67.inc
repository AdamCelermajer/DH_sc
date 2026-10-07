// Native presentation handoff after real loading completion. This state is a
// render transport receipt, never a replacement Level/player readiness field.
namespace {
struct NativeSourceSceneV67 {
 enum class Phase {empty,preparing,active,failed};Phase phase{Phase::empty};
 std::weak_ptr<void> world;
 std::weak_ptr<dh2::loader::CanonicalLevelContextV1> level;
 std::string failure;
 bool gpu_restore_pending{},gpu_restore_attempted{};
 void clear(){phase=Phase::empty;world.reset();level.reset();failure.clear();gpu_restore_pending=gpu_restore_attempted=false;}
 void context_lost(){
  if(phase==Phase::active){gpu_restore_pending=true;gpu_restore_attempted=false;failure.clear();}
 }
};
NativeSourceSceneV67 native_source_scene_v67;
bool restore_native_source_gpu_v68(AAssetManager* assets,std::string& error)try{
 auto& scene=native_source_scene_v67;
 if(!scene.gpu_restore_pending){error.clear();return true;}
 if(scene.gpu_restore_attempted){error=scene.failure;return false;}
 scene.gpu_restore_attempted=true;
 model_renderer::SourceCampaignCandidateBorrowV55 actual;
 if(scene.phase!=NativeSourceSceneV67::Phase::active||!assets||
    !model_renderer::borrow_source_campaign_candidate_v55(actual,error)||
    scene.world.lock()!=actual.actual_world||scene.level.lock()!=actual.level||
    !model_renderer::prepare_source_campaign_geometry_v64(assets,error)){
  if(error.empty())error="Context restoration lost same live native campaign";
  scene.failure=error;return false;
 }
 // Gameplay callback and CPU menu/actor owners are already bound. Rebuilding
 // admitted GPU storage does not repeat C1, InitPost, HUD reload or save load.
 scene.gpu_restore_pending=false;scene.failure.clear();error.clear();return true;
}catch(const std::exception& failure){native_source_scene_v67.failure=error=failure.what();return false;}
}
bool model_renderer::source_campaign_scene_active_v67(){
 return native_source_scene_v67.phase==NativeSourceSceneV67::Phase::active&&
  !native_source_scene_v67.world.expired()&&!native_source_scene_v67.level.expired();
}
bool model_renderer::activate_source_campaign_scene_v67(AAssetManager* assets,std::string& error)try{
 auto& scene=native_source_scene_v67;SourceCampaignCandidateBorrowV55 actual;
 if(!assets||!source_campaign_loading_complete_v64()||!borrow_source_campaign_candidate_v55(actual,error)){
  if(error.empty())error="Native scene handoff requires genuine completed source loading";return false;
 }
 if(scene.phase==NativeSourceSceneV67::Phase::active){
  if(scene.world.lock()==actual.actual_world&&scene.level.lock()==actual.level){error.clear();return true;}
  error="Previous source scene requires actual teardown before replacement";return false;
 }
 if(scene.phase!=NativeSourceSceneV67::Phase::empty){error=scene.failure.empty()?"Retained source scene activation prefix cannot be replayed":scene.failure;return false;}
 scene.phase=NativeSourceSceneV67::Phase::preparing;scene.world=actual.actual_world;scene.level=actual.level;
 const auto fail=[&](){scene.phase=NativeSourceSceneV67::Phase::failed;scene.failure=error;return false;};
 PlayerGameplayBinding player;
 if(!borrow_source_campaign_player_gameplay_v67(actual.actual_world,player,error))return fail();
 std::shared_ptr<void> manager;
 if(!player.active||player.world_owner!=actual.actual_world||!player.character||!player.save||!player.gear||
    !player.gear->ready()||!actual.camera_application||!actual.camera_application->loaded()||
    actual.camera_application->roots()!=actual.roots){
  error="Source scene needs actual selected player/profile/Gear and loaded same-world camera";return fail();
 }
 auto pm=actual.application->source_player_manager_v59();dh2::player::PlayerInfoFieldsV1* local{};
 if(!pm||!pm->get_local_player(0,false,local,error)||!local||local->character660!=player.character){
  if(error.empty())error="Source scene player differs from published local Character660";return fail();
 }
 std::function<bool(const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>&,bool,std::string&)> gameplay;
 if(!borrow_source_campaign_gameplay_v64(actual,gameplay,error)||!gameplay||
    !borrow_native_menu_manager_v58(actual.actual_world,manager,error)||
    !prepare_source_campaign_geometry_v64(assets,error)||
    !original_ui.activate_source_player_v67(player,manager,error))return fail();
 if(!bind_source_campaign_gameplay_v64(actual.actual_world,actual.level,std::move(gameplay),error))return fail();
 // Loading remains visible through every failed source prefix. Only the
 // complete actual handoff changes Android presentation to gameplay.
 scene.phase=NativeSourceSceneV67::Phase::active;front_ui.deactivate();error.clear();return true;
}catch(const std::exception& failure){
 native_source_scene_v67.phase=NativeSourceSceneV67::Phase::failed;
 native_source_scene_v67.failure=error=failure.what();return false;
}
