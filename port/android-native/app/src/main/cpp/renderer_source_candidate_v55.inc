// Include in model_renderer.cpp AFTER the actual World/canonical/C1 owner
// definitions and table loaders. Caller retains candidate + World separately;
// candidate callbacks borrow World weakly, preventing World/loading cycles.
namespace {
struct RendererImmutableTablesV55 {
 std::shared_ptr<dh2::character::CharacterGameDesign> design;
 std::shared_ptr<dh2::data::DesignSettingsOwner> settings;
 std::shared_ptr<dh2::data::SkillTables> skills;
 std::shared_ptr<dh2::data::EffectsTables> effects;
};
std::shared_ptr<RendererImmutableTablesV55> renderer_immutable_tables_v55;
std::shared_ptr<RendererImmutableTablesV55> source_immutable_tables_v55(AAssetManager* assets){
 if(renderer_immutable_tables_v55)return renderer_immutable_tables_v55;
 if(!assets)throw std::runtime_error("Required actual APK immutable Arrays provider");
 auto tables=std::make_shared<RendererImmutableTablesV55>();
 if(live_script_world){tables->design=live_script_world->design;tables->settings=live_script_world->settings_owner;
  tables->skills=live_script_world->skills_owner;tables->effects=live_script_world->effects_owner;}
 else{tables->design=load_game_design(assets);tables->settings=load_design_settings(assets);
  tables->skills=load_skill_tables(assets);tables->effects=load_effects_tables(assets);}
 if(!tables->design||!tables->settings||!tables->skills||!tables->effects)
  throw std::runtime_error("Required complete actual immutable Arrays owners");
 renderer_immutable_tables_v55=tables;return tables;
}
std::shared_ptr<WorldScriptContext> create_source_world_v55(AAssetManager* assets,
 const std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50>& profile,
 const std::string& selected_logical_level,std::string& error){
 if(!assets||!profile||profile->private_directory.empty()||profile->private_directory.front()!='/'||selected_logical_level.empty()){
  error="Required actual APK/selected immutable profile/explicit logical Level";return {};
 }
 try{
  std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> application;
  if(!model_renderer::borrow_actual_application_services_v5(application,error))return {};
  std::shared_ptr<dh2::character::DebugSwitches> actual_debug;
  if(application->source_debug_services_v55())
   actual_debug=application->source_debug_services_v55();
  else{
   actual_debug=live_script_world?live_script_world->debug:
    std::shared_ptr<dh2::character::DebugSwitches>(dh2_character_debug_create(),WorldScriptContext::DebugDelete{});
   if(!actual_debug||!application->publish_source_debug_services_v55(actual_debug,error))return {};
  }
  // Share the same immutable Arrays used to select the saved logical row.
  // No mutable World/Character/Script/Scene receiver is copied.
  const auto held_tables=source_immutable_tables_v55(assets);const auto& tables=*held_tables;
  auto world=std::make_shared<WorldScriptContext>(tables.design,tables.settings,tables.skills,tables.effects,
    profile->private_directory,selected_logical_level,actual_debug);
  world->source_application_v55=application;
  world->canonical_world=std::make_shared<RendererCanonicalWorldV4>(*world);
  error.clear();return world;
 }catch(const std::exception& failure){error=failure.what();return {};}
}
struct RendererSourceFactoryCoreV55 {
 std::shared_ptr<RendererCanonicalWorldV4> canonical;
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
 std::shared_ptr<WorldScriptContext> world;
 std::shared_ptr<dh2::loader::CanonicalLevelContextV1> level;
 std::shared_ptr<RendererLevelApplicationV25> application;
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
 std::weak_ptr<WorldScriptContext> world;
 std::shared_ptr<RendererCanonicalWorldV4> canonical;
 std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50> profile;
 std::shared_ptr<RendererSourceFactoryCoreV55> factory;
 std::shared_ptr<dh2::loader::CanonicalReceiverTransportV1> transport;
 std::shared_ptr<dh2::world::CanonicalModuleGraphV3> graph;
 std::shared_ptr<dh2::world::ModulePFRoomsV3> rooms;
 std::shared_ptr<dh2::loader::RetainedLevelModuleGraphV1> preparation;
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
  auto manager=std::shared_ptr<CanonicalObjectManagerV1>(candidate->canonical,&candidate->canonical->manager);
  if(!manager->borrow_fresh_source_v50(manager,candidate->manager_freshness,error))return false;
  if(candidate->canonical->scene_roots_v20&&candidate->canonical->scene_roots_v20!=in.roots){error="Source candidate attempted a second SceneManager root owner";return false;}
  candidate->canonical->scene_roots_v20=in.roots;
  const auto& source=in.level->constructor_fields_v3();
  in.world->host_level.row_index=source.row3c;in.world->host_level.difficulty=source.difficulty40;
  candidate->rooms=std::make_shared<ModulePFRoomsV3>(in.floors,in.map,std::move(in.pf_debug));
  auto graph_services=std::move(in.module_graph);
  if(graph_services.rooms||graph_services.candidate){error="Source Module graph must borrow this sole canonical candidate/PF owner";return false;}
  graph_services.rooms=candidate->rooms;graph_services.candidate=candidate->canonical;
  auto source_application=in.application->application;
  if(!source_application){error="Required actual Application services for Module spawn checks";return false;}
  graph_services.spawn_application.application_lease=source_application;
  graph_services.spawn_application.random=source_application->source_random_v62().get();
  graph_services.spawn_application.online_byte5=[application=source_application](bool& online,std::string& e){
   auto source=application->get_online_loading_v55();if(!source){e="Required actual Application COnline owner";return false;}
   online=source->byte5()!=0;e.clear();return true;
  };
  graph_services.mark_for_deletion=[weak=std::weak_ptr<SourceWorldBorrowV61>(in.world)](CanonicalGameObjectBaseOwnerV1& base,std::string& e){
   auto world=weak.lock();if(!world||!world->canonical_world){e="Released actual Module ObjectManager owner";return false;}
   return world->canonical_world->manager.source_mark_for_deletion_v89(base.identity(),e);
  };
  if(graph_services.visual.roots&&graph_services.visual.roots!=in.roots){error="Source Module visual uses foreign SceneManager roots";return false;}
  graph_services.visual.roots=in.roots;
  // SAME actual V54 device witness, independent of high-performance flags.
  graph_services.visual.driver_type=[](std::int32_t& type,std::string& e){return model_renderer::borrow_actual_device_driver_type_v55(type,e);};
  candidate->graph=std::make_shared<CanonicalModuleGraphV3>(std::move(graph_services));
  auto core=std::make_shared<RendererSourceFactoryCoreV55>();core->canonical=candidate->canonical;core->remaining=std::move(in.remaining);core->unknown_type_debug=std::move(in.unknown_type_debug);
  CanonicalLevelModuleConstructionV2 construction;construction.candidate=candidate->canonical;
  construction.module_globals=&in.application->source->module_globals;construction.level_config=std::move(in.config);
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
  if(!in.application->files->cache.borrow_archive_v55(archive,error))return false;
  auto classes=candidate->transport->services();
  if(!RetainedLevelModuleGraphV1::create({archive,in.level,candidate->canonical,&candidate->canonical->manager,classes,
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
  out.preparation=preparation;out.manager={canonical,&canonical->manager};
  // These sibling leases pin the raw dispatch callbacks without retaining
  // candidate or its containing World, avoiding a loading ownership cycle.
  out.resource_pins.push_back(transport);out.resource_pins.push_back(factory);
  out.resource_pins.push_back(graph);out.resource_pins.push_back(rooms);
  error.clear();return true;
 }
};
}
