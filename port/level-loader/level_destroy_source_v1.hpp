#pragma once
#include "level_unload_source_v1.hpp"
#include "game_event_manager_v50.hpp"
#include "script_manager_owner_v52.hpp"
#include "../level-world/level_savegame_runtime_v1.hpp"
#include "../level-world/character_loot_item_manager_v8.hpp"
#include "../level-world/generic_lua_script_owner_v13.hpp"
#include <map>
namespace dh2::loader {
// SAME Character::s_cachedCharOIDs map<int,unsigned> at original9a292c.
struct LevelCachedCharacterOIDsV1 {std::shared_ptr<void> owner;std::map<std::int32_t,std::uint32_t>* map{};};
enum class LevelMessageQueueV1 {status,tutorial,dialog,online_status};
struct LevelDestroyServicesV1 {
 std::shared_ptr<void> owner; // independent; each actual scope is borrowed at reach
 // Admission guard only: SAME render/scene/event/object delivery already
 // quiesced by the external lifecycle/Unload path. Drops no engine storage.
 std::function<bool(const std::shared_ptr<CanonicalLevelContextV1>&,std::string&)> require_quiescent_delivery;
 std::function<bool(const std::shared_ptr<CanonicalLevelContextV1>&,std::string&)> cleanup_skills,unlock_objects;
 std::function<bool(std::string&)> clear_all_aggro,clear_group_info,clear_concurrent_ai;
 std::function<bool(LevelCachedCharacterOIDsV1&,std::string&)> cached_character_oids;
 std::function<bool(LevelMessageQueueV1,std::string&)> flush_message_queue;
 std::function<bool(std::shared_ptr<ScriptManagerOwnerV52>&,std::string&)> scripts;
 // Genuine ScriptManager.Flush45a2ac; it is not UnLoadAllScripts45a1c8.
 std::function<bool(ScriptManagerOwnerV52&,std::string&)> script_flush;
 std::function<bool(LevelReleaseReceiverV1&,std::string&)> info_hud,hud_controls,projectiles,visual_fx,application,pf_world,animation_sets;
 std::function<bool(const LevelReleaseReceiverV1&,std::string&)> info_hud_flush,hud_controls_flush,projectile_flush,fx_flush_libraries,pf_flush,animation_sets_flush,clean_glitch;
 std::function<bool(std::shared_ptr<character::CharacterLootItemManagerV8>&,std::string&)> items;
 // Whole native Item cleanup may compose this SAME Flush with canonical
 // storage/tooltip retirement. No second source ItemManager Flush is called.
 std::function<bool(character::CharacterLootItemManagerV8&,std::string&)> item_flush;
 // Produces required real derived Objective D1 bodies bound to SAME manager.
 //Selected Objective D1s are base-vptr stores/BXLR. Native dispatcher adapter
 //aliases retire BEFORE loader storage release; this must not fabricate
 //gameplay Unregister, byte1c/Talk changes, inventory or marker callbacks.
 std::function<bool(const std::shared_ptr<GameEventManagerV50>&,GameEventNativeDestructionV1&,std::string&)> event_destructors;
 std::function<bool(std::uintptr_t,LevelReleaseReceiverV1&,std::string&)> batch_compiler;
 std::function<bool(const LevelReleaseReceiverV1&,std::string&)> batch_d1;
 std::function<bool(const LevelReleaseReceiverV1&,std::shared_ptr<world::CanonicalObjectManagerV1>&,std::string&)> objects;
 // Whole actual ObjectManager.Flush3496b8, on SAME source map/transport. Main
 // owns receiver class D0 bodies; Conditions compiled slots must clear before
 // class storage free. Each catalog release journal remains until genuine
 // class teardown AND manager/transport/draw unpublication are complete.
 std::function<bool(world::CanonicalObjectManagerV1&,std::string&)> object_flush;
 std::function<bool(const LevelReleaseReceiverV1&,std::string&)> zoom_set_camera_null;
 std::function<bool(const LevelReleaseReceiverV1&,LevelReleaseReceiverV1&,std::string&)> scene,physical;
 // Actual SceneManager virtual68 and active-camera NULL, distinct source calls.
 // virtual68 removes SAME authored roots/map clone membership/render aliases.
 // It cannot drop floor resource pins still owned by the SAME PF floor graph.
 std::function<bool(const LevelReleaseReceiverV1&,std::string&)> scene_virtual68,scene_active_camera_null,physical_clear;
 std::function<bool(const std::shared_ptr<void>&,std::shared_ptr<dh2::level::LevelSavegameRuntimeV1>&,std::string&)> save_runtime;
 std::function<bool(const LevelReleaseReceiverV1&,std::shared_ptr<dh2::scripts::LuaScriptCacheOwnerV13>&,std::string&)> lua_cache;
 // Actual embedded LuaScript44 D1; no new VM. The caller clears its retained
 // native-storage lease only after this body has closed the existing VM.
 std::function<bool(const std::shared_ptr<void>&,std::string&)> lua_script_d1;
 // Host ownership retirement after original base D1: does not perform or
 // substitute source teardown. Check SAME manager/transport/draw aliases are
 // quiescent, then retire existing graph/catalog/preparation journals only.
 // Independent BRES buffer pins held by PF/clone users must remain intact.
 std::function<bool(const std::shared_ptr<CanonicalLevelContextV1>&,std::string&)> retire_after_unpublication;
};
enum class LevelDestroyPhaseV1 {idle,quiesce,cleanup_skills,unlock_objects,clear_aggro,clear_groups,cached_oids,clear_concurrent,message_status,message_tutorial,message_dialog_first,message_dialog_second,message_online,script_flush,info_hud,hud_controls,items,projectiles,fx_libraries,event194,batch158,batch15c,application,object_flush,zoom_null,scene_clear,active_camera_null,physical_clear,pf_flush,animation_sets,event_flush,save_d0,clean_glitch,lua_cache,name_d1,lua_d1,event_base_d1,retire,complete,failed};
// Whole original Level D1/D2 source order (3f92c4 / 3f90b0). This is NOT
// Level::Unload, and never clears GSLevel34/s_level, closes HUD menu, starts
// another World or flushes another PF graph. Existing outer GSLevel Dtor owns
// Unload -> HUD close -> this body -> field34/s_level unpublication.
class LevelDestroySourceV1 final {
 std::weak_ptr<CanonicalLevelContextV1> level_;LevelDestroyServicesV1 services_;
 LevelDestroyPhaseV1 phase_{LevelDestroyPhaseV1::idle},failed_at_{LevelDestroyPhaseV1::idle};
 bool busy_{},attempted_{},complete_{};std::string failure_;
 bool fail(std::string& e){if(failure_.empty())failure_=e.empty()?"Required genuine Level D1 leaf at phase "+std::to_string(unsigned(phase_)):e;if(phase_!=LevelDestroyPhaseV1::failed){failed_at_=phase_;phase_=LevelDestroyPhaseV1::failed;}e=failure_;return false;}
 //The first failure is authoritative even when a provider clears its mutable error.
 bool continuing(std::string& e)const{if(!failure_.empty()){e=failure_;return false;}return true;}
 template<class F,class...A> bool call(LevelDestroyPhaseV1 p,const F& f,std::string& e,A&&...a){
  if(!continuing(e))return false;phase_=p;if(!f)return fail(e);
  const bool delivered=f(std::forward<A>(a)...,e);
  if(!continuing(e))return false;if(!delivered)return fail(e);return true;
 }
 template<class Get,class Flush>bool singleton(LevelDestroyPhaseV1 phase,const Get& get,const Flush& flush,std::string& e){LevelReleaseReceiverV1 receiver;if(!call(phase,get,e,receiver)||!receiver){if(e.empty())e="Required actual reached native singleton";return fail(e);}return call(phase,flush,e,receiver);}
public:
 LevelDestroySourceV1(std::weak_ptr<CanonicalLevelContextV1> level,LevelDestroyServicesV1 services):level_(std::move(level)),services_(std::move(services)){}
 bool execute(const std::shared_ptr<CanonicalLevelContextV1>& level,std::string& e){
  //Latch recursion at the active source phase, including a foreign nested receiver.
  if(busy_){e="Level D1 cannot reenter a reached prefix";return fail(e);}
  auto expected=level_.lock();if(!expected||!level||expected.get()!=level.get()||expected.owner_before(level)||level.owner_before(expected)||!services_.owner){e="Required SAME actual Level/independent D1 services";return false;}
  if(complete_){e.clear();return true;}if(attempted_){e=failure_.empty()?"Level D1 cannot replay/reenter a reached prefix":failure_;return false;}
  attempted_=true;busy_=true;struct Guard{bool& b;~Guard(){b=false;}}guard{busy_};e.clear();
  try{
   auto borrow=level->constructor_borrow_v3();if(!borrow.owner||borrow.identity!=level->identity()||!borrow.fields){e="Required SAME actual Level D1 source fields";return fail(e);}auto& fields=*borrow.fields;
   if(!call(LevelDestroyPhaseV1::quiesce,services_.require_quiescent_delivery,e,level))return false;
   if(!call(LevelDestroyPhaseV1::cleanup_skills,services_.cleanup_skills,e,level)||!call(LevelDestroyPhaseV1::unlock_objects,services_.unlock_objects,e,level)||!call(LevelDestroyPhaseV1::clear_aggro,services_.clear_all_aggro,e)||!call(LevelDestroyPhaseV1::clear_groups,services_.clear_group_info,e))return false;
   LevelCachedCharacterOIDsV1 ids;if(!call(LevelDestroyPhaseV1::cached_oids,services_.cached_character_oids,e,ids)||!ids.owner||!ids.map){if(e.empty())e="Required SAME Character.s_cachedCharOIDs tree";return fail(e);}if(!ids.map->empty())ids.map->clear();
   if(!call(LevelDestroyPhaseV1::clear_concurrent,services_.clear_concurrent_ai,e))return false;
   if(!call(LevelDestroyPhaseV1::message_status,services_.flush_message_queue,e,LevelMessageQueueV1::status)||!call(LevelDestroyPhaseV1::message_tutorial,services_.flush_message_queue,e,LevelMessageQueueV1::tutorial)||!call(LevelDestroyPhaseV1::message_dialog_first,services_.flush_message_queue,e,LevelMessageQueueV1::dialog)||!call(LevelDestroyPhaseV1::message_dialog_second,services_.flush_message_queue,e,LevelMessageQueueV1::dialog)||!call(LevelDestroyPhaseV1::message_online,services_.flush_message_queue,e,LevelMessageQueueV1::online_status))return false;
   std::shared_ptr<ScriptManagerOwnerV52> scripts;if(!call(LevelDestroyPhaseV1::script_flush,services_.scripts,e,scripts)||!scripts){if(e.empty())e="Required actual global ScriptManager";return fail(e);}if(!call(LevelDestroyPhaseV1::script_flush,services_.script_flush,e,*scripts))return false;
   if(!singleton(LevelDestroyPhaseV1::info_hud,services_.info_hud,services_.info_hud_flush,e)||!singleton(LevelDestroyPhaseV1::hud_controls,services_.hud_controls,services_.hud_controls_flush,e))return false;
   std::shared_ptr<character::CharacterLootItemManagerV8> items;if(!call(LevelDestroyPhaseV1::items,services_.items,e,items)||!items){if(e.empty())e="Required SAME actual ItemManager";return fail(e);}if((services_.item_flush?!services_.item_flush(*items,e):!items->flush(e))||!continuing(e))return fail(e);
   if(!singleton(LevelDestroyPhaseV1::projectiles,services_.projectiles,services_.projectile_flush,e)||!singleton(LevelDestroyPhaseV1::fx_libraries,services_.visual_fx,services_.fx_flush_libraries,e))return false;
   if(!continuing(e))return false;phase_=LevelDestroyPhaseV1::event194;
   GameEventLevelFieldsV50 actual_events;if(!level->game_event_fields_v50(actual_events,e)||!continuing(e)||!actual_events.field194||!actual_events.owner194){if(e.empty())e="Required SAME actual Level194 owning fields";return fail(e);}
   if(!fields.field194&&*actual_events.owner194){e="Level194 cleared before genuine native event teardown; retained lease cannot expire as D0";return fail(e);}
   if(fields.field194){
    auto& actual=actual_events;if(!*actual.owner194||*actual.field194!=reinterpret_cast<std::uintptr_t>(actual.owner194->get())){if(e.empty())e="Required SAME actual GameEvent194 receiver and lease";return fail(e);}
    auto manager=*actual.owner194;GameEventNativeDestructionV1 bodies;
    if(!services_.event_destructors||!services_.event_destructors(manager,bodies,e)||!continuing(e)||!manager->destroy_native_storage_v1(bodies,e)||!continuing(e))return fail(e);
    // D1/functional unregister + native vector free have REALLY completed.
    // A replacement alias must not be retired by this old receiver's journal.
    if(actual.owner194->get()!=manager.get()||actual.owner194->owner_before(manager)||manager.owner_before(*actual.owner194)||*actual.field194!=reinterpret_cast<std::uintptr_t>(manager.get())){e="GameEvent194 lease changed during native D1";return fail(e);}
    *actual.field194=0;actual.owner194->reset();
   }
   auto batch=[&](std::uintptr_t& slot,LevelDestroyPhaseV1 phase){if(!continuing(e))return false;phase_=phase;const auto identity=slot;if(!identity)return true;LevelReleaseReceiverV1 receiver;if(!services_.batch_compiler||!services_.batch_compiler(identity,receiver,e)||!continuing(e)||!receiver||receiver.identity!=identity){if(e.empty())e="Required SAME reached BatchNodeCompiler";return fail(e);}if(!services_.batch_d1||!services_.batch_d1(receiver,e)||!continuing(e))return fail(e);slot=0;return true;};
   if(!batch(fields.field158,LevelDestroyPhaseV1::batch158)||!batch(fields.field15c,LevelDestroyPhaseV1::batch15c))return false;
   LevelReleaseReceiverV1 app;if(!call(LevelDestroyPhaseV1::application,services_.application,e,app)||!app){if(e.empty())e="Required actual Application";return fail(e);}
   std::shared_ptr<world::CanonicalObjectManagerV1> objects;if(!call(LevelDestroyPhaseV1::object_flush,services_.objects,e,app,objects)||!objects){if(e.empty())e="Required SAME App38 ObjectManager";return fail(e);}if(!call(LevelDestroyPhaseV1::object_flush,services_.object_flush,e,*objects))return false;
   if(!call(LevelDestroyPhaseV1::zoom_null,services_.zoom_set_camera_null,e,app))return false;
   LevelReleaseReceiverV1 scene;if(!call(LevelDestroyPhaseV1::scene_clear,services_.scene,e,app,scene)||!scene){if(e.empty())e="Required SAME App10.Scene1c";return fail(e);}if(!call(LevelDestroyPhaseV1::scene_clear,services_.scene_virtual68,e,scene))return false;
   // Original rereads App10.Scene1c after virtual68; do not keep stale scene.
   scene={};if(!call(LevelDestroyPhaseV1::active_camera_null,services_.scene,e,app,scene)||!scene){if(e.empty())e="Required reread SAME SceneManager";return fail(e);}if(!call(LevelDestroyPhaseV1::active_camera_null,services_.scene_active_camera_null,e,scene))return false;
   LevelReleaseReceiverV1 physical;if(!call(LevelDestroyPhaseV1::physical_clear,services_.physical,e,app,physical)||!physical){if(e.empty())e="Required SAME App44 PhysicalWorld";return fail(e);}if(!call(LevelDestroyPhaseV1::physical_clear,services_.physical_clear,e,physical))return false;
   if(!singleton(LevelDestroyPhaseV1::pf_flush,services_.pf_world,services_.pf_flush,e)||!singleton(LevelDestroyPhaseV1::animation_sets,services_.animation_sets,services_.animation_sets_flush,e))return false;
   if(!continuing(e))return false;phase_=LevelDestroyPhaseV1::event_flush;if(!fields.events||fields.events->identity()!=borrow.identity){e="Required SAME inherited Level EventManager";return fail(e);}if(!fields.events->flush(e)||!continuing(e))return fail(e);
   if(!continuing(e))return false;phase_=LevelDestroyPhaseV1::save_d0;if(fields.save_ec){const auto actual=fields.save_ec;std::shared_ptr<dh2::level::LevelSavegameRuntimeV1> runtime;if(!services_.save_runtime||!services_.save_runtime(actual,runtime,e)||!continuing(e)||!runtime){if(e.empty())e="Required SAME actual LevelSavegame runtime";return fail(e);}if(!runtime->owner().release(e)||!continuing(e))return fail(e);if(fields.save_ec.get()!=actual.get()||fields.save_ec.owner_before(actual)||actual.owner_before(fields.save_ec)){e="Save_ec alias changed during genuine Save D0";return fail(e);}fields.save_ec.reset();}
   // Original reloads the actual singleton after Save D0, then caches this
   // SAME App for CleanGlitch and the subsequently reached LuaManager3c.
   app={};if(!call(LevelDestroyPhaseV1::clean_glitch,services_.application,e,app)||!app){if(e.empty())e="Required reread actual Application";return fail(e);}if(!call(LevelDestroyPhaseV1::clean_glitch,services_.clean_glitch,e,app))return false;
   std::shared_ptr<dh2::scripts::LuaScriptCacheOwnerV13> lua;if(!call(LevelDestroyPhaseV1::lua_cache,services_.lua_cache,e,app,lua)||!lua){if(e.empty())e="Required SAME App3c LuaManager cache";return fail(e);}lua->flush_buffered_files();
   if(!continuing(e))return false;phase_=LevelDestroyPhaseV1::name_d1;std::string{}.swap(fields.name_f8);
   if(!continuing(e))return false;phase_=LevelDestroyPhaseV1::lua_d1;if(!fields.script44){e="Required actual embedded Level LuaScript44";return fail(e);}auto script=fields.script44;if(!services_.lua_script_d1||!services_.lua_script_d1(script,e)||!continuing(e))return fail(e);if(fields.script44.get()!=script.get()||fields.script44.owner_before(script)||script.owner_before(fields.script44)){e="Embedded script44 lease changed during actual Lua D1";return fail(e);}fields.script44.reset();
   // Existing owner destructor IS the retained original EventManagerD1 body:
   // pending-node clear, delayed clear, pending clear, receiver tree clear.
   if(!continuing(e))return false;phase_=LevelDestroyPhaseV1::event_base_d1;fields.events.reset();
   if(!call(LevelDestroyPhaseV1::retire,services_.retire_after_unpublication,e,level))return false;
   if(!continuing(e))return false;phase_=LevelDestroyPhaseV1::complete;complete_=true;e.clear();return true;
  }catch(const std::exception& ex){e=ex.what();return fail(e);}catch(...){e="Original Level D1 provider threw; retained source prefix";return fail(e);}
 }
 auto phase()const noexcept{return phase_;}auto failed_at()const noexcept{return failed_at_;}
 bool complete()const noexcept{return complete_;}
};
// Installs loader-owned bodies into the EXISTING GSLevel lifecycle authority.
// Callbacks hold journals strongly but their actual Level scopes weakly. Main
// must retain these services only in its current external lifecycle/journal,
// never inside a Level owner they would pin through providers.
inline bool bind_level_release_source_v1(GSLevelServicesV2<CanonicalLevelContextV1>& services,
 std::shared_ptr<LevelUnloadSourceV1> unload,std::shared_ptr<LevelDestroySourceV1> destroy,std::string& e){
 if(!unload||!destroy||services.unload_level||services.destroy_level){e="Require unbound SAME GSLevel unload/D1 source slots";return false;}
 services.unload_level=[unload=std::move(unload)](const auto& level,std::string& error){return unload->execute(level,error);};
 services.destroy_level=[destroy=std::move(destroy)](const auto& level,std::string& error){return destroy->execute(level,error);};e.clear();return true;
}
}
