#include "source_campaign_retirement_v88.hpp"
#include "source_campaign_startup_abort_v114.hpp"
#include "source_campaign_release_v88.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_process_objects_v121.hpp"
#include "source_campaign_ai_queue_v105.hpp"
#include <area_transition_request_v114.hpp>
#include <projectile_precache_source_v96.hpp>
#include "projectile_precache_campaign_v96.hpp"
#include <level_batching_source_v96.hpp>
#include <native_batch_resources_v110.hpp>
#include "source_campaign_batch_compilation_v111.hpp"
#include "source_campaign_class_release_v106.hpp"
#include "source_campaign_module_rooms_v91.hpp"
#include "source_campaign_object_loading_deferred_v106.hpp"
#include "source_campaign_gameplay_inputs_v107.hpp"
#include "actual_device_android_v54.hpp"
#include "native_resource_budget_v38.hpp"
#include <level_lightset_stage9_v53.hpp>
#include <stage_loader_audio_v94.hpp>
#include "native_source_script_ui_v98.hpp"
#include "source_campaign_script_runtime_v99.hpp"
#include "source_campaign_save_objects_v86.hpp"
#include "source_campaign_items_v88.hpp"
#include "source_campaign_file_io_v65.hpp"
#include "../level-loader/stage_loader_stage34_v1.hpp"
#include "retained_character_position_owner_v7.hpp"
#include "canonical_character_candidate_v60.hpp"
#include "source_campaign_zones_v83.hpp"
#include "source_campaign_events_v75.hpp"
#include "source_campaign_anchor_v75.hpp"
#include "source_campaign_fx_v77.hpp"
#include "model_renderer.hpp"
#include "renderer_campaign_conditions_v75.hpp"
#include "source_campaign_fx_v77.hpp"
#include "../level-loader/native_root_gs_bootstrap_v55.hpp"
#include "../level-loader/stage_loader_v52_script_manager.hpp"
#include "../level-loader/retained_level_module_graph_v1.hpp"
#include "../level-loader/level_root_filename_route_v52.hpp"
#include "../level-loader/script_command_receivers_v59.hpp"
#include "canonical_module_graph_v3.hpp"
#include "canonical_gameobject_graph_v68.hpp"
#include "level_config_publication_v2.hpp"
#include "scene_manager_map_owner_v2.hpp"
#include "character_game_design.hpp"
#include "character_design_services.hpp"
#include "gameplay_camera_application_v23.hpp"
#include "application_player_manager_bootstrap_v59.hpp"
#include "application_spawn_random_owner_v4.hpp"
#include "visual_fx_manager_libraries_v63.hpp"
#include "campaign_navigation_registry_v64.hpp"
#include "renderer_native_menu_prefix_v62.hpp"
#include "renderer_native_menu_load_v98.hpp"
#include "source_campaign_terminal_v97.hpp"
#include "source_campaign_faery_v109.hpp"
#include <terminal_loading_v97.hpp>
#include <player_controller_attachment_v70.hpp>
#include <captured_menu_lease_v101.hpp>
#include "../level-loader/level_gameplay_update_v66.hpp"
#include "physical_world.hpp"
#include "player_manager_post_init_v66.hpp"
#include "module_fog_v67.hpp"
#include "level_fog_source_v103.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "gameplay_camera_anchor_v6.hpp"
#include "authored_camera_basis_v22.hpp"
#include "canonical_point3d_globals_v1.hpp"
#include "source_input_manager_v60.hpp"
#include "../engine-audio/audio_level_gameplay_v67.hpp"
#include "../level-loader/level_player_placement_v68.hpp"
#include "../level-loader/stage_loader_v50_game_events.hpp"
#include <android/log.h>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <tuple>
#include <utility>
namespace model_renderer {
bool borrow_source_campaign_physical_services_v90(const std::shared_ptr<SourceWorldBorrowV61>& world,
 dh2::world::CanonicalZonePhysicalServicesV82& out,std::string& e){
 if(!world||!world->physical_services_v90){e="Required actual composed native physical services";return false;}
 return world->physical_services_v90(out,e);
}
// Implemented by the production canonical Character provider bridge. This
// lends THIS completed candidate, rather than constructing a Crypt actor.
bool bind_campaign_character_providers_v62(AAssetManager*,const SourceCampaignCandidateBorrowV55&,std::string&);
namespace {
struct RendererSourceFactoryCoreV55 {
 std::shared_ptr<SourceCanonicalBorrowV61> canonical;
 std::unique_ptr<dh2::world::CanonicalLevelModuleBindingsV2> modules;
 std::function<bool(const dh2::world::CanonicalFactoryEntryV1&,
  const dh2::world::CanonicalSourceObjectRequestV1&,dh2::world::CanonicalClassReceiverV1&,std::string&)> remaining;
 std::function<bool(const char*,std::string&)> unknown_type_debug;
 static bool construct(void* raw,const dh2::world::CanonicalFactoryEntryV1& factory,
  const dh2::world::CanonicalSourceObjectRequestV1& request,dh2::world::CanonicalClassReceiverV1& out,std::string& error){
  auto& self=*static_cast<RendererSourceFactoryCoreV55*>(raw);
  if(factory.name&&(!std::strcmp(factory.name,"Module")||!std::strcmp(factory.name,"Block")||!std::strcmp(factory.name,"LevelConfig")))
   return self.modules->construct_receiver(factory,request,out,error);
  if(!self.remaining){error=std::string("Required actual source catalog constructor: ")+(factory.name?factory.name:"NULL");return false;}
  return self.remaining(factory,request,out,error);
 }
 static bool unknown(void* raw,const char* name,std::string& error){
  auto& self=*static_cast<RendererSourceFactoryCoreV55*>(raw);
  if(!self.unknown_type_debug){error="Required actual unknown catalog Debug provider";return false;}
  return self.unknown_type_debug(name,error);
 }
};
struct RendererSourceCandidateInputsV55 {
 std::shared_ptr<SourceWorldBorrowV61> world;
 std::shared_ptr<dh2::loader::CanonicalLevelContextV1> level;
 std::shared_ptr<SourceWorldBorrowV61> application;
 std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50> profile;
 // Actual application's SceneManager/PF owners; never inspection clones.
 std::shared_ptr<dh2::world::GameObjectSceneRootRegistryV1> roots;
 std::shared_ptr<dh2::floors::World> floors;
 std::shared_ptr<dh2::world::SceneManagerMapOwnerV2> map;
 dh2::world::ModulePFDebugV3 pf_debug;
 dh2::world::CanonicalModuleGraphServicesV3 module_graph;
 dh2::world::LevelConfigServicesV1 config;
 std::function<bool(const dh2::world::CanonicalSourceObjectRequestV1&,
  const std::shared_ptr<dh2::world::CanonicalModuleRecordV2>&,
  dh2::world::GameObjectInitializationServicesV1&,dh2::world::ModuleInitServicesV1&,std::string&)> module_platform;
 decltype(RendererSourceFactoryCoreV55::remaining) remaining;
 std::function<bool(const char*,std::string&)> unknown_type_debug;
 dh2::loader::CanonicalFileSourceServicesV1 file_services;
};
struct RendererSourceCandidateV55 {
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::shared_ptr<SourceCanonicalBorrowV61> canonical;
 std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50> profile;
 std::shared_ptr<RendererSourceFactoryCoreV55> factory;
 std::shared_ptr<dh2::loader::CanonicalReceiverTransportV1> transport;
 std::shared_ptr<dh2::world::CanonicalModuleGraphV3> graph;
 std::shared_ptr<dh2::world::ModulePFRoomsV3> rooms;
 std::shared_ptr<dh2::loader::RetainedLevelModuleGraphV1> preparation;
 std::shared_ptr<dh2::navigation::CampaignNavigationRegistryV64> navigation;
 dh2::world::CanonicalObjectManagerV1::FreshSourceBorrowV50 manager_freshness;
 static bool create(RendererSourceCandidateInputsV55 in,std::shared_ptr<RendererSourceCandidateV55>& out,std::string& error){
  using namespace dh2;using namespace dh2::world;using namespace dh2::loader;
  if(!in.world||!in.world->canonical_world||!in.level||!in.application||!in.profile||
     in.profile->private_directory!=in.world->files_directory||!in.roots||!in.floors||!in.map){
   error="Required actual fresh source World/C1/Application/Scene/PF owners";return false;
  }
  const auto* ctor=in.level->constructor_owner_v3();
  if(!ctor||ctor->phase()!=LevelConstructorPhaseV3::complete){error="Source candidate graph requires actual completed C1";return false;}
  auto candidate=std::make_shared<RendererSourceCandidateV55>();candidate->world=in.world;candidate->canonical=in.world->canonical_world;candidate->profile=std::move(in.profile);
  candidate->navigation=std::make_shared<dh2::navigation::CampaignNavigationRegistryV64>();
  in.world->source_pf_navigation_v115=candidate->navigation;
  auto manager=candidate->canonical->manager_lease;
  if(!manager->borrow_fresh_source_v50(manager,candidate->manager_freshness,error))return false;
  if(candidate->canonical->scene_roots_v20&&candidate->canonical->scene_roots_v20!=in.roots){error="Source candidate attempted a second SceneManager root owner";return false;}
  candidate->canonical->scene_roots_v20=in.roots;
  const auto& source=in.level->constructor_fields_v3();
  in.world->host_level->row_index=source.row3c;in.world->host_level->difficulty=source.difficulty40;
  candidate->rooms=std::make_shared<ModulePFRoomsV3>(in.floors,in.map,std::move(in.pf_debug));
  in.world->source_pf_rooms_v115=candidate->rooms;
  auto graph_services=std::move(in.module_graph);
  if(graph_services.rooms||graph_services.candidate){error="Source Module graph must borrow this sole canonical candidate/PF owner";return false;}
  graph_services.rooms=candidate->rooms;graph_services.candidate=candidate->canonical->owner;
  if(graph_services.visual.roots&&graph_services.visual.roots!=in.roots){error="Source Module visual uses foreign SceneManager roots";return false;}
  graph_services.visual.roots=in.roots;
  graph_services.visual.retire_root_gpu_v107=[weak=std::weak_ptr<SourceWorldBorrowV61>(in.world)](std::uintptr_t root,std::string& e){auto w=weak.lock();if(!w){e="Retired actual Module GPU release scope";return false;}return retire_source_campaign_root_geometry_v106(w->owner,root,e);};
  // SAME actual V54 device witness, independent of high-performance flags.
  graph_services.visual.driver_type=[](std::int32_t& type,std::string& e){return model_renderer::borrow_actual_device_driver_type_v55(type,e);};
  candidate->graph=std::make_shared<CanonicalModuleGraphV3>(std::move(graph_services));
  auto core=std::make_shared<RendererSourceFactoryCoreV55>();core->canonical=candidate->canonical;core->remaining=std::move(in.remaining);core->unknown_type_debug=std::move(in.unknown_type_debug);
  CanonicalLevelModuleConstructionV2 construction;construction.candidate=candidate->canonical->owner;
  construction.module_globals=&in.application->level_application->module_globals;construction.level_config=std::move(in.config);
  construction.level_config.set_level_config=[weak=std::weak_ptr<RendererSourceFactoryCoreV55>(core),
    expected=std::weak_ptr<CanonicalLevelContextV1>(in.level)](std::uintptr_t identity,std::string& e){
   auto factory=weak.lock();auto level=expected.lock();CanonicalCurrentLevelBorrowV1 current;
   if(!factory||!level||!model_renderer::borrow_current_native_level_v27(current,e)||current.level()!=level){
    if(e.empty())e="LevelConfig InitPost requires SAME actual current Level";return false;
   }
   const auto fields=level->config_fields();
   // Original3f150c publishes config38 before assertingNULL or looking up
   // Arrays.Sounds. Preserve that prefix even if its later provider fails.
   if(!fields.config38){e="Required actual Level config38 publication field";return false;}
   *fields.config38=identity;
   if(!identity){e="Original SetLevelConfig NULL assertion/unsafe branch required";return false;}
   // SetLevelConfig resolves names against the SAME registered Arrays::Sounds
   // retained by the real Application SoundManager; no second sound table.
   audio::AudioApplicationBorrowV42 sound;
   if(!model_renderer::borrow_actual_application_audio_v42(sound,e))return false;
   auto* runtime=sound.manager?sound.manager->runtime_on_producer():nullptr;
   if(!runtime||!runtime->source_data_initialized()){
    e="Required actual Application Arrays::Sounds initialization";return false;
   }
   LevelConfigPublicationBorrowV2 publication{fields.level_owner,fields.config38,
    fields.music11c,fields.safezone120,fields.ambient124,sound.manager,&runtime->bindings().names(),
    [weak](std::uintptr_t config){auto factory=weak.lock();return factory&&factory->modules?factory->modules->level_config(config):nullptr;}};
   return level_config_publication_v2(publication,identity,e);
  };
  construction.module_services=[weak=std::weak_ptr<CanonicalModuleGraphV3>(candidate->graph),platform=std::move(in.module_platform)](const auto& request,const auto& record,auto& init,auto& module,std::string& e){
   auto graph=weak.lock();if(!graph){e="Actual source Module graph was released";return false;}
   if(platform&&!platform(request,record,init,module,e))return false;
   init.device_high_performance=[](bool& high,std::string& error){return model_renderer::borrow_actual_device_high_performance_v54(high,error);};
   return graph->bind(record,init,module,e);
  };
  core->modules=std::make_unique<CanonicalLevelModuleBindingsV2>(core->canonical->properties,CanonicalClassServicesV1{},std::move(construction));
  candidate->factory=core;
  CanonicalReceiverTransportServicesV1 transport_services;transport_services.owner=core;transport_services.context=core.get();
  transport_services.construct=RendererSourceFactoryCoreV55::construct;transport_services.unknown_type_debug=RendererSourceFactoryCoreV55::unknown;
  candidate->transport=std::make_shared<CanonicalReceiverTransportV1>(core->canonical->properties,std::move(transport_services));
  assets::ZipAssetPackV1 archive;
  if(!in.application->archive(archive,error))return false;
  auto classes=candidate->transport->services();
  if(!RetainedLevelModuleGraphV1::create({archive,in.level,candidate->canonical->owner,&candidate->canonical->manager,classes,
        std::move(in.file_services),candidate->graph,in.floors,candidate->rooms},candidate->preparation,error))return false;
  if(!candidate->preparation->bind_module_record_provider_v44(core,[weak=std::weak_ptr<RendererSourceFactoryCoreV55>(core)](const auto& object,std::shared_ptr<CanonicalModuleRecordV2>& record,std::string& e){
    auto source=weak.lock();if(!source){e="Source Module factory released";return false;}
    auto* actual=source->modules->module(object.identity);
    if(!actual||!object.lease||object.lease.get()!=actual){e="Actual registered Module has no SAME factory record";return false;}
    record=std::static_pointer_cast<CanonicalModuleRecordV2>(object.lease);e.clear();return true;
   },error))return false;
  out=std::move(candidate);error.clear();return true;
 }
 bool lend_loading_inputs_v55(dh2::loader::SourceLoadingInputsV43& out,std::string& error)const{
  if(!preparation||!canonical||world.expired()){error="Required live source candidate graph/World";return false;}
  if(out.preparation||out.manager){error="Source loading candidate must have sole preparation/manager";return false;}
  out.preparation=preparation;out.manager=canonical->manager_lease;
  // These sibling leases pin the raw dispatch callbacks without retaining
  // candidate or its containing World, avoiding a loading ownership cycle.
  out.resource_pins.push_back(transport);out.resource_pins.push_back(factory);
  out.resource_pins.push_back(graph);out.resource_pins.push_back(rooms);
  error.clear();return true;
 }
};
using SourceScriptFactoryV55=decltype(dh2::loader::ScriptManagerServicesV52::command_factory);
std::shared_ptr<SourceWorldBorrowV61> source_script_world_v62(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&);
SourceScriptFactoryV55 source_script_factory_v62(const std::shared_ptr<SourceWorldBorrowV61>& world){
 using namespace dh2::loader;
 ScriptCommandBehaviorV59 body;body.actual_owner=world->application;
 // ScriptManager survives Worlds. Retain weak process App/immutable Arrays
 // authority, and borrow the current native candidate only at a world leaf.
 // Never freeze its command factory onto the first Level's World receiver.
 body.init=[weak_app=std::weak_ptr<dh2::application::ApplicationServicesOwnerV5>(world->application),
   weak_design=std::weak_ptr<dh2::character::CharacterGameDesign>(world->design)](const CheckedCommandBorrowV59& command,std::string& e){
  auto app=weak_app.lock();auto design=weak_design.lock();
  if(!app||!design){e="Actual Application script Init authority expired";return false;}
  if(*command.kind8==10){
   // StartDialog459048 caches Data, obtains actual constants2c, then caches
   // Data+c BEFORE getConstant459068. Keep the source scalar through lookup.
   if(!std::dynamic_pointer_cast<Script_StartDialogReceiverV59>(command.actual_receiver)){
    e="StartDialog Init requires its actual concrete C1 receiver";return false;
   }
   auto constants=design->borrow();const auto* binding=constants.design();
   if(!binding||!binding->lookup){e="Required actual Application constants2c provider";return false;}
   const auto* field=command.actual_data->scalar(0x0c);
   if(!field||field->width!=4){e="Required StartDialog SAME Data+c signed32";return false;}
   std::int32_t authored{};std::memcpy(&authored,&field->bits,sizeof(authored));
   std::int32_t target{};
   if(binding->lookup(binding->context,0,"DialogStyles","EnterLocationDialog",&target)!=0){
    e="Actual PyDataConstants getConstant delivery failed";return false;
   }
   *command.skip4=authored==target?1:0;e.clear();return true;
  }
  if(*command.kind8==20){
   if(!std::dynamic_pointer_cast<Script_PlayEffectReceiverV59>(command.actual_receiver)){
    e="PlayEffect Init requires its actual concrete C1 receiver";return false;
   }
   // Original4598bc caches Data+8 BEFORE reading the current FX singleton.
   const auto* field=command.actual_data->scalar(8);
   if(!field||field->width!=4){e="Required PlayEffect SAME Data+8 signed32";return false;}
   std::int32_t id{};std::memcpy(&id,&field->bits,sizeof(id));
   const auto libraries=app->source_fx_libraries_v63();
   if(!libraries||!libraries->belongs_to(app)){e="Required SAME current Application FX singleton";return false;}
   return libraries->register_set_to_load(id,e);
  }
  if(*command.kind8==22||*command.kind8==23){
   auto current=source_script_world_v62(app);
   if(!current){e="Required actual current source World for script MenuManager";return false;}
   std::shared_ptr<void> manager;
   // Original GetInstance precedes the current Data read, including HUD miss.
   if(!model_renderer::borrow_native_menu_manager_v58(current->owner,manager,e))return false;
   CheckedCommandBorrowV59 data;if(!command.actual_receiver->checked_data_borrow(data,e))return false;
   const auto* name=data.actual_data->cstring(0x10);
   if(!name){e="Required Show/Hide SAME current Data CString10";return false;}
   if(std::strstr(name,"HUD"))return command.actual_receiver->write_operand_pointer(0x10,0,{},e);
   std::shared_ptr<void> menu;std::uintptr_t id{};
   if(!model_renderer::borrow_native_script_menu_v62(current->owner,name,menu,id,e))return false;
   return command.actual_receiver->write_operand_pointer(0x10,id,std::move(menu),e);
  }
  auto actual=source_script_world_v62(app);
  if(!actual||!actual->script_command_init){
   e="Required actual nontrivial canonical script Init "+std::string(command.descriptor->class_name);return false;
  }
  return actual->script_command_init(command,e);
 };
 // Source Execute/Finish remain real engine bodies; base Init is recovered
 // literal BXLR in the concrete command owner, not this adapter's fallback.
 return [body=std::move(body)](std::int32_t kind,ScriptCommandBorrowV52& out,std::string& e){
  std::shared_ptr<CanonicalScriptCommandV59> receiver;
  if(!create_script_command_receiver_v59(kind,body,receiver,e))return false;
  out=receiver->constructor_borrow();e.clear();return true;
 };
}
bool borrow_source_script_manager_v55(const std::shared_ptr<SourceWorldBorrowV61>& application,
 SourceScriptFactoryV55 factory,std::shared_ptr<dh2::loader::ScriptManagerOwnerV52>& out,std::string& error){
 using namespace dh2::loader;
 if(!application||!application->application||!application->files_owner){error="Required actual Application/cache ScriptManager services";return false;}
 auto& authority=*application->application;
 if(authority.source_script_manager_v52()){out=authority.source_script_manager_v52();error.clear();return true;}
 if(!factory){error="Required genuine canonical command C1 factory before ScriptManager publication";return false;}
 ScriptManagerServicesV52 services;services.owner=application->files_owner;
 services.open_file=[read=application->read](const std::string& name,ScriptFileV52& out,std::string& e){
  auto bytes=std::make_shared<std::vector<std::uint8_t>>();bool found{};
  if(!read(name,found,*bytes,e))return false;
  out={bytes,bytes->data(),bytes->size(),found};e.clear();return true;
 };
 services.command_factory=std::move(factory); // Real canonical command C1, never an opcode placeholder.
 auto actual=std::make_shared<ScriptManagerOwnerV52>(std::move(services));
 if(!authority.publish_source_script_manager_v52(actual,error))return false;
 out=std::move(actual);error.clear();return true;
}
bool bind_source_stage8_v55(const std::shared_ptr<RendererSourceCandidateV55>& candidate,
 const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& level,
 const std::shared_ptr<dh2::loader::ScriptManagerOwnerV52>& scripts,
 dh2::loader::EarlyLoadingDebugV46 debug,dh2::loader::SourceLoadingInputsV43& loading,std::string& error){
 using namespace dh2;using namespace dh2::loader;
 if(!candidate||!candidate->factory||!candidate->preparation||candidate->preparation->level()!=level||!scripts||loading.external.stage_body[8]){
  error="Stage8 requires SAME candidate/C1/sole actual ScriptManager and empty body slot";return false;
 }
 Stage8LevelBorrowV51 actual;if(!borrow_stage8_level_v51(level,actual,error))return false;
 if(!loading.external.stage_body[13]){
  const std::weak_ptr<RendererSourceCandidateV55> weak_candidate=candidate;
  const std::weak_ptr<CanonicalLevelContextV1> weak_level=level;
  const std::weak_ptr<ScriptManagerOwnerV52> weak_scripts=scripts;
  loading.external.stage_body[13]=[weak_candidate,weak_level,weak_scripts,trace=debug](std::string& e){
   auto candidate=weak_candidate.lock();auto level=weak_level.lock();auto scripts=weak_scripts.lock();
   auto world=candidate?candidate->world.lock():nullptr;SourceCampaignCandidateBorrowV55 current;
   if(!candidate||!level||!scripts||!world||!borrow_source_campaign_candidate_runtime_v61(current,e)||
    current.actual_world!=world->owner||current.level!=level||candidate->preparation->level()!=level||
    level->constructor_fields_v3().field130!=13||!world->application||world->application->source_script_manager_v52()!=scripts){
    if(e.empty())e="Required SAME actual state13/ScriptManager/candidate";return LifecycleStepV36::failed;
   }
   if(!early_loading_trace_v46(trace,e)||!scripts->init_commands_v95(e))return LifecycleStepV36::failed;
   e.clear();return LifecycleStepV36::complete;
  };
 }
 Stage8ServicesV51 services;services.debug=std::move(debug);
 const auto canonical=candidate->canonical;const auto weak=std::weak_ptr<RendererSourceFactoryCoreV55>(candidate->factory);
 services.borrow_actual_config=[weak,canonical](std::uintptr_t identity,Stage8ConfigBorrowV51& out,std::string& e){
  auto core=weak.lock();const auto* config=core?core->modules->level_config(identity):nullptr;
  if(!config){e="Required actual registered LevelConfig38 source receiver";return false;}
  const world::CanonicalObjectBorrowV1* object{};std::int32_t key{};
  bool more=canonical->manager.source_ordered_begin_v38(key,object);
  while(more&&(!object||object->identity!=identity))more=canonical->manager.source_ordered_next_v38(key,key,object);
  if(!more||!object||!object->lease){e="LevelConfig38 is not published in SAME canonical manager";return false;}
  auto* actual_config=const_cast<world::CanonicalLevelConfigV1*>(config);
  out={object->lease,identity,config->string(0x150),&config->dfog_colors(),config->scalar_float_v55(0x210),
   [weak,identity,actual_config](const std::string& text,std::string& e){
    auto core=weak.lock();if(!core||core->modules->level_config(identity)!=actual_config){e="Released or foreign LevelConfig string150 producer";return false;}
    auto fields=actual_config->properties().fields;
    if(!fields.write_string){e="Required genuine LevelConfig CString property writer";return false;}
    return fields.write_string(fields.context,0x150,text,e);
   }};
  e.clear();return true;
 };
 services.init_modules_fog=[weak,canonical](std::uintptr_t manager,
   const std::vector<std::array<float,3>>& colors,std::int32_t range,std::string& e){
  auto core=weak.lock();
  if(!core||manager!=reinterpret_cast<std::uintptr_t>(&canonical->manager)){
   e="InitModulesFogColor requires SAME actual source manager";return false;
  }
  std::vector<world::ModuleFogBorrowV67> modules;modules.reserve(canonical->manager.modules().size());
  for(const auto id:canonical->manager.modules()){
   auto* module=core->modules->module(id);
   if(!module||!module->receiver){e="Actual Module68 list has no retained factory receiver";return false;}
   modules.push_back({id,module->receiver->base().vector3(0x160),&module->receiver->source_fog_color_v67()});
  }
  // PLT30eda8 is lrand48, not rand nor the separate game Random channels.
  return world::source_init_module_fog_v67(modules,colors,range,[](std::int32_t& value,std::string& e){
   value=static_cast<std::int32_t>(::lrand48());e.clear();return true;
  },e);
 };
 // NULL38 default Spawn/override still needs its genuine producer. The
 // nonempty fog path now reaches actual Module68 receivers and libc stream.
 services.object_manager_owner=canonical;services.object_manager_identity=reinterpret_cast<std::uintptr_t>(&canonical->manager);
 services.script_manager=bind_script_manager_v52(scripts);
 loading.external.stage_body[8]=stage8_config_scripts_v51(std::move(actual),std::move(services));
 loading.resource_pins.push_back(scripts);error.clear();return true;
}
#include "renderer_source_stage9_v94.inc"
#include "renderer_source_stage15_audio_v94.inc"
#include "renderer_source_stage18_stage32_v95.inc"
struct RendererSourceCampaignV55 {
 std::shared_ptr<SourceWorldBorrowV61> world;
 std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50> profile;
 std::shared_ptr<SourceWorldBorrowV61> application;
 std::shared_ptr<RendererSourceCandidateV55> candidate;
 std::shared_ptr<dh2::camera::GameplayCameraApplicationV23> camera_application;
 std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59> player_manager;
 dh2::loader::GSLevelServicesV2<dh2::loader::CanonicalLevelContextV1> menu;
 dh2::loader::NativeRootGSBootstrapV55 bootstrap;
 std::shared_ptr<dh2::loader::NativeGSLevelGlobalsV27> globals;
 enum class RetirementPhaseV88 {idle,admit,cancel,gs_destroy,native_retire,prefix_quiesce,prefix_cancel,prefix_release,prefix_native_retire,failed};
 RetirementPhaseV88 retirement_phase_v88{RetirementPhaseV88::idle};
 SourceCampaignRetirementServicesV88 retirement_v88;
 SourceCampaignStartupAbortServicesV114 startup_abort_v114;
 bool startup_cancel_requested_v114{},startup_release_complete_v114{},startup_native_complete_v114{};
 bool retirement_busy_v88{};std::string retirement_failure_v88;
 RetirementPhaseV88 retirement_resume_v114{RetirementPhaseV88::idle};
 bool retirement_gs_complete_v114{},retirement_native_complete_v114{};
 std::string retirement_first_failure_v114;
 bool failed{},cancelling{},cancelled{},final_tail_complete{};std::string failure;
};
std::unique_ptr<RendererSourceCampaignV55> source_campaign_v55;
bool borrow_source_campaign_world_owner_impl_v104(std::shared_ptr<void>& out,std::string& e){
 out.reset();if(!source_campaign_v55||!source_campaign_v55->world||!source_campaign_v55->world->owner){e="Required retained actual campaign World";return false;}
 out=source_campaign_v55->world->owner;e.clear();return true;
}
bool borrow_source_campaign_admission_impl_v104(const std::shared_ptr<void>& world,std::shared_ptr<SourceCampaignAdmissionV104>& out,std::string& e){
 out.reset();
 if(!world||!source_campaign_v55||!source_campaign_v55->world||source_campaign_v55->world->owner!=world||
    source_campaign_v55->world->owner.owner_before(world)||world.owner_before(source_campaign_v55->world->owner)){
  e="Source admission addressed another actual retained World";return false;
 }
 out=source_campaign_v55->world->admission_v104;e.clear();return bool(out);
}
bool close_source_campaign_admission_impl_v104(const std::shared_ptr<void>& world,bool& pending,std::string& e){
 std::shared_ptr<SourceCampaignAdmissionV104> gate;
 return borrow_source_campaign_admission_impl_v104(world,gate,e)&&gate->close(pending,e);
}
bool require_source_campaign_quiescence_impl_v104(const std::shared_ptr<void>& world,std::string& e){
 std::shared_ptr<SourceCampaignAdmissionV104> gate;
 return borrow_source_campaign_admission_impl_v104(world,gate,e)&&gate->require_closed_quiescent(e);
}
bool rebind_source_campaign_admission_after_context_loss_impl_v104(std::string& e){
 std::shared_ptr<void> world;std::shared_ptr<SourceCampaignAdmissionV104> gate;
 return borrow_source_campaign_world_owner_impl_v104(world,e)&&borrow_source_campaign_admission_impl_v104(world,gate,e)&&gate->rebind_after_context_loss(e);
}
std::shared_ptr<SourceWorldBorrowV61> source_script_world_v62(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& application){
 if(!source_campaign_v55||!source_campaign_v55->world||source_campaign_v55->world->application!=application)return {};
 return source_campaign_v55->world;
}
bool source_debug_load_v55(const std::weak_ptr<SourceWorldBorrowV61>& weak,std::string& error){
 auto world=weak.lock();
 if(!world||!world->debug){error="Required same Application Debug source owner";return false;}
 const auto result=dh2_character_debug_load(world->debug.get(),world->debug_files);
 if(result!=1){error="Original DebugSwitches.load failed in bounded source domain: "+std::to_string(result);return false;}
 error.clear();return true;
}
bool source_debug_switch_v55(const std::weak_ptr<SourceWorldBorrowV61>& weak,
 const char* key,bool& value,std::string& error){
 auto world=weak.lock();std::uint32_t actual{};
 if(!world||!world->debug||!key){error="Required same actual Debug/key";return false;}
 const auto result=dh2_character_debug_get(&actual,world->debug.get(),key,world->debug_files);
 if(result!=1){error="Original DebugSwitches.GetSwitch failed: "+std::to_string(result);return false;}
 value=actual!=0;error.clear();return true;
}

#include "source_campaign_camera_v67.inc"
#include "renderer_source_stage34_v80.inc"
#include "renderer_source_batching_v96.inc"
#include "renderer_native_batch_resources_v110.inc"
#include "source_campaign_stage12_v98.inc"
#include "source_campaign_terminal_v97.inc"

bool connect_source_campaign_candidate_v55(RendererSourceCampaignV55& state,
 const std::shared_ptr<dh2::loader::NativeGSLevelRuntimeV27>& gs,
 const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& actual,
 dh2::loader::NativeRootLoadingInputsV50& out,std::string& error){
 using namespace dh2;using namespace dh2::loader;
 auto current_world=state.world;const auto application=state.world->application;
 const auto weak=std::weak_ptr<SourceWorldBorrowV61>(current_world);
 RendererSourceCandidateInputsV55 candidate;
 candidate.world=current_world;candidate.level=actual;candidate.application=state.application;
 candidate.profile=state.profile;
 candidate.roots=state.camera_application?state.camera_application->roots():nullptr;
 if(current_world->source_pf_floors_v115||current_world->source_pf_map_v115||
    current_world->source_pf_navigation_v115||current_world->source_pf_rooms_v115){
  error="Source candidate cannot replace a reached PF constructor prefix";return false;
 }
 candidate.floors=std::make_shared<floors::World>();
 current_world->source_pf_floors_v115=candidate.floors;
 candidate.map=std::make_shared<world::SceneManagerMapOwnerV2>(candidate.roots);
 current_world->source_pf_map_v115=candidate.map;
 candidate.config.owner=state.world->files_owner;
 candidate.config.debug_switch=[weak](const char* key,bool& value,std::string& e){return source_debug_switch_v55(weak,key,value,e);};
 // Actual arrays/sound catalog and remaining canonical class routes must be
 // supplied at their source call boundaries; no replacement LevelConfig or
 // Item is synthesized here. The factory retains each completed source prefix.
 candidate.module_graph.read_asset=[read=state.world->read](const std::string& name,
   std::shared_ptr<const std::vector<std::uint8_t>>& bytes,bool& found,std::string& e){
  auto result=std::make_shared<std::vector<std::uint8_t>>();
  if(!read(name,found,*result,e))return false;
  bytes=found?result:nullptr;e.clear();return true;
 };
 candidate.unknown_type_debug=[](const char* name,std::string& e){
  if(!name){e="Required unknown source factory type CString";return false;}
  __android_log_print(ANDROID_LOG_WARN,"DH2Native","Source unknown factory type | %s",name);e.clear();return true;
 };
 if(!RendererSourceCandidateV55::create(std::move(candidate),state.candidate,error))return false;
 if(!state.candidate->lend_loading_inputs_v55(out.source,error))return false;
 if(!bind_deferred_campaign_object_loading_v106(current_world,out.source,error))return false;
 // Original stage7 writes the process Random seeds at99f89c/99f8a0.
 // Borrow the same App-retained channels used by canonical spawn/loot.
 const auto random=application->source_random_v62();
 out.source.globals={random,&random->channel(0).seed,&random->channel(1).seed};
 out.source.procedural.online_byte5=[online=application->get_online_loading_v55()](std::uint8_t& value,std::string& e){
  value=online->byte5();e.clear();return true;
 };
 // The filesystem backend is the existing mounted cache directory. Resolve
 // metadata only, then the source walker acquires the selected payload once.
 assets::ZipAssetPackV1 mounted;
 if(!state.world->archive(mounted,error))return false;
 LevelRootFilenameServicesV52 filenames;filenames.actual_filesystem_owner=state.world->files_owner;
 filenames.is_using_uncompiled_data=[weak_app=std::weak_ptr<dh2::application::ApplicationServicesOwnerV5>(application)](
   const std::string& name,bool& value,std::string& e){
  auto app=weak_app.lock();if(!app){e="Actual Application expired during source filename mode query";return false;}
  value=original_level_uses_uncompiled_v52(!app->source_loading_v55().uncompiled_data_path_bc.empty(),name);
  e.clear();return true;
 };
 AssignedReadLeavesV65 parser;parser.owner=state.world->files_owner;
 parser.parse_result=[weak](bool success,std::string& e){
  if(!weak.lock()){e="Source XML parser-result authority expired";return false;}
  // Original LoadFile3f4014/18 has no assertion/debug work on a successful
  // actual TiXML parse. Its invalid/trap path is a strict native rejection;
  // this result observes the genuine parser, not an initialized-state policy.
  if(!success){e="Original Level XML parse failed; unsafe assertion path rejected";
   __android_log_print(ANDROID_LOG_ERROR,"DH2Native","%s",e.c_str());return false;}
  e.clear();return true;
 };
 parser.unavailable_buffer=[](std::string& e){
  e="Required original unavailable StreamBuffer assertion/trap policy";return false;
 };
 if(!source_campaign_detail_v61::bind_cache_filename_source_v65(mounted,state.world->files_owner,
    std::move(filenames.is_using_uncompiled_data),std::move(parser),out.source,error))return false;
 out.manager_freshness=state.candidate->manager_freshness;
 auto& fields=application->source_loading_v55();
 out.application.actual_owner=application;out.application.byte_ec=&fields.byte_ec;
 out.application.original_debug_word_global=&fields.lua_num_calls9a2400;
 out.application.original_debug_flag_global=&fields.lua_dump_call_list9a2404;
 out.stage0_application={application,application,application->identity(),
  &fields.g_bigI9f6890,&fields.g_bigV9f688c,&fields.byte_b4};
 out.stage0.clean_glitch=[application](const Stage0ApplicationBorrowV46& actual_app,std::string& e){
  if(actual_app.application_owner.get()!=application.get()||actual_app.application_identity!=application->identity()){
   e="Foreign Application.CleanGlitch receiver";return false;
  }
  return model_renderer::clean_native_graphics_v50(e);
 };
 if(!model_renderer::bind_root_stage0_sound_v44(out.stage0,error))return false;
 // This aliases the SAME process backend already used by native bodies. Stage5
 // invokes its actual load exactly once; no second PhysicalWorld is allocated.
 out.physical_world=state.world->physical_world;
 out.prefix.actual_application_owner=application;out.prefix.cheat_global_owner=application;
 out.prefix.handle_cheats_inGame=&fields.handle_cheats_inGame9f640a;
 out.prefix.debug_owner=current_world->debug;
 out.prefix.debug_load=[weak](std::string& e){return source_debug_load_v55(weak,e);};
 out.prefix.debug_get_switch=[weak](const char* key,bool& value,std::string& e){return source_debug_switch_v55(weak,key,value,e);};
 out.prefix.debug_instance_get_switch=out.prefix.debug_get_switch;
 // Native log transport for original _DEBUG_OUT's formatted loading-step
 // message. No source loading mutation or work-completion counter is added.
 out.prefix.debug_out_loading_step=[](std::uint32_t phase,std::string& e){
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original loading step | %u",phase);e.clear();return true;
 };
 out.early_debug={current_world->debug,out.prefix.debug_instance_get_switch};
 if(!bind_source_stage18_stage32_v95(current_world,state.candidate,actual,out.early_debug,out.source,error))return false;
 if(!bind_source_batching_stages_v96(current_world,state.candidate,actual,out.early_debug,out.source,error))return false;
 if(!bind_source_batch_resources_v110(current_world,state.candidate,actual,error))return false;
 if(!bind_source_stage33_stage34_v80(current_world,state.candidate,actual,out.early_debug,out.source.external,error))return false;
 if(!out.source.external.stage_body[29]){
  Stage29PrecacheServicesV96 preload;preload.debug=out.early_debug;
  preload.validate_current=[weak,level=std::weak_ptr<CanonicalLevelContextV1>(actual)](std::string& e){
   auto world=weak.lock();auto expected=level.lock();SourceCampaignCandidateBorrowV55 current;
   if(!world||!expected||!borrow_source_campaign_candidate_runtime_v61(current,e)||
      current.actual_world!=world->owner||current.level!=expected||expected->constructor_fields_v3().field130!=29||
      source_campaign_retirement_requested_v88()){
    if(e.empty())e="Required SAME current Stage29 projectile scope";
    return false;
   }
   return true;
  };
  preload.actual_item_precache=[validate=preload.validate_current](std::string& e){SourceCampaignCandidateBorrowV55 current;
   return validate(e)&&borrow_source_campaign_candidate_runtime_v61(current,e)&&precache_source_campaign_items_v88(current,e);
  };
  preload.projectile_sources=[weak,level=std::weak_ptr<CanonicalLevelContextV1>(actual)](ProjectilePrecacheSourcesV96& out,std::string& e){
   return lend_source_campaign_projectile_precache_v96(weak,level,out,e);
  };
  out.source.external.stage_body[29]=stage29_precache_source_v96(std::move(preload));
 }
 auto libraries=application->source_fx_libraries_v63();
 if(!libraries){
  fx::VisualFxLibraryDebugV63 debug;debug.application=application;
  debug.invoke=[weak_app=std::weak_ptr<dh2::application::ApplicationServicesOwnerV5>(application)](
    std::uint32_t operation,const char* name,std::uint32_t& value,std::string& e){
   auto app=weak_app.lock();auto world=app?source_script_world_v62(app):nullptr;
   if(!world||!world->fx_debug){e="Required current actual App FX Debug authority";return false;}
   return world->fx_debug(operation,name,value,e);
  };
  libraries=std::make_shared<fx::VisualFxManagerLibrariesV63>(state.world->effects->borrow(),std::move(debug));
  if(!application->publish_source_fx_libraries_v63(libraries,error))return false;
 }
 const auto tables=state.world->effects->borrow();
 if(!libraries->belongs_to(application)||&libraries->source_tables().sets()!=&tables.sets()||
    &libraries->source_tables().dictionary()!=&tables.dictionary()){
  error="Campaign FX library requires SAME App/immutable Arrays snapshot";return false;
 }
 out.source.external.stage_body[4]=[libraries,level=std::weak_ptr<CanonicalLevelContextV1>(actual),debug=out.early_debug](std::string& e){
  auto current=level.lock();
  if(!current||current->constructor_fields_v3().field130!=4){e="Stage4 requires SAME source current Level phase4";return LifecycleStepV36::failed;}
  if(!early_loading_trace_v46(debug,e)||!libraries->build_libraries(e))return LifecycleStepV36::failed;
  return LifecycleStepV36::complete;
 };
 out.source.resource_pins.push_back(libraries);
 GameEventLevelFieldsV50 actual_event_fields;
 if(!actual->game_event_fields_v50(actual_event_fields,error))return false;
 auto* pm_receiver=state.world->player_manager?state.world->player_manager->manager():nullptr;
 auto* pm_fields=pm_receiver?pm_receiver->source_frame_fields_v68():nullptr;
 if(!pm_fields||!state.world->game_event_tables){error="Required SAME completed PM C1 and actual immutable v2Events table";return false;}
 Stage6ServicesV50 event_services;event_services.debug=out.early_debug;
 event_services.player={std::shared_ptr<void>(state.world->player_manager,pm_receiver),
   reinterpret_cast<std::uintptr_t>(pm_receiver),&pm_fields->byte6c9};
 event_services.actual_events=state.world->game_event_tables->borrow();
 event_services.player_update=[weak](const Stage6PlayerBorrowV50& borrower,std::string& e){
  auto world=weak.lock();auto pm=world?world->player_manager:nullptr;
  auto* manager=pm?pm->manager():nullptr;auto* fields=manager?manager->source_frame_fields_v68():nullptr;
  if(!world||!pm||!manager||!fields||borrower.identity!=reinterpret_cast<std::uintptr_t>(manager)||
     borrower.actual_owner.get()!=manager||borrower.actual_owner.owner_before(pm)||pm.owner_before(borrower.actual_owner)||
     borrower.byte6c9!=&fields->byte6c9||!world->source_pm_update_provider_v70||!world->source_pm_update_v70){
   e="Required SAME real pre-player PlayerManager.Update378fb4 provider";return false;
  }
  return world->source_pm_update_v70(e);
 };
 out.source.external.stage_body[6]=stage6_game_events_v50(std::move(actual_event_fields),std::move(event_services));
 out.source.resource_pins.push_back(state.world->game_event_tables);
 out.source.external.stage_body[31]=[weak,level=std::weak_ptr<CanonicalLevelContextV1>(actual)](std::string& e){
  auto world=weak.lock();auto source=level.lock();
  if(!world||!source||source->constructor_fields_v3().field130!=31){e="Stage31 requires SAME current Level31/GameEvent194";return LifecycleStepV36::failed;}
  SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<GameEventRuntimeV75> events;
  if(!borrow_source_campaign_candidate_runtime_v61(candidate,e)||candidate.level!=source||
     candidate.actual_world!=world->owner||!borrow_source_campaign_events_v75(candidate,events,e)||!events->compile(e))
   return LifecycleStepV36::failed;
  e.clear();return LifecycleStepV36::complete; //Dispatcher alone owns31->32.
 };
 out.source.external.stage_body[11]=[prepared=std::weak_ptr<RetainedLevelModuleGraphV1>(state.candidate->preparation),
   debug=out.early_debug](std::string& e){
  auto source=prepared.lock();
  if(!source||source->level()->constructor_fields_v3().field130!=11){e="Stage11 requires SAME source PF/Level owner";return LifecycleStepV36::failed;}
  if(!early_loading_trace_v46(debug,e)||!source->prepare_floors(e))return LifecycleStepV36::failed;
  return LifecycleStepV36::complete;
 };
 out.source.external.stage_body[14]=[weak,level=std::weak_ptr<CanonicalLevelContextV1>(actual),debug=out.early_debug](std::string& e){
  auto world=weak.lock();auto source=level.lock();
  if(!world||!source||!world->player_manager||!world->canonical_world||source->constructor_fields_v3().field130!=14){
   e="Stage14 requires SAME actual PM/Level/ObjectManager";return LifecycleStepV36::failed;
  }
  if(!early_loading_trace_v46(debug,e))return LifecycleStepV36::failed;
  const auto empty=LevelPlayerPlacementServicesV68{};
  const auto& services=world->player_placement_v68?*world->player_placement_v68:empty;
  if(!source_place_level_players_v68(*source,*world->player_manager,world->canonical_world->manager,services,e))return LifecycleStepV36::failed;
  auto online=world->application->get_online_loading_v55();
  if(!online){e="Required actual Stage14 GetOnline";return LifecycleStepV36::failed;}
  if(online->byte5()){
   dh2::player::PlayerInfoFieldsV1* player{};bool active{};
   if(!world->player_manager->get_local_player(0,false,player,e)||!player||!world->player_manager->network()||
      !world->player_manager->network()->is_active_selected_v67(*player,online->byte5(),active,e)){
    if(e.empty())e="Required actual online local PlayerInfo selected5c";return LifecycleStepV36::failed;
   }
   if(!active){
    // Original inactive5c alone reaches matching virtual3c, connection
    // DisconnectAll, then Application.GoToMainMenu(3).
    e="Required original inactive Stage14 matching/disconnect/mainmenu tail";return LifecycleStepV36::failed;
   }
  }
  e.clear();return LifecycleStepV36::complete;
 };
 out.source.external.stage_body[20]=[weak,candidate=std::weak_ptr<RendererSourceCandidateV55>(state.candidate),
   level=std::weak_ptr<CanonicalLevelContextV1>(actual),debug=out.early_debug](std::string& e){
  auto world=weak.lock();auto graph=candidate.lock();auto source=level.lock();
  if(!world||!graph||!source||source->constructor_fields_v3().field130!=20){e="Stage20 requires SAME actual camera/World/Level";return LifecycleStepV36::failed;}
  if(!early_loading_trace_v46(debug,e)||!source_load_camera_v67(world,graph,source,e))return LifecycleStepV36::failed;
  return LifecycleStepV36::complete;
 };
 out.source.external.stage_body[25]=[level=std::weak_ptr<CanonicalLevelContextV1>(actual),debug=out.early_debug](std::string& e){
  auto source=level.lock();
  if(!source||source->constructor_fields_v3().field130!=25){e="Stage25 requires SAME source current Level";return LifecycleStepV36::failed;}
  if(!early_loading_trace_v46(debug,e)||!model_renderer::clean_native_graphics_v50(e))return LifecycleStepV36::failed;
  return LifecycleStepV36::complete;
 };
 out.source.external.stage_body[30]=[libraries,level=std::weak_ptr<CanonicalLevelContextV1>(actual),debug=out.early_debug](std::string& e){
  auto source=level.lock();
  if(!source||source->constructor_fields_v3().field130!=30){e="Stage30 requires SAME source FX/Level owner";return LifecycleStepV36::failed;}
  if(!early_loading_trace_v46(debug,e)||!libraries->precache_libraries(e))return LifecycleStepV36::failed;
  return LifecycleStepV36::complete;
 };
 std::shared_ptr<ScriptManagerOwnerV52> scripts;
 out.source.external.stage_body[26]=[weak,level=std::weak_ptr<CanonicalLevelContextV1>(actual),debug=out.early_debug](std::string& e){
  auto world=weak.lock();auto source=level.lock();
  if(!world||!source||source->constructor_fields_v3().field130!=26){e="Stage26 requires SAME actual World/Level";return LifecycleStepV36::failed;}
  SourceCampaignCandidateBorrowV55 candidate;
  if(!early_loading_trace_v46(debug,e)||!borrow_source_campaign_candidate_runtime_v61(candidate,e)||
     candidate.actual_world!=world->owner||!bind_native_source_script_ui_v98(candidate,e)||
     !model_renderer::source_native_loadmenu3_v62(world->owner,e)||
     !model_renderer::bind_native_character_reward_text_v114(world->owner,e)||
     !model_renderer::bind_native_campaign_combat_presentation_v115(world->owner,e)||
     !model_renderer::bind_native_character_interaction_ui_v115(world->owner,e)||
     !model_renderer::refresh_actual_message_caches_stage26_v66(e))return LifecycleStepV36::failed;
  if(!world->post_init_characters){e="Required actual PlayerManager.PostInitCharacters36f2bc";return LifecycleStepV36::failed;}
  if(!world->post_init_characters(e)||!model_renderer::complete_actual_hud_refresh_stage26_v66(e))return LifecycleStepV36::failed;
  return LifecycleStepV36::complete;
 };
 if(!borrow_source_script_manager_v55(state.world,source_script_factory_v62(state.world),scripts,error)||
    !bind_source_stage8_v55(state.candidate,actual,scripts,out.early_debug,out.source,error)||
    !bind_source_stage9_v94(state.world,state.candidate,actual,out.source,error)||
    !bind_source_stage15_audio_v94(state.world,state.candidate,actual,out.early_debug,borrow_source_resource_prefix_v95,out.source,error))return false;
 out.frame.actual_debug_owner=current_world->debug;
 out.frame.debug_load=out.prefix.debug_load;
 out.frame.debug_get_switch=out.prefix.debug_get_switch;
 // Positive Lua_DumpCalls requires the original SetSwitch/save provider; it
 // is not silently downgraded to the missing-file false source branch.
 out.frame.debug_set_switch=[](const char*,bool,std::string& e){e="Required actual DebugSwitches.SetSwitch/save positive branch";return false;};
 if(!model_renderer::borrow_native_gs_frame_menu_v58(current_world->owner,out.frame,error))return false;
 out.gameplay=[weak,level=std::weak_ptr<CanonicalLevelContextV1>(actual)](const std::shared_ptr<CanonicalLevelContextV1>& receiver,bool force,std::string& e){
  auto world=weak.lock();auto expected=level.lock();CanonicalCurrentLevelBorrowV1 current;
  if(!world||!expected||receiver!=expected||receiver.owner_before(expected)||expected.owner_before(receiver)||
     !model_renderer::borrow_current_native_level_v27(current,e)||current.level()!=expected){
   if(e.empty())e="Gameplay update requires SAME actual source World/current Level";return false;
  }
  if(!world->gameplay_update){e="Required actual canonical Level.Update gameplay producer";return false;}
  return world->gameplay_update(receiver,force,e);
 };
 out.source.external.stage_body[2]=[weak,trace=out.early_debug](std::string& e){
  if(!early_loading_trace_v46(trace,e))return LifecycleStepV36::failed;
  auto world=weak.lock();if(!world){e="Released actual stage2 World";return LifecycleStepV36::failed;}
  for(const auto id:{2,1,3})if(!model_renderer::unload_native_menu_v58(world->owner,id,e))return LifecycleStepV36::failed;
  e.clear();return LifecycleStepV36::complete;
 };
 out.source.external.publish_progress=[menu=state.menu,actual,weak,stable=&state](std::int32_t phase,std::int32_t progress,std::string& e){
  CanonicalLevelContextV1::LoadingFieldsV26 fields;
  if(!actual->loading_fields_v26(fields,e)||!fields.state130||!fields.progress30||
     *fields.state130!=std::uint32_t(phase)||*fields.progress30!=std::uint32_t(progress)){
   if(e.empty())e="Source progress must observe SAME actual C1 scalar tail";return false;
  }
  if(phase==38){
   auto world=weak.lock();if(!world){e="Expired actual World before source completion tail";return false;}
   // Original3f6f6c reads the SAME USE_NATIVE_DRM_GAME global, then validates
   // with argument1 only when true. Fade menu lookup/push follows either path.
   bool native_drm{};
   if(!model_renderer::borrow_native_campaign_drm_v93(world->owner,native_drm,e))return false;
   if(native_drm&&!model_renderer::validate_native_campaign_license_v93(world->owner,1,e))return false;
   std::uintptr_t fade{};
   if(!menu.get_menu||!menu.get_menu("menu_FadeFromBlackScreen",fade,e)||!menu.push_menu)return false;
   // There is no source NULL check before PushMenu. The actual native menu
   // provider owns its source-invalid NULL rejection; do not skip that call.
   if(!menu.push_menu(fade,e))return false;
  }
  std::uintptr_t receiver{},render{},character{};
  if(!menu.get_menu||!menu.get_menu("menu_Loading",receiver,e))return false;
  if(!receiver){if(phase==38)stable->final_tail_complete=true;e.clear();return true;} // Genuine GetMenu NULL source branch.
  if(!menu.menu_render_fx||!menu.menu_render_fx(receiver,render,e)||
     !menu.check_menu_weak_proxy||!menu.check_menu_weak_proxy(receiver,e)||
     !menu.menu_character||!menu.menu_character(receiver,character,e)||
     !menu.invoke_as_no_arguments)return false;
  if(!menu.invoke_as_no_arguments(render,character,"onProgress",e))return false;
  if(phase==38)stable->final_tail_complete=true;
  return true;
 };
  if(!bind_source_campaign_terminal_v97(current_world,actual,out.early_debug,out.prefix,state.menu.get_menu,out.source,error))return false;
 if(!bind_source_campaign_stage12_v98(current_world,actual,out.source,error))return false;
  // All source body/progress/tail authorities remain explicit. Later root
 // stages bind their genuine providers into this SAME candidate connection.
 (void)gs;
 error.clear();return true;
}


}
#include "source_campaign_module_rooms_v91.inc"
bool borrow_source_campaign_world_owner_v104(std::shared_ptr<void>& out,std::string& e){
 return borrow_source_campaign_world_owner_impl_v104(out,e);
}
bool borrow_source_campaign_admission_v104(const std::shared_ptr<void>& world,std::shared_ptr<SourceCampaignAdmissionV104>& out,std::string& e){
 return borrow_source_campaign_admission_impl_v104(world,out,e);
}
bool close_source_campaign_admission_v104(const std::shared_ptr<void>& world,bool& pending,std::string& e){
 return close_source_campaign_admission_impl_v104(world,pending,e);
}
bool require_source_campaign_quiescence_v104(const std::shared_ptr<void>& world,std::string& e){
 return require_source_campaign_quiescence_impl_v104(world,e);
}
bool rebind_source_campaign_admission_after_context_loss_v104(std::string& e){
 return rebind_source_campaign_admission_after_context_loss_impl_v104(e);
}
bool start_source_campaign_runtime_v61(AAssetManager* assets,
 std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50> profile,SourceCampaignRendererServicesV61 renderer,std::string& error,
 const dh2::loader::AreaTransitionRequestV114* prepared_transition)try{
 using namespace dh2;using namespace dh2::loader;
 if(source_campaign_v55){error="Source campaign already retained; require whole source unload before a new launch";return false;}
 if(!assets||!profile||profile->metadata.slot<0||profile->metadata.selected_difficulty<0||profile->metadata.selected_difficulty>=3){
  error="Required immutable selected profile and genuine per-difficulty location";return false;
 }
 // Confirmed transitions consume one previously projected live Save tuple.
 // The profile is Main's fresh restore projection, never cached menu state.
 if(prepared_transition){
  const auto& plan=*prepared_transition;
  if(!plan.restore.file||plan.restore.profile_slot!=profile->metadata.slot||
     plan.application.profile_slot!=profile->metadata.slot||
     plan.source_difficulty!=profile->metadata.selected_difficulty||
     plan.gs.word20!=std::uint32_t(profile->metadata.slot)||plan.source.seed!=plan.gs.word20||
     plan.source.identity.empty()||plan.source.definition.empty()||plan.destination_row<0||
     plan.gs.name18!=plan.source.definition||plan.application.filename!=plan.source.definition){
   error="Required SAME actual restore profile/slot and confirmed source/GS transition tuple";return false;
  }
 }
 if(!renderer.globals||!renderer.create_world){error="Required actual renderer source boundary";return false;}
 if(renderer.globals->s_level){error="Original GS current Level exists; require actual unload";return false;}
 auto owned=std::make_unique<RendererSourceCampaignV55>();owned->profile=std::move(profile);
 owned->globals=renderer.globals; // Retain actual process slots even if create_world fails.
 source_campaign_v55=std::move(owned);auto* retained=source_campaign_v55.get();
 if(!renderer.create_world(assets,retained->profile,retained->world,error)||!retained->world){
  retained->failed=true;retained->failure=error;return false;
 }
 auto* state=retained;
 if(state->world->apk_assets_v111&&state->world->apk_assets_v111!=assets){
  error="Source World APK provider differs from actual native launch";state->failed=true;state->failure=error;return false;
 }
 state->world->apk_assets_v111=assets;
 const auto& actual_world=*state->world;const auto& canonical=actual_world.canonical_world;
 const auto process_objects=actual_world.application?actual_world.application->source_objects_v121():nullptr;
 if(!actual_world.owner||!actual_world.application||!actual_world.design||!actual_world.effects||!actual_world.fx_debug||!canonical||!canonical->owner||
    !canonical->manager_lease||canonical->manager_lease.get()!=&canonical->manager||
    !process_objects||!process_objects->belongs_to(actual_world.application)||
    process_objects->manager().get()!=canonical->manager_lease.get()||
    process_objects->manager().owner_before(canonical->manager_lease)||canonical->manager_lease.owner_before(process_objects->manager())||
    !actual_world.debug||!actual_world.debug_files||!actual_world.host_level||!actual_world.level_application||
    !actual_world.files_owner||!actual_world.read||!actual_world.archive||!actual_world.camera_application||
    !actual_world.player_manager||!actual_world.physical_world||!actual_world.native_level_c1_v25||
    !actual_world.native_gslevel_v27||!actual_world.native_loading_v50||!actual_world.last_frame||
    !actual_world.application_dt||!actual_world.application_tick||*actual_world.native_loading_v50){
  error="Required coherent actual renderer boundary/sole empty loading slot";state->failed=true;state->failure=error;return false;
 }
 const auto application=state->world->application;
 state->application=state->world;state->camera_application=state->world->camera_application;
 state->player_manager=state->world->player_manager;state->globals=renderer.globals;
 const auto difficulty=std::size_t(state->profile->metadata.selected_difficulty);
 auto tables=state->world->design->borrow();const auto* levels=tables.levels();
 const auto row=prepared_transition?prepared_transition->destination_row:state->profile->metadata.location.levels[difficulty];
 if(!levels||row<0||std::size_t(row)>=levels->levels.size()){
  error="Required authentic selected LNAM/default row";state->failed=true;state->failure=error;return false;
 }
 if(prepared_transition){
  std::int32_t original_row=-1;const dh2::data::LevelRecord* original_record{};
  if(!borrow_area_transition_filename_level_v114(*levels,prepared_transition->source.definition.c_str(),
       original_row,original_record,error)||!original_record||original_row!=row||
     std::size_t(row)>=levels->level_names.size()||
     levels->level_names[std::size_t(row)]!=prepared_transition->source.identity){
   if(error.empty())error="Confirmed transition differs from original C1 LevelList selection";
   state->failed=true;state->failure=error;return false;
  }
 }
 auto held_design=std::make_shared<character::CharacterGameDesign::Borrow>(state->world->design->borrow());
 LevelConstructorApplicationV4 app;app.owner=state->world->files_owner;app.levels=held_design->levels();
 app.lua.owner=held_design;app.lua.design=*held_design->design();app.private_vm_limit=16u*1024u*1024u;
 const auto online=application->get_online_loading_v55();
 app.online_byte5=[online](std::uint8_t& out,std::string& e){out=online->byte5();e.clear();return true;};
 if(!bind_source_campaign_save_application_v86(state->world,app.saves,error)){
  state->failed=true;state->failure=error;return false;
 }
 app=state->world->level_application->bind(std::move(app));
 NativeRootGSBootstrapInputsV55 input;input.globals=state->globals;
 input.application=std::move(app);
 if(prepared_transition){
  // Projection already read Debug/Timer/Save once; do not select seeds again.
  input.request=prepared_transition->source;
  input.arguments=prepared_transition->gs;
 }else{
 input.request.identity=levels->level_names[std::size_t(row)];
 input.request.definition=levels->levels[std::size_t(row)].file;
 input.request.seed=std::uint32_t(state->profile->metadata.slot);
 const auto& location=state->profile->metadata.location;
 // Source __LoadLevelName468b78 writes LNAM seeds at Save+5c; LEPT is
 // Save+40. NativeStartGame passes LEPT as Application's signed argument2;
 // Application32c008 reads seed5c for BOTH GS unsigned24/28 cells.
 std::uint32_t source_seed=std::uint32_t(location.seeds[difficulty]);
 if(!source_seed)source_seed=std::uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(
   std::chrono::steady_clock::now().time_since_epoch()).count()); // Native Timer.getRealTime backend.
 input.arguments={input.request.definition,location.entry_points[difficulty],input.request.seed,
  source_seed,source_seed,1,location.use_spawn_point[difficulty],-1,state->profile->requested_difficulty};
 }
 input.online_state=[](std::uint32_t&,std::string& e){e="Required actual OnlineGameState34 producer";return false;};
 input.constructor.online_byte5=[online](std::uint8_t& out,std::string& e){out=online->byte5();e.clear();return true;};
 input.constructor.online_state34=[](std::int32_t&,std::string& e){e="Required actual OnlineGameState34 producer";return false;};
 input.constructor.flush_animation_sets=[camera=state->camera_application](std::string& e){
  if(!camera||!camera->ready()||!camera->animation_manager()){e="Required SAME initialized Application AnimSetManager";return false;}
  camera->animation_manager()->source_flush_v17();e.clear();return true;
 };
 auto* stable=state;const auto weak=std::weak_ptr<SourceWorldBorrowV61>(state->world);
 input.bind_loading_callbacks=[stable,weak,application](ui::LoadingMenuStateServicesV1 loading,
   GSLevelServicesV2<CanonicalLevelContextV1>& constructor,std::string& e){
  auto world=weak.lock();if(!world){e="Expired fresh source World before GS bootstrap";return false;}
  auto debug=[weak](const char* key,std::int32_t& out,std::string& e){
   if(!source_debug_load_v55(weak,e))return false;
   // The menu provider's NULL key denotes the original Load-only leaf.
   // It has no switch result; preserve its ignored output and SAME owner.
   if(!key)return true;
   bool value{};if(!source_debug_switch_v55(weak,key,value,e))return false;out=value;return true;
  };
  if(!borrow_native_gs_menu_v27(world->owner,loading,std::move(debug),constructor,e)||
     !bind_native_menu_application_ec_v58(world->owner,application,&application->source_loading_v55().byte_ec,e))return false;
  stable->menu=constructor;return true;
 };
 // Constructor services need menu registration before the synchronous GS C1
 // callback. NativeRootGSBootstrap transfers this same mutable service closure.
 input.connect_candidate=[stable](const auto& gs,const auto& level,auto& out,std::string& e){
  return connect_source_campaign_candidate_v55(*stable,gs,level,out,e);
 };
 // State was retained before the actual renderer startup prefix.
 if(!source_campaign_v55->bootstrap.begin(std::move(input),error)){
  source_campaign_v55->failed=true;source_campaign_v55->failure=error;return false;
 }
 auto& world=*source_campaign_v55->world;
 if(!source_campaign_v55->bootstrap.adopt_into(*world.native_gslevel_v27,*world.native_loading_v50,error)){
  source_campaign_v55->failed=true;source_campaign_v55->failure=error;return false;
 }
 *world.native_level_c1_v25=(*world.native_gslevel_v27)->connection()->level_connection();
 SourceCampaignCandidateBorrowV55 candidate;
 if(!borrow_source_campaign_candidate_runtime_v61(candidate,error)||
    !prepare_source_campaign_release_v108(candidate,error)||
    !bind_campaign_character_providers_v62(assets,candidate,error)||
    !prepare_source_campaign_character_preload_v81(candidate,error)||
    !prepare_source_campaign_script_start_v99(candidate,error)||
    !bind_source_campaign_early_gameplay_v107(candidate,error)){
  source_campaign_v55->failed=true;source_campaign_v55->failure=error;return false;
 }
 error.clear();return true;
}catch(const std::exception& failure){
 error=failure.what();
 if(source_campaign_v55){source_campaign_v55->failed=true;source_campaign_v55->failure=error;}
 return false;
}catch(...){
 error="Source campaign startup provider threw";
 if(source_campaign_v55){source_campaign_v55->failed=true;source_campaign_v55->failure=error;}
 return false;
}
bool source_campaign_runtime_active_v61(){return bool(source_campaign_v55);}
bool source_campaign_profile_loading_outcome_v114(
 const std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50>& profile,
 std::shared_ptr<void>& world,bool& complete,std::string& error){
 world.reset();complete=false;
 if(!profile||!source_campaign_v55||source_campaign_v55->profile!=profile||
    source_campaign_v55->profile.owner_before(profile)||profile.owner_before(source_campaign_v55->profile)){
  error="Actual prepared-profile startup absent or replaced";return false;
 }
 const auto& state=*source_campaign_v55;
 if(state.failed){error=state.failure.empty()?"Native prepared startup failed; prefix retained":state.failure;return false;}
 if(state.cancelling||state.cancelled||source_campaign_retirement_requested_v88()){
  error="Prepared-profile startup was cancelled or retired";return false;
 }
 if(state.world)world=state.world->owner;
 complete=source_campaign_loading_complete_runtime_v64();error.clear();return true;
}
bool source_campaign_loading_outcome_v114(const std::shared_ptr<void>& world,bool& complete,std::string& error){
 complete=false;
 if(!world||!source_campaign_v55||!source_campaign_v55->world||
    source_campaign_v55->world->owner!=world||
    source_campaign_v55->world->owner.owner_before(world)||world.owner_before(source_campaign_v55->world->owner)){
  error="Actual transition startup World absent or replaced";return false;
 }
 const auto& state=*source_campaign_v55;
 if(state.failed){error=state.failure.empty()?"Native source loading failed; prefix retained":state.failure;return false;}
 if(state.cancelling||state.cancelled||source_campaign_retirement_requested_v88()){
  error="Actual transition startup was cancelled or retired";return false;
 }
 complete=source_campaign_loading_complete_runtime_v64();error.clear();return true;
}
bool source_campaign_loading_complete_runtime_v64(){
 if(!source_campaign_v55||source_campaign_retirement_requested_v88()||source_campaign_v55->failed||source_campaign_v55->cancelling||source_campaign_v55->cancelled||
    !source_campaign_v55->final_tail_complete)return false;
 const auto& gs=source_campaign_v55->bootstrap.gs();
 return gs&&gs->fields().level34&&gs->fields().level34->constructor_fields_v3().field130==38&&
   source_campaign_v55->globals->s_level==gs->fields().level34;
}
bool capture_source_campaign_modules_runtime_v64(std::vector<dh2::loader::ModuleDrawFrameV1>& out,std::string& error){
 if(!source_campaign_loading_complete_runtime_v64()||!source_campaign_v55->candidate||
    !source_campaign_v55->candidate->preparation){
  error="Module submission requires genuine completed source loading/tail";return false;
 }
 return source_campaign_v55->candidate->preparation->capture_draw_frames(out,error);
}
bool bind_source_campaign_object_graph_v68(const SourceCampaignCandidateBorrowV55& expected,
 std::shared_ptr<dh2::world::CanonicalGameObjectGraphV68> graph,std::string& error){
 SourceCampaignCandidateBorrowV55 actual;
 if(!graph||!borrow_source_campaign_candidate_runtime_v61(actual,error))return false;
 if(actual.actual_world!=expected.actual_world||actual.level!=expected.level||actual.objects!=expected.objects||
    source_campaign_v55->failed||actual.level->constructor_fields_v3().field130>=7||
    source_campaign_v55->world->gameobject_graph_v68){
  error="Object graph must publish once on actual candidate before XML construction";return false;
 }
 source_campaign_v55->world->gameobject_graph_v68=std::move(graph);error.clear();return true;
}
bool capture_source_campaign_objects_v68(std::vector<std::shared_ptr<dh2::world::RetainedGameObjectVisualV1>>& out,std::string& error){
 if(!source_campaign_loading_complete_runtime_v64()||!source_campaign_v55->world->gameobject_graph_v68){
  error="Object scene submission requires actual completed campaign resource graph";return false;
 }
 return source_campaign_v55->world->gameobject_graph_v68->capture_visuals(out,error);
}
bool borrow_source_campaign_candidate_runtime_v61(SourceCampaignCandidateBorrowV55& out,std::string& error){
 if(!source_campaign_v55||!source_campaign_v55->world||!source_campaign_v55->candidate||
    !source_campaign_v55->application){error="Required actually produced source campaign graph";return false;}
 auto& state=*source_campaign_v55;const auto& candidate=state.candidate;
 const auto& gs=state.bootstrap.gs();
 if(!gs||!gs->connection()||!gs->connection()->level_connection()->complete()||
    !candidate->preparation||candidate->preparation->level()!=gs->fields().level34||
    state.globals->s_level!=gs->fields().level34){
  error="Required SAME completed GS/C1/global/candidate lifetime";return false;
 }
 out={state.world->owner,state.world->application,gs->fields().level34,
   candidate->canonical->manager_lease,
   &candidate->canonical->properties,state.profile,candidate->preparation->floor_world(),
   candidate->canonical->scene_roots_v20,state.world->physical_world,state.world->application->source_fx_libraries_v63(),
   candidate->navigation,state.camera_application,
   candidate->rooms,candidate->rooms->map_owner_v69(),candidate->transport,
   state.world->application->source_script_manager_v52()};
 error.clear();return true;
}
bool borrow_source_campaign_condition_world_v70(const SourceCampaignCandidateBorrowV55& candidate,
 std::shared_ptr<SourceWorldBorrowV61>& out,std::string& error){
 SourceCampaignCandidateBorrowV55 actual;
 if(!borrow_source_campaign_candidate_runtime_v61(actual,error))return false;
 const auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
 if(!same(candidate.actual_world,actual.actual_world)||!same(candidate.application,actual.application)||
    !same(candidate.level,actual.level)||!same(candidate.objects,actual.objects)||candidate.properties!=actual.properties){
  error="Condition composition requires SAME retained source campaign World/App/Level/manager/property map";return false;
 }
 auto world=source_campaign_v55->world;
 if(!world||!same(world->owner,actual.actual_world)||world->application!=actual.application||
    !world->player_manager||world->player_manager!=world->application->source_player_manager_v59()){
  error="Condition composition lost actual source World or retained Application PlayerManager";return false;
 }
 out=std::move(world);error.clear();return true;
}
#include "source_campaign_module_conditions_v75.inc"
bool bind_source_campaign_class_factory_runtime_v61(SourceCampaignClassFactoryV60 factory,std::string& error){
 SourceCampaignCandidateBorrowV55 actual;
 if(!factory||!borrow_source_campaign_candidate_runtime_v61(actual,error)){
  if(error.empty())error="Required real canonical source class factory";return false;
 }
 auto& state=*source_campaign_v55;
 const auto& fields=actual.level->constructor_fields_v3();
 if(state.failed||fields.field130>=7||!state.candidate->factory||state.candidate->factory->remaining){
  error="Source class factory must bind once before XML construction; failed/loading prefix cannot be replayed";return false;
 }
 state.candidate->factory->remaining=std::move(factory);error.clear();return true;
}
bool bind_source_campaign_script_init_runtime_v62(const std::shared_ptr<void>& world,
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& application,
 std::function<bool(const dh2::loader::CheckedCommandBorrowV59&,std::string&)> init,std::string& error){
 SourceCampaignCandidateBorrowV55 actual;
 if(!init||!borrow_source_campaign_candidate_runtime_v61(actual,error)){
  if(error.empty())error="Required real canonical script Init leaf";return false;
 }
 auto& state=*source_campaign_v55;
 if(state.failed||actual.actual_world.get()!=world.get()||actual.actual_world.owner_before(world)||world.owner_before(actual.actual_world)||
    actual.application!=application||actual.application.owner_before(application)||application.owner_before(actual.application)||
    actual.level->constructor_fields_v3().field130>=8||state.world->script_command_init){
  error="Script Init leaf requires SAME live campaign owner and sole binding before source stage8";return false;
 }
 state.world->script_command_init=std::move(init);error.clear();return true;
}
bool bind_source_campaign_gameplay_runtime_v64(const std::shared_ptr<void>& world,
 const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& level,
 std::function<bool(const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>&,bool,std::string&)> update,std::string& error){
 SourceCampaignCandidateBorrowV55 actual;
 if(!update||!source_campaign_loading_complete_runtime_v64()||!borrow_source_campaign_candidate_runtime_v61(actual,error)){
  if(error.empty())error="Required genuine completed source loading and actual gameplay producer";return false;
 }
 if(world.get()!=actual.actual_world.get()||world.owner_before(actual.actual_world)||actual.actual_world.owner_before(world)||
    level!=actual.level||level.owner_before(actual.level)||actual.level.owner_before(level)||source_campaign_v55->world->gameplay_update){
  error="Gameplay producer requires sole binding to SAME actual completed World/Level";return false;
 }
 source_campaign_v55->world->gameplay_update=std::move(update);error.clear();return true;
}
bool bind_source_campaign_post_init_runtime_v66(const std::shared_ptr<void>& world,
 const std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59>& pm,
 std::function<bool(std::string&)> body,std::string& error){
 SourceCampaignCandidateBorrowV55 actual;
 if(!body||!pm||!borrow_source_campaign_candidate_runtime_v61(actual,error)){
  if(error.empty())error="Required actual PM.PostInitCharacters body";return false;
 }
 auto& state=*source_campaign_v55;
 if(state.failed||world.get()!=actual.actual_world.get()||world.owner_before(actual.actual_world)||actual.actual_world.owner_before(world)||
    pm!=state.world->player_manager||pm.owner_before(state.world->player_manager)||state.world->player_manager.owner_before(pm)||
    actual.level->constructor_fields_v3().field130>=26||state.world->post_init_characters){
  error="PostInitCharacters requires SAME PM/World and sole binding before actual source stage26";return false;
 }
 state.world->post_init_characters=std::move(body);error.clear();return true;
}
bool bind_source_campaign_post_init_v66(const std::shared_ptr<void>& world,
 const std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59>& pm,
 std::function<bool(std::string&)> body,std::string& error){
 return bind_source_campaign_post_init_runtime_v66(world,pm,std::move(body),error);
}
bool bind_source_campaign_post_init_services_v66(const SourceCampaignCandidateBorrowV55& candidate,
 dh2::player::PlayerManagerPostInitServicesV66 services,std::string& error){
 SourceCampaignCandidateBorrowV55 actual;
 if(!borrow_source_campaign_candidate_runtime_v61(actual,error))return false;
 if(candidate.actual_world!=actual.actual_world||candidate.actual_world.owner_before(actual.actual_world)||
    actual.actual_world.owner_before(candidate.actual_world)||candidate.application!=actual.application||
    candidate.application.owner_before(actual.application)||actual.application.owner_before(candidate.application)||!services.provider){
  error="PostInit services require SAME actual campaign World/App and producer";return false;
 }
 auto pm=actual.application->source_player_manager_v59();
 services.active5c=[pm,app=std::weak_ptr<dh2::application::ApplicationServicesOwnerV5>(actual.application)](
   dh2::player::PlayerInfoFieldsV1& player,bool& value,std::string& e){
  auto application=app.lock();
  if(!application||!pm||!pm->belongs_to_application(application)||!pm->network()){
   e="Required SAME App/PM/CNet for selected PlayerInfo.IsActive";return false;
  }
  auto online=application->get_online_loading_v55();
  if(!online){e="Required actual GetOnline before PlayerInfo.IsActive";return false;}
  return pm->network()->is_active_selected_v67(player,online->byte5(),value,e);
 };
 auto body=[pm,app=std::weak_ptr<dh2::application::ApplicationServicesOwnerV5>(actual.application),services=std::move(services)](std::string& e){
  auto application=app.lock();
  if(!application||!pm||!pm->belongs_to_application(application)){e="Actual PM/PostInit Application expired or replaced";return false;}
  return dh2::player::source_post_init_characters_v66(*pm,*application,services,e);
 };
 return bind_source_campaign_post_init_runtime_v66(actual.actual_world,pm,std::move(body),error);
}
bool advance_source_scene_v69(std::string& error){
 SourceCampaignCandidateBorrowV55 actual;
 if(!source_campaign_loading_complete_runtime_v64()||!borrow_source_campaign_candidate_runtime_v61(actual,error)){
  if(error.empty())error="Required genuine source loading completion before actual scene phase";return false;
 }
 dh2::loader::CanonicalCurrentLevelBorrowV1 current;
 if(!actual.roots||!borrow_current_native_level_v27(current,error)||current.level()!=actual.level){
  if(error.empty())error="Actual Scene phase addressed another current Level";return false;
 }
 std::uint32_t timer{};std::uint64_t epoch{};
 if(!borrow_application_time_v68(timer,error)||!borrow_application_frame_v69(epoch,error))return false;
 return actual.roots->source_scene_phase_v69(epoch,timer,error)&&source_campaign_fx_scene_v77(actual,epoch,timer,error);
}
bool register_source_scene_nodes_v69(bool native_force,std::string& error){
 SourceCampaignCandidateBorrowV55 actual;
 if(!source_campaign_loading_complete_runtime_v64()||!borrow_source_campaign_candidate_runtime_v61(actual,error)){
  if(error.empty())error="Required true38 source Scene registration owner";return false;
 }
 dh2::loader::CanonicalCurrentLevelBorrowV1 current;
 if(!actual.roots||!borrow_current_native_level_v27(current,error)||current.level()!=actual.level){
  if(error.empty())error="Source registration requires SAME current Scene/Level";return false;
 }
 const auto scripts=actual.application->source_script_manager_v52();
 if(!scripts||scripts->diagnostics().failed){error="Required actual App-owned ScriptManager byte30 producer";return false;}
 dh2::world::SceneRegistrationTransportV69 transport;
 transport.provider=actual.roots;
 transport.clear_render_lists=clear_source_scene_drawlist_v69;
 transport.register_nodes=rebuild_source_scene_drawlist_v69;
 transport.refresh_cached_nodes=refresh_source_scene_cached_drawlist_v69;
 return actual.roots->source_register_nodes_v69(native_force,scripts->fields().byte30,transport,error);
}
bool enable_source_campaign_fog_v68(const SourceCampaignCandidateBorrowV55& candidate,std::string& error){
 SourceCampaignCandidateBorrowV55 actual;if(!borrow_source_campaign_candidate_runtime_v61(actual,error))return false;
 if(!actual.roots||candidate.actual_world!=actual.actual_world||candidate.level!=actual.level||candidate.roots!=actual.roots){
  error="EnableFog addressed another actual source Level/SceneManager";return false;
 }
 auto fields=actual.level->config_fields();
 const auto* config=fields.config38?source_campaign_v55->candidate->factory->modules->level_config(*fields.config38):nullptr;
 const auto* start=config?config->integer(0x1d8):nullptr;
 const auto* end=config?config->integer(0x1dc):nullptr;
 const auto* color=config?config->vector(0x1e0):nullptr;
 if(!start||!end||!color){error="Original Level.EnableFog requires actual LevelConfig38 fog fields";return false;}
 actual.roots->source_enable_fog_v68(static_cast<float>(*start),static_cast<float>(*end),*color);
 error.clear();return true;
}
bool disable_source_campaign_fog_v68(const SourceCampaignCandidateBorrowV55& candidate,std::string& error){
 SourceCampaignCandidateBorrowV55 actual;if(!borrow_source_campaign_candidate_runtime_v61(actual,error))return false;
 if(!actual.roots||candidate.actual_world!=actual.actual_world||candidate.level!=actual.level||candidate.roots!=actual.roots){
  error="DisableFog addressed another actual source Level/SceneManager";return false;
 }
 actual.roots->source_disable_fog_v67();error.clear();return true;
}
bool bind_source_player_manager_update_v70(const SourceCampaignCandidateBorrowV55& candidate,
 const std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59>& pm,
 std::shared_ptr<void> provider,std::function<bool(std::string&)> update,std::string& error){
 SourceCampaignCandidateBorrowV55 actual;if(!borrow_source_campaign_candidate_runtime_v61(actual,error))return false;
 const auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
 auto& world=*source_campaign_v55->world;
 if(!same(candidate.actual_world,actual.actual_world)||!same(candidate.level,actual.level)||
    !same(pm,world.player_manager)||!provider||!update||world.source_pm_update_v70||
    actual.level->constructor_fields_v3().field130>6){
  error="PM.Update requires one actual SAME App/PM producer before Stage6";return false;
 }
 world.source_pm_update_provider_v70=std::move(provider);world.source_pm_update_v70=std::move(update);error.clear();return true;
}
bool bind_source_campaign_player_placement_v68(const SourceCampaignCandidateBorrowV55& candidate,
 dh2::loader::LevelPlayerPlacementServicesV68 services,std::string& error){
 SourceCampaignCandidateBorrowV55 actual;
 if(!borrow_source_campaign_candidate_runtime_v61(actual,error))return false;
 auto& state=*source_campaign_v55;
 const auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
 if(state.failed||!same(candidate.actual_world,actual.actual_world)||!same(candidate.level,actual.level)||
    !same(candidate.objects,actual.objects)||state.world->player_placement_v68||
    actual.level->constructor_fields_v3().field130>14||!services.provider){
  error="Placement services require SAME source World/Level/manager and one producer before stage14";return false;
 }
 state.world->player_placement_v68=std::make_shared<dh2::loader::LevelPlayerPlacementServicesV68>(std::move(services));
 error.clear();return true;
}
bool bind_source_campaign_gameplay_services_v66(const SourceCampaignCandidateBorrowV55& candidate,
 dh2::loader::LevelGameplayServicesV66 services,std::string& error){
 SourceCampaignCandidateBorrowV55 actual;
 if(!borrow_source_campaign_candidate_runtime_v61(actual,error))return false;
 auto& state=*source_campaign_v55;
 const auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
 if(state.failed||!same(candidate.actual_world,actual.actual_world)||!same(candidate.level,actual.level)||
    !same(candidate.application,actual.application)||!same(candidate.objects,actual.objects)||
    !same(services.actual_application,actual.application)||!same(services.actual_object_manager,actual.objects)||
    !services.provider||state.world->gameplay_services_v66){
  error="Gameplay leaves require sole binding to SAME actual source Level/App/ObjectManager";return false;
 }
 // The real frame source is App8c. This native physics adapter implements the
 // recovered PhysicalWorld.update Step(dt*float0.001,10) on its SAME backend.
 const auto weak=std::weak_ptr<SourceWorldBorrowV61>(state.world);
 services.debug_load=[weak](std::string& e){return source_debug_load_v55(weak,e);};
 services.debug_switch=[weak](const char* name,bool& value,std::string& e){return source_debug_switch_v55(weak,name,value,e);};
 services.local_character=[weak](std::uintptr_t& value,std::string& e){
  auto world=weak.lock();dh2::player::PlayerInfoFieldsV1* info{};
  if(!world||!world->player_manager||!world->player_manager->get_local_player(0,true,info,e))return false;
  if(!info){e="Source GetLocalPlayer returned NULL before Character660 dereference";return false;}
  value=info->character660;e.clear();return true;
 };
 services.player_manager714=[weak](std::uintptr_t& value,std::string& e){
  auto world=weak.lock();auto* manager=world&&world->player_manager?world->player_manager->manager():nullptr;
  const auto* fields=manager?manager->source_frame_fields_v68():nullptr;
  if(!fields){e="Required actual completed PM C1 before field714 read";return false;}
  value=fields->field714;e.clear();return true;
 };
 services.player_manager_update=[weak](std::string& e){
  auto world=weak.lock();if(!world||!world->source_pm_update_provider_v70||!world->source_pm_update_v70){
   e="Required SAME pre-player and gameplay PlayerManager.Update owner";return false;
  }
  return world->source_pm_update_v70(e);
 };
 // Enrolled at Stage26 by UI/actor V96, then executed at the original
 // Level.Update phase BEFORE events/physics/objects. Same parsed context
 // arrays own command index/blocking state; no second script tick/clock.
 services.start_script=[weak](std::int32_t id,std::int32_t module,bool received,std::string& e){
  auto world=weak.lock();auto scripts=world&&world->application?world->application->source_script_manager_v52():nullptr;
  if(!scripts){e="Required SAME Application ScriptManager.StartScript";return false;}
  return scripts->start_script_v96(id,module,received,e);
 };
 services.execute_all_scripts=[weak](std::string& e){
  auto world=weak.lock();auto scripts=world&&world->application?world->application->source_script_manager_v52():nullptr;
  if(!scripts){e="Required SAME Application ScriptManager.ExecuteAllScripts";return false;}
  bool any_running{};return scripts->execute_all_scripts_v96(any_running,e);
 };
 services.in_game_view=[](bool& value,std::string& e){
  dh2::loader::CanonicalCurrentLevelBorrowV1 first;
  if(!borrow_current_native_level_v27(first,e))return false;
  if(!first){value=false;e.clear();return true;}
  // Original Application.IsCurrentlyInGameView repeats GetCurrentLevel,
  // then returns the nonzero current Level198 byte.
  dh2::loader::CanonicalCurrentLevelBorrowV1 second;
  if(!borrow_current_native_level_v27(second,e)||!second){if(e.empty())e="Source second current-Level became NULL";return false;}
  value=second.level()->constructor_fields_v3().byte198!=0;e.clear();return true;
 };
 services.game_events_update=[weak](std::string& e){
  auto world=weak.lock();if(!world||!world->game_events_v75){e="Required SAME actual GameEvent194 Compile/Register owner";return false;}
  return world->game_events_v75->update(e);
 };
 services.update_fx=[weak](std::string& e){
  auto world=weak.lock();SourceCampaignCandidateBorrowV55 actual;
  if(!world||!borrow_source_campaign_candidate_runtime_v61(actual,e)||actual.actual_world!=world->owner){
   if(e.empty())e="Required SAME actual source campaign FX phase";return false;
  }
  return update_source_campaign_fx_v77(actual,e);
 };
 services.physical_update=[weak](std::string& e){
  auto world=weak.lock();if(!world||!world->physical_world||!world->application||
     !world->application->source_loading_v55().native_dt_produced_v93){e="Required SAME loaded physics and produced App8c";return false;}
  world->physical_world->update(world->application->source_loading_v55().dt8c);e.clear();return true;
 };
 const auto candidate_weak=std::weak_ptr<RendererSourceCandidateV55>(state.candidate);
 const auto level_weak=std::weak_ptr<dh2::loader::CanonicalLevelContextV1>(actual.level);
 const auto roots=actual.roots;
 const auto config=[candidate_weak,level_weak](std::string& e)->const dh2::world::CanonicalLevelConfigV1*{
  auto candidate=candidate_weak.lock();auto level=level_weak.lock();
  if(!candidate||!level||!candidate->factory){e="Released actual campaign/config provider";return nullptr;}
  const auto config_fields=level->config_fields();
  const auto id=config_fields.config38?*config_fields.config38:0;
  auto* value=candidate->factory->modules->level_config(id);
  if(!id||!value){e="Required actual published LevelConfig38";return nullptr;}
  return value;
 };
 services.prepare_debug_hud=[weak](std::string& e){
  auto world=weak.lock();if(!world){e="Released actual DebugHUD World";return false;}
  return prepare_actual_gameplay_debug_v67(world->owner,e);
 };
 // Audio's composer may replace this leaf with its identical actual Scene
 // setter. Both setters share source104; there is no second ambient snapshot.
 services.set_ambient=[config,roots](std::string& e){
  const auto* value=config(e);const auto* rgb=value?value->vector(0x1cc):nullptr;
  if(!rgb||!roots){if(e.empty())e="Required actual LevelConfig1cc/Scene104";return false;}
  roots->source_set_ambient_v67({(*rgb)[0],(*rgb)[1],(*rgb)[2],1.f});e.clear();return true;
 };
 services.update_zoom=[weak](std::string& e){auto world=weak.lock();
  if(!world||!world->camera_application){e="Required actual Level camera zoom";return false;}
  return world->camera_application->source_update_zoom_v67(e);
 };
 services.camera_override_global=[weak](std::uintptr_t& value,std::string& e){auto world=weak.lock();
  if(!world||!world->application){e="Required SAME actual camera global";return false;}
  value=world->application->active_camera().source_active();e.clear();return true;
 };
 services.camera_override_update=[weak](std::string& e){auto world=weak.lock();
  auto session=world&&world->camera_application?world->camera_application->world():nullptr;
  if(!session||!session->camera||!session->camera->overview()){e="Required actual source CameraOverview12c";return false;}
  return session->camera->overview()->update(e);
 };
 services.camera_update=[weak](std::string& e){auto world=weak.lock();
  if(!world||!world->camera_application){e="Required SAME actual Camera128";return false;}
  return world->camera_application->source_update_level_v67(e);
 };
 services.camera_node_update_false=[weak](std::string& e){auto world=weak.lock();
  if(!world||!world->camera_application){e="Required SAME actual Camera.Node8";return false;}
  // Selected CCamera vptr+28 B8 is updateAbsolutePosition(false),597c60.
  return world->camera_application->source_update_absolute_v67(e);
 };
 services.update_camera_fog=[weak,level_weak,config,roots](std::string& e){
  auto world=weak.lock();auto level=level_weak.lock();const auto* settings=config(e);
  if(!world||!level||!settings||!roots||!world->camera_application)return false;
  const auto* start=settings->integer(0x1d8);const auto* end=settings->integer(0x1dc);
  if(!start||!end){e="Required produced LevelConfig fog1d8/1dc";return false;}
  dh2::camera::PointV2 eye,parent;if(!world->camera_application->source_positions_v67(eye,parent,e))return false;
  const auto& fields=level->constructor_fields_v3();
  const float offsets[]={fields.field19c,fields.field1a0,fields.field1a4};
  float relative[3],scaled[3];
  for(unsigned i=0;i<3;++i){
   relative[i]=(eye[i]-parent[i])+offsets[i];scaled[i]=relative[i]*roots->frame_fields_v67().fog_transform458[i];
  }
  const float x=scaled[0]*scaled[0],y=scaled[1]*scaled[1],z=scaled[2]*scaled[2];
  const float sum=(x+y)+z;float distance=static_cast<float>(std::sqrt(static_cast<double>(sum)));
  if(relative[2]<0.f)distance=-distance;
  roots->source_update_fog_v67(static_cast<float>(*start)+distance,static_cast<float>(*end)+distance);
  e.clear();return true;
 };
 services.update_render_flags=[weak,roots](std::string& e){
  if(!roots){e="Required SAME source SceneManager render flags";return false;}
  auto& fields=roots->source_frame_fields_v67();
  const auto update=[&](const char* name,std::uint8_t& field){
   bool disabled{};
   if(!source_debug_load_v55(weak,e)||!source_debug_switch_v55(weak,name,disabled,e))return false;
   if(field==static_cast<std::uint8_t>(disabled)){
    // The original repeats GetInstance/GetSwitch before the reached store.
    if(!source_debug_switch_v55(weak,name,disabled,e))return false;
    field=static_cast<std::uint8_t>(!disabled);++fields.parameter_revision;
   }
   return true;
  };
  if(!update("RENDERING_DisableAllLighting",fields.lighting430)||
     !update("RENDERING_DisableAllSpecular",fields.specular431))return false;
  e.clear();return true;
 };
 services.update_dynamic_fog=[weak,candidate_weak,level_weak,roots](std::string& e){
  auto world=weak.lock();auto candidate=candidate_weak.lock();auto level=level_weak.lock();
  if(!world||!candidate||!level||!roots){e="Required SAME actual dynamic-fog owners";return false;}
  const auto config_fields=level->config_fields();
  const auto id=config_fields.config38?*config_fields.config38:0;
  if(!id){e.clear();return true;} // original NULL LevelConfig branch
  const auto* settings=candidate->factory->modules->level_config(id);
  if(!settings){e="Unregistered actual LevelConfig38";return false;}
  if(settings->dfog_colors().empty()){e.clear();return true;}
  const auto* range=settings->scalar_float_v55(0x214);
  if(!range||!world->camera_application){e="Required produced dynamic-fog distance214/camera";return false;}
  dh2::camera::PointV2 eye,parent;if(!world->camera_application->source_positions_v67(eye,parent,e))return false;
  std::vector<dh2::world::ModuleFogBorrowV67> modules;modules.reserve(candidate->canonical->manager.modules().size());
  for(const auto module_id:candidate->canonical->manager.modules()){
   auto* record=candidate->factory->modules->module(module_id);
   if(!record||!record->receiver){e="Actual Module68 list has no retained fog receiver";return false;}
   modules.push_back({module_id,record->receiver->base().vector3(0x160),&record->receiver->source_fog_color_v67()});
  }
  std::array<float,3> color;
  if(!dh2::world::source_dynamic_module_fog_v67(modules,eye.data(),*range,color,e))return false;
  std::array<std::uint8_t,4> bytes{0,0,0,0};
  for(unsigned i=0;i<3;++i){
   bytes[i]=dh2::world::source_fog_color_byte_v68(color[i]);
  }
  roots->source_frame_fields_v67().fog_color=bytes;++roots->source_frame_fields_v67().parameter_revision;
  auto fields=const_cast<dh2::world::CanonicalLevelConfigV1*>(settings)->properties().fields;
  if(!fields.write_vector3){e="Required actual LevelConfig1ec vector writer";return false;}
  return fields.write_vector3(fields.context,0x1ec,color,e);
 };
 dh2::audio::AudioCampaignServicesV46 campaign_audio;
 dh2::audio::AudioLevelBindingsV67 audio_bindings;
 audio_bindings.camera=actual.camera_application;audio_bindings.roots=actual.roots;
 audio_bindings.config=[candidate_weak](std::uintptr_t id)->const dh2::world::CanonicalLevelConfigV1*{
  auto candidate=candidate_weak.lock();return candidate&&candidate->factory?candidate->factory->modules->level_config(id):nullptr;
 };
 audio_bindings.local_character0=services.local_character;
 audio_bindings.target_position=[weak](std::uintptr_t id,dh2::camera::PointV2& position,std::string& e){
  auto world=weak.lock();SourceCampaignCameraActorBorrowV67 actor;
  if(!world||!borrow_source_campaign_camera_actor_v67(world->owner,id,actor,e)||!actor.position160)return false;
  for(unsigned i=0;i<3;++i)position[i]=actor.position160[i];e.clear();return true;
 };
 audio_bindings.driver=[weak](dh2::audio::AudioListenerDriverV67& driver,std::string& e){
  auto world=weak.lock();std::int32_t width{},height{};
  if(!world||!world->camera_application||!borrow_actual_camera_viewport_v20(width,height,e))return false;
  driver.picking.viewport_width=driver.picking.render_width=width;
  driver.picking.viewport_height=driver.picking.render_height=height;
  driver.picking.scene_manager_present=driver.picking.camera_present=true;
  driver.screen_width=width;driver.screen_height=height;driver.viewport_bottom=height;
  e.clear();return true;
 };
 audio_bindings.look_at=[weak](std::uintptr_t id,dh2::camera::PointV2& out,std::string& e){
  auto world=weak.lock();if(!world){e="Released actual listener Character World";return false;}
  return source_campaign_character_look_at_v68(world->owner,id,out,e);
 };
 audio_bindings.visual_up=[weak](std::uintptr_t id,dh2::camera::PointV2& out,std::string& e){
  auto world=weak.lock();if(!world){e="Released actual listener visual World";return false;}
  return source_campaign_character_visual_up_v68(world->owner,id,out,e);
 };
 if(!borrow_campaign_audio_services_v68(actual,campaign_audio,audio_bindings,error))return false;
 std::shared_ptr<dh2::audio::AudioLevelGameplayV67> audio_owner;
 if(!compose_campaign_level_audio_v67(std::move(campaign_audio),std::move(audio_bindings),services,audio_owner,error))return false;
 // The three source audio callbacks capture this same owner. No extra
 // SoundManager or state observer is created and no source144 byte is set.
 state.world->gameplay_services_v66=std::make_shared<dh2::loader::LevelGameplayServicesV66>(std::move(services));
 error.clear();return true;
}
bool borrow_source_campaign_gameplay_v64(const SourceCampaignCandidateBorrowV55& candidate,
 std::function<bool(const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>&,bool,std::string&)>& out,std::string& error){
 SourceCampaignCandidateBorrowV55 actual;
 if(!source_campaign_loading_complete_runtime_v64()||!borrow_source_campaign_candidate_runtime_v61(actual,error)){
  if(error.empty())error="Required genuinely completed source loading before gameplay borrow";return false;
 }
 const auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
 if(!same(candidate.actual_world,actual.actual_world)||!same(candidate.level,actual.level)||!same(candidate.objects,actual.objects)){
  error="Gameplay borrow addressed another actual candidate";return false;
 }
 auto services=source_campaign_v55->world->gameplay_services_v66;
 if(!services){error="Required actual canonical Level.Update actor/Script/Event/Scene/audio leaves";return false;}
 // Native activation admission, not an original readiness field or source
 // callback. Do not hand the renderer a seemingly usable partial updater.
 const std::pair<bool,const char*> mandatory[]={
  {bool(services->prepare_debug_hud),"actual MenuDebugHUD counters"},
  {bool(services->save_update),"Character.SG_Update"},
  {bool(services->player_manager_update),"PlayerManager.Update"},
  {bool(services->player_manager714),"actual PM714"},
  {bool(services->execute_all_scripts),"ScriptManager.ExecuteAllScripts"},
  {bool(services->game_events_update),"GameEventManager.Update"},
  {bool(services->set_ambient),"Scene ambient"},
  {bool(services->update_spawn_groups),"SpawnGroupManager.update"},
  {bool(services->handle_ai_groups),"CharAI.HandleGroups"},
  {bool(services->update_objects),"whole canonical ObjectManager.Update"},
  {bool(services->inc_ai_queue),"CharAI.IncUpdateQueue"},
  {bool(services->update_zoom),"Level.UpdateCameraZoom"},
  {bool(services->update_fx),"VisualFXManager.Update"},
  {bool(services->camera_update),"Camera128.Update"},
  {bool(services->camera_node_update_false),"CameraNode.updateAbsolutePosition(false)"},
  {bool(services->update_camera_fog),"camera fog"},
  {bool(services->update_render_flags),"source scene lighting/specular flags"},
  {bool(services->start_level_sound),"Level sound suffix"},
  {bool(services->update_listener),"Level.UpdateListener"},
  {bool(services->update_dynamic_fog),"Level.UpdateDynamicFog"}
 };
 for(const auto& leaf:mandatory)if(!leaf.first){error=std::string("Required before native gameplay activation: ")+leaf.second;return false;}
 // No calls are replayed here and no phase/readiness fields are set. The
 // retained coordinator owns the original branch/order and failure prefix.
 auto& retained=source_campaign_v55->world->gameplay_owner_v66;
 if(!retained)retained=std::make_shared<dh2::loader::LevelGameplayUpdateV66>(actual.level,*services);
 auto updater=retained;
 out=[updater](const auto& receiver,bool force,std::string& e){return updater->update(receiver,force,e);};
 error.clear();return true;
}
bool tick_source_campaign_runtime_v61(std::string& error){
 if(!source_campaign_v55){error="Required retained source campaign";return false;}
 auto& state=*source_campaign_v55;
 if(source_campaign_retirement_requested_v88()){
  if(state.retirement_phase_v88==RendererSourceCampaignV55::RetirementPhaseV88::failed){error=state.retirement_failure_v88;return false;}
  error.clear();return true; //Root drains native retirement instead of a source frame.
 }
 if(state.cancelling){
  if(!state.world||!state.world->native_loading_v50||!*state.world->native_loading_v50){
   error="Required actual pinned loading owner for source cancellation";return false;
  }
  const auto status=(*state.world->native_loading_v50)->drain_cancel(error);
  if(status==dh2::loader::LifecycleStatusV36::failed)return false;
  if(status==dh2::loader::LifecycleStatusV36::cancelled){state.cancelling=false;state.cancelled=true;}
  error.clear();return true;
 }
 if(state.cancelled){error="Source cleanup drained; original GS/Level destruction remains required";return false;}
 if(state.failed){error=state.failure;return false;}
 if(!state.world||(!state.world->native_loading_v50||!*state.world->native_loading_v50)){error="Required SAME actual GS loading connection";return false;}
 SourceCampaignDeliveryV104 delivery;bool blocked{};
 if(!delivery.acquire(state.world->admission_v104,SourceCampaignDeliveryKindV104::update,blocked,error))return false;
 if(blocked){error.clear();return true;}
 // NativeBridge publishes the actual Application+8c once before dispatching
 // this frame. Loading, HUD and combat consume that SAME unsigned source dt;
 // this connection must not introduce a second Timer subtraction.
 std::uint32_t dt{};
 if(!model_renderer::borrow_application_dt_v93(dt,error))return false;
 *state.world->application_dt=dt;*state.world->application_tick=dt<=2000;
 if(!*state.world->application_tick){error.clear();return true;} // Original Application long-gap skip, no clamp.
 const auto result=(*state.world->native_loading_v50)->tick();
 if(result==dh2::loader::GSLevelUpdateResultV44::failed){
  state.failed=true;state.failure=(*state.world->native_loading_v50)->error();
  //Describe the actual retained failure point. The loading bar alone cannot
  //prove which earlier consumers ran or distinguish failures within one stage.
  const auto& level=(*state.world->native_loading_v50)->level();
  if(level)state.failure="Stage "+std::to_string(level->constructor_fields_v3().field130)+": "+state.failure;
  error=state.failure;return false;
 }
 error.clear();return true;
}
bool request_source_campaign_cancel_runtime_v61(std::string& error){
 if(!source_campaign_v55||!source_campaign_v55->world||!source_campaign_v55->world->native_loading_v50||
    !*source_campaign_v55->world->native_loading_v50){
  error="Required actually attached source loading owner; startup abort remains explicit";return false;
 }
 auto& state=*source_campaign_v55;
 if(state.cancelled){error="Source cleanup already drained; require original GS destruction";return false;}
 (*state.world->native_loading_v50)->request_cancel();state.cancelling=true;error.clear();return true;
}

bool source_campaign_retirement_requested_v88()noexcept{
 return source_campaign_v55&&source_campaign_v55->retirement_phase_v88!=RendererSourceCampaignV55::RetirementPhaseV88::idle;
}
bool begin_source_campaign_retirement_v88(SourceCampaignRetirementServicesV88 services,std::string& e){
 if(!source_campaign_v55||!source_campaign_v55->world||!source_campaign_v55->world->release_v88||
    !services.owner||!services.quiesce||!services.retire_world){e="Required retained GS release journals and genuine native retirement providers";return false;}
 auto& state=*source_campaign_v55;
 if(state.retirement_phase_v88!=RendererSourceCampaignV55::RetirementPhaseV88::idle){e="Campaign retirement already requested; drain the same retained request";return false;}
 const auto& w=state.world;
 for(const auto& enclosing:{w->owner,std::shared_ptr<void>(w),std::shared_ptr<void>(w->application)}){
  if(!services.owner.owner_before(enclosing)&&!enclosing.owner_before(services.owner)){e="Retirement provider must not retain its enclosing World/Application";return false;}
 }
 // Stop new queued input/draw deliveries immediately at request acceptance.
 // Existing scopes remain pinned; the drain observes them before cancellation.
 bool pending{};
 if(!w->admission_v104||!w->admission_v104->close(pending,e))return false;
 state.retirement_v88=std::move(services);state.retirement_phase_v88=RendererSourceCampaignV55::RetirementPhaseV88::admit;
 e.clear();return true;
}
bool retry_source_campaign_retirement_v114(std::string& e){
 using Phase=RendererSourceCampaignV55::RetirementPhaseV88;
 if(!source_campaign_v55||source_campaign_v55->retirement_busy_v88||
    source_campaign_v55->retirement_phase_v88!=Phase::failed||
    source_campaign_v55->retirement_resume_v114==Phase::idle||
    source_campaign_v55->retirement_resume_v114==Phase::failed){
  e="Require failed SAME retained campaign retirement continuation";return false;
 }
 auto& state=*source_campaign_v55;
 state.retirement_phase_v88=state.retirement_resume_v114;
 state.retirement_failure_v88.clear();e.clear();return true;
}
bool borrow_source_campaign_startup_prefix_v114(SourceCampaignStartupPrefixV114& out,std::string& e){
 if(!source_campaign_v55||!source_campaign_v55->failed){e="Require actually retained failed source startup";return false;}
 auto& state=*source_campaign_v55;SourceCampaignStartupPrefixV114 actual;
 actual.world=state.world;actual.selected=state.profile;actual.globals=state.globals;actual.gs=state.bootstrap.gs();
 if(actual.gs){actual.gs_connection=actual.gs->connection().get();actual.published_level34=actual.gs->fields().level34;
  if(actual.gs_connection){actual.level_connection=actual.gs_connection->level_connection();
   if(actual.level_connection){actual.allocated_level=actual.level_connection->candidate();actual.bindings=actual.level_connection->bindings();actual.level_c1_complete=actual.level_connection->complete();}}
 }
 actual.has_loading=bool(state.bootstrap.loading());
 if(state.world&&state.world->native_loading_v50&&*state.world->native_loading_v50)actual.has_loading=true;
 out=std::move(actual);e.clear();return true;
}
bool borrow_source_campaign_startup_candidate_v114(const SourceCampaignStartupPrefixV114& prefix,
 SourceCampaignCandidateBorrowV55& out,std::string& e){
 out={};
 if(!source_campaign_v55||!source_campaign_v55->failed||source_campaign_v55->world!=prefix.world||
    source_campaign_v55->bootstrap.gs()!=prefix.gs||source_campaign_v55->globals!=prefix.globals){
  e="Startup candidate loan addressed a replaced failed prefix";return false;
 }
 auto& state=*source_campaign_v55;auto w=prefix.world;
 if(!w){e.clear();return true;} // No World was produced.
 out.actual_world=w->owner;out.application=w->application;
 out.level=prefix.published_level34?prefix.published_level34:prefix.allocated_level;
 out.selected_profile=state.profile;out.physical_world=w->physical_world;
 out.camera_application=w->camera_application;
 if(w->canonical_world){
  out.objects=w->canonical_world->manager_lease;out.properties=&w->canonical_world->properties;
  out.roots=w->canonical_world->scene_roots_v20;
 }
 if(w->application){
  out.fx_libraries=w->application->source_fx_libraries_v63();
  out.script_manager_v69=w->application->source_script_manager_v52();
 }
 if(auto candidate=state.candidate){
  if(candidate->canonical&&candidate->canonical!=w->canonical_world){e="Startup canonical graph differs from its actual World";return false;}
  if(candidate->preparation){
   if(candidate->preparation->level()!=out.level){e="Startup preparation belongs to another Level";return false;}
   out.floors=candidate->preparation->floor_world();
  }
  out.navigation_registry=candidate->navigation;out.module_rooms_v69=candidate->rooms;
  if(candidate->rooms)out.map_owner_v69=candidate->rooms->map_owner_v69();
  out.receiver_transport_v69=candidate->transport;
 }
 // These are actual nullable reached owners, not loading/readiness synthesis.
 e.clear();return true;
}
SourceCampaignRetirementResultV88 release_source_campaign_startup_prefix_v114(
 const SourceCampaignStartupPrefixV114& prefix,std::string& e){
 using R=SourceCampaignRetirementResultV88;
 if(!source_campaign_v55||!source_campaign_v55->failed||
    source_campaign_v55->world!=prefix.world||source_campaign_v55->bootstrap.gs()!=prefix.gs||
    source_campaign_v55->globals!=prefix.globals){
  e="Failed-start release addressed a replaced native prefix";return R::failed;
 }
 if(prefix.published_level34||(prefix.gs&&prefix.gs->fields().level34)){
  // A completed C1 was published before a later callback failed. Consume
  // the existing qualified release instead of treating it as an incomplete C1.
  if(!prefix.level_c1_complete||!prefix.world||!prefix.world->release_v88||!prefix.gs){
   e="Published failed-start Level requires its actual qualified release owner";return R::failed;
  }
  if(!prefix.gs->destroy(e))return R::failed;
  bool unload{},destroy{};
  if(!source_campaign_release_complete_v88(prefix.world->release_v88,unload,destroy,e)||!unload||!destroy){
   if(e.empty())e="Published failed-start Level release has not completed";return R::failed;
  }
 }else if(prefix.gs){
  if(!prefix.gs->abort_unpublished_constructor_v114(e))return R::failed;
 }else if(prefix.allocated_level||prefix.level_connection){
  e="Failed-start Level allocation lost its actual GS connection";return R::failed;
 }
 if(prefix.globals&&prefix.globals->s_level){e="Failed-start release left the current Level published";return R::failed;}
 e.clear();return R::complete;
}
bool begin_source_campaign_startup_abort_v114(SourceCampaignStartupAbortServicesV114 services,std::string& e){
 using Phase=RendererSourceCampaignV55::RetirementPhaseV88;
 SourceCampaignStartupPrefixV114 prefix;if(!borrow_source_campaign_startup_prefix_v114(prefix,e))return false;
 if(!services.release_prefix)services.release_prefix=release_source_campaign_startup_prefix_v114;
 auto& state=*source_campaign_v55;
 if(state.retirement_busy_v88||state.retirement_phase_v88!=Phase::idle||!services.owner||
    !services.quiesce||!services.release_prefix||!services.retire_native){e="Require once-only genuine failed-start native abort providers";return false;}
 if(prefix.world&&((prefix.world->native_loading_v50&&*prefix.world->native_loading_v50)||
                  (prefix.world->native_gslevel_v27&&*prefix.world->native_gslevel_v27))){
  e="Adopted source GS/loading must use ordinary SAME V88 release retirement";return false;
 }
 for(const auto& enclosing:{std::shared_ptr<void>(prefix.world),prefix.world?prefix.world->owner:std::shared_ptr<void>{},
      prefix.world?std::shared_ptr<void>(prefix.world->application):std::shared_ptr<void>{},
      std::shared_ptr<void>(prefix.gs),std::shared_ptr<void>(prefix.allocated_level),std::shared_ptr<void>(prefix.level_connection)}){
  if(enclosing&&!services.owner.owner_before(enclosing)&&!enclosing.owner_before(services.owner)){
   e="Startup abort services must not own containing World/GS/Level prefix";return false;
  }
 }
 if(prefix.world&&prefix.world->admission_v104){bool pending{};if(!prefix.world->admission_v104->close(pending,e))return false;}
 state.startup_abort_v114=std::move(services);state.retirement_phase_v88=Phase::prefix_quiesce;
 e.clear();return true;
}
namespace {
SourceCampaignRetirementResultV88 drain_source_campaign_startup_prefix_v114(std::string& e){
 using R=SourceCampaignRetirementResultV88;using P=RendererSourceCampaignV55::RetirementPhaseV88;
 auto& state=*source_campaign_v55;if(state.retirement_busy_v88){e.clear();return R::pending;}
 state.retirement_busy_v88=true;struct Busy{bool* flag;~Busy(){if(flag)*flag=false;}} busy{&state.retirement_busy_v88};
 auto fail=[&]{if(state.retirement_first_failure_v114.empty())state.retirement_first_failure_v114=e.empty()?"Genuine failed-start abort provider failed":e;
  state.retirement_failure_v88=state.retirement_first_failure_v114;state.retirement_resume_v114=state.retirement_phase_v88;
  state.retirement_phase_v88=P::failed;e=state.retirement_failure_v88;return R::failed;};
 try{
  SourceCampaignStartupPrefixV114 prefix;if(!borrow_source_campaign_startup_prefix_v114(prefix,e))return fail();
  auto current=[&]{if(source_campaign_v55.get()!=&state||state.world!=prefix.world||state.bootstrap.gs()!=prefix.gs){e="SAME failed-start prefix changed during native abort";return false;}return true;};
  if(state.retirement_phase_v88==P::prefix_quiesce){
   const auto r=state.startup_abort_v114.quiesce(prefix,e);if(r==R::failed)return fail();if(r==R::pending){e.clear();return r;}
   state.retirement_phase_v88=P::prefix_cancel; //Commit true native quiescence before post-scope checks.
   if(!current())return fail();
  }
  if(state.retirement_phase_v88==P::prefix_cancel){
   //Cancel only the actual retained connection, if C1 progressed that far.
   //Absent loading is genuine absence; partial native C1 still needs release.
   if(!state.startup_cancel_requested_v114){
    if(state.bootstrap.loading())state.bootstrap.loading()->request_cancel();
    state.startup_cancel_requested_v114=true;
   }
   if(state.bootstrap.loading()){
    const auto r=state.bootstrap.loading()->drain_cancel(e);
    if(r==dh2::loader::LifecycleStatusV36::failed)return fail();
    if(r!=dh2::loader::LifecycleStatusV36::cancelled){e.clear();return R::pending;}
   }
   state.retirement_phase_v88=P::prefix_release;
  }
  if(state.retirement_phase_v88==P::prefix_release){
   if(!state.startup_release_complete_v114){const auto r=state.startup_abort_v114.release_prefix(prefix,e);
    if(r==R::failed)return fail();if(r==R::pending){e.clear();return r;}state.startup_release_complete_v114=true;
   }
   if(!current())return fail();
   if((prefix.gs&&prefix.gs->fields().level34)||(prefix.globals&&prefix.globals->s_level)){
    e="Actual partial GS/Level prefix release did not clear native34/s_level";return fail();
   }
   //When enrolled, use the existing release receipts, never another journal.
   if(prefix.world&&prefix.world->release_v88){bool unload{},destroy{};
    if(!source_campaign_release_complete_v88(prefix.world->release_v88,unload,destroy,e)||!unload||!destroy){
     if(e.empty())e="Existing startup release journals remain incomplete";return fail();
    }
   }
   state.retirement_phase_v88=P::prefix_native_retire;
  }
  if(!state.startup_native_complete_v114){const auto r=state.startup_abort_v114.retire_native(prefix,e);
   if(r==R::failed)return fail();if(r==R::pending){e.clear();return r;}state.startup_native_complete_v114=true;
  }
  if(!current())return fail();
  if((prefix.gs&&prefix.gs->fields().level34)||(prefix.globals&&prefix.globals->s_level)){
   e="Native unpublication changed cleared startup source slots";return fail();
  }
  if(prefix.world){
   if(prefix.world->native_loading_v50)prefix.world->native_loading_v50->reset();
   if(prefix.world->native_gslevel_v27)prefix.world->native_gslevel_v27->reset();
   if(prefix.world->native_level_c1_v25)prefix.world->native_level_c1_v25->reset();
  }
  //Only authentic native release/final retirement permits wrapper storage drop.
  busy.flag=nullptr;source_campaign_v55.reset();e.clear();return R::complete;
 }catch(const std::exception& ex){e=ex.what();return fail();}
 catch(...){e="Genuine failed-start abort threw; reached prefix retained";return fail();}
}
}
SourceCampaignRetirementResultV88 drain_source_campaign_retirement_v88(std::string& e){
 using Result=SourceCampaignRetirementResultV88;using Phase=RendererSourceCampaignV55::RetirementPhaseV88;
 if(source_campaign_v55&&(source_campaign_v55->retirement_phase_v88==Phase::prefix_quiesce||
    source_campaign_v55->retirement_phase_v88==Phase::prefix_cancel||
    source_campaign_v55->retirement_phase_v88==Phase::prefix_release||
    source_campaign_v55->retirement_phase_v88==Phase::prefix_native_retire))return drain_source_campaign_startup_prefix_v114(e);
 if(!source_campaign_v55||!source_campaign_retirement_requested_v88()){e="Required actual retained campaign retirement request";return Result::failed;}
 auto& state=*source_campaign_v55;
 if(state.retirement_phase_v88==Phase::failed){e=state.retirement_failure_v88;return Result::failed;}
 if(state.retirement_busy_v88){e.clear();return Result::pending;} //existing synchronous native scope
 state.retirement_busy_v88=true;
 //Capture independent strong scopes before callbacks. Native root may remove
 //its external aliases, but this World/frame cannot expire until delivery ends.
 auto world=state.world;auto gs=state.bootstrap.gs();auto globals=state.globals;
 struct Busy {bool* flag;~Busy(){if(flag)*flag=false;}} busy{&state.retirement_busy_v88};
 auto fail=[&]{state.retirement_failure_v88=e.empty()?"Actual source campaign retirement provider failed":e;
  if(state.retirement_first_failure_v114.empty())state.retirement_first_failure_v114=state.retirement_failure_v88;
  state.retirement_failure_v88=state.retirement_first_failure_v114;
  state.retirement_resume_v114=state.retirement_phase_v88;
  state.retirement_phase_v88=Phase::failed;e=state.retirement_failure_v88;return Result::failed;};
 try{
  if(!world||!gs||!globals||!world->native_loading_v50||!*world->native_loading_v50||
     !world->native_gslevel_v27||world->native_gslevel_v27->get()!=gs.get()){e="Retirement lost SAME GS/loading/World ownership";return fail();}
  if(state.retirement_phase_v88==Phase::admit){
   const auto result=state.retirement_v88.quiesce(world->owner,e);
   if(result==Result::failed)return fail();if(result==Result::pending){e.clear();return Result::pending;}
   //Admission closed before cancellation touches actual loading resources.
   (*world->native_loading_v50)->request_cancel();state.cancelling=true;state.retirement_phase_v88=Phase::cancel;
  }
  if(state.retirement_phase_v88==Phase::cancel){
   const auto result=(*world->native_loading_v50)->drain_cancel(e);
   if(result==dh2::loader::LifecycleStatusV36::failed)return fail();
   if(result!=dh2::loader::LifecycleStatusV36::cancelled){e.clear();return Result::pending;}
   state.cancelling=false;state.cancelled=true;state.retirement_phase_v88=Phase::gs_destroy;
  }
  if(state.retirement_phase_v88==Phase::gs_destroy){
   //Exactly ONE existing original GS Dtor. Its journals own Unload, HUD close,
   //Level D1 and both source slot clears. Failure never replays that prefix.
   if(!state.retirement_gs_complete_v114){
    if(!gs->destroy(e))return fail();state.retirement_gs_complete_v114=true;
   }
   if(gs->fields().level34||globals->s_level){e="Original GS Dtor did not unpublish actual34/s_level";return fail();}
   bool unload{},destroy{};if(!source_campaign_release_complete_v88(world->release_v88,unload,destroy,e)||!unload||!destroy){e="GS destruction returned before actual Level release journals completed";return fail();}
   state.retirement_phase_v88=Phase::native_retire;
  }
  if(!state.retirement_native_complete_v114){
   const auto result=state.retirement_v88.retire_world(world->owner,e);
   if(result==Result::failed)return fail();if(result==Result::pending){e.clear();return Result::pending;}
   state.retirement_native_complete_v114=true;
  }
  if(gs->fields().level34||globals->s_level||source_campaign_v55->world!=world){e="Actual source/renderer ownership changed during final retirement";return fail();}
  //Native retirement observed unpublication. Only now release source wrappers;
  //no destruction/readiness callback is simulated by storage expiration.
  world->native_loading_v50->reset();world->native_gslevel_v27->reset();
  if(world->native_level_c1_v25)world->native_level_c1_v25->reset();
  busy.flag=nullptr;source_campaign_v55.reset();e.clear();return Result::complete;
 }catch(const std::exception& ex){e=ex.what();return fail();}
 catch(...){e="Source campaign retirement provider threw; retained reached prefix";return fail();}
}

}
