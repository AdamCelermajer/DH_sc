// Include in model_renderer.cpp AFTER the actual World/canonical/C1 owner
// definitions and table loaders. Caller retains candidate + World separately;
// candidate callbacks borrow World weakly, preventing World/loading cycles.
bool install_source_campaign_gameplay_inputs_v107(const std::shared_ptr<SourceWorldBorrowV61>&,std::string&);
bool install_campaign_noncharacter_producer_v89(const std::shared_ptr<SourceWorldBorrowV61>&,std::string&);
static bool install_campaign_no_room_primitive_v89(const std::shared_ptr<SourceWorldBorrowV61>&,std::string&);
static bool install_campaign_player_light_tweakers_v90(const std::shared_ptr<SourceWorldBorrowV61>&,std::string&);
namespace {
struct CampaignEnvironmentNameSnapshotV89;
struct RendererImmutableTablesV55 {
 std::shared_ptr<dh2::character::CharacterGameDesign> design;
 std::shared_ptr<dh2::data::DesignSettingsOwner> settings;
 std::shared_ptr<dh2::data::SkillTables> skills;
 std::shared_ptr<dh2::data::EffectsTables> effects;
 std::shared_ptr<dh2::data::Dictionary> animation_dictionary;
 std::shared_ptr<dh2::loader::GameEventTablesV50> game_events;
 std::shared_ptr<dh2::loader::GameObjectArraysOwnerV81> game_object_arrays_v81;
 bool game_object_arrays_attempted_v81{};
 std::string game_object_arrays_failure_v81;
 std::shared_ptr<CampaignEnvironmentNameSnapshotV89> campaign_names_v89;
 bool campaign_names_attempted_v89{};
 std::string campaign_names_failure_v89;
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
 // A fresh campaign never runs the legacy load_world actor-table stores.
 // Decode the process camera dictionary from the same embedded cache now;
 // its immutable lease is independent of App/World and the current GL scene.
 const auto names=read(assets,"animations_dictionary_pyarraynames.bin","data");
 const auto values=read(assets,"animations_dictionary_pyarray.bin","data");
 tables->animation_dictionary=std::make_shared<dh2::data::Dictionary>();std::string error;
 if(!dh2::data::load_dictionary({names.data(),names.size()},{values.data(),values.size()},*tables->animation_dictionary,error))
  throw std::runtime_error(error);
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

struct SourceApplicationDebugTransportV61 {
 std::shared_ptr<RendererLevelFilesV25> files;
 std::shared_ptr<dh2::character::DebugSwitches> actual;
 dh2::character::DebugFileServices24 service{this,open,close};
 static int open(void* raw,const char* name,std::uintptr_t* out){
  auto& self=*static_cast<SourceApplicationDebugTransportV61*>(raw);
  if(!self.files||!name||std::strcmp(name,"DebugSwitches.savegame")||!out)return 1;
  *out=0;errno=0;auto* file=std::fopen((self.files->directory+"/"+name).c_str(),"rb");
  if(!file)return errno==ENOENT?0:1;*out=reinterpret_cast<std::uintptr_t>(file);return 0;
 }
 static int close(void*,std::uintptr_t file){return !file||std::fclose(reinterpret_cast<std::FILE*>(file))?1:0;}
 bool trace(std::string& e){std::uint32_t ignored{};
  if(!actual||dh2_character_debug_load(actual.get(),&service)!=1||
     dh2_character_debug_get(&ignored,actual.get(),"isTracingAnimSetManager",&service)!=1){e="Required actual App Debug transport";return false;}
  e.clear();return true;
 }
};
bool create_renderer_source_borrow_v61(AAssetManager* assets,
 const std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50>& profile,
 std::shared_ptr<SourceWorldBorrowV61>& out,std::string& error){
 if(world_mode){error="Prior live gameplay requires genuine Level unload";return false;}
 if(!assets||!profile||profile->metadata.selected_difficulty<0||profile->metadata.selected_difficulty>=3){error="Required actual selected profile/APK";return false;}
 const auto immutable=source_immutable_tables_v55(assets);auto tables=immutable->design->borrow();
 const auto* levels=tables.levels();const auto row=profile->metadata.location.levels[std::size_t(profile->metadata.selected_difficulty)];
 if(!levels||row<0||std::size_t(row)>=levels->level_names.size()){error="Required actual saved LNAM/default row producer";return false;}
 const auto world=create_source_world_v55(assets,profile,levels->level_names[std::size_t(row)],error);if(!world)return false;
 auto result=std::make_shared<SourceWorldBorrowV61>();out=result; // Retain actual failed startup prefixes.
 result->owner=world;result->design=world->design;result->debug=world->debug;
 result->effects=world->effects_owner;
 result->settings=world->settings_owner;result->camera_session=&world->camera_session_v23;
 result->fx_debug=[weak=std::weak_ptr<WorldScriptContext>(world)](std::uint32_t operation,const char* name,std::uint32_t& value,std::string& e){
  auto actual=weak.lock();if(!actual||!actual->effect_modules||!actual->effect_services.call){e="Actual App FX Debug module transport expired";return false;}
  const auto result=actual->effect_services.call(actual->effect_services.context,operation,name,&value);
  if(result){e="Actual FX Debug service failed: "+std::to_string(result);return false;}
  e.clear();return true;
 };
 result->debug_files=&world->debug_files;result->host_level=&world->host_level;result->files_directory=world->files_directory;
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 if(!borrow_actual_application_services_v5(application,error))return false;
 result->application=application;
 const auto canonical=world->canonical_world;
 auto manager=canonical->manager_owner;
 result->canonical_world=std::make_shared<SourceCanonicalBorrowV61>(SourceCanonicalBorrowV61{
  canonical,manager,canonical->manager,canonical->properties,canonical->scene_roots_v20});
 result->native_level_c1_v25=&world->native_level_c1_v25;result->native_gslevel_v27=&world->native_gslevel_v27;
 result->native_loading_v50=&world->native_loading_v50;result->last_frame=&last_frame;
 result->application_dt=&current_application_dt;result->application_tick=&current_application_tick;
 result->physical_world=std::shared_ptr<dh2::physical::NativeWorld>(application,&actor_world);
 if(!renderer_level_application_v25||renderer_level_application_v25->application!=application||
    renderer_level_application_v25->files->directory!=world->files_directory)
  renderer_level_application_v25=std::make_shared<RendererLevelApplicationV25>(application,assets,world->files_directory);
 const auto level_app=renderer_level_application_v25;result->level_application=level_app->source;
 result->files_owner=level_app->files;
 // v2Events is in the embedded original ZIP, not a separately staged APK
 // asset. Decode once through the SAME archive owner used by Level.LoadFile.
 // Publish only the completed immutable table; failed candidates retain their
 // real World/file prefix without exposing a half-decoded Arrays receiver.
 if(!immutable->game_events){
  std::vector<std::uint8_t> records,names;bool found{};
  if(!level_app->files->cache.read("data/pydata/v2eventmanager_pyarray.bin",found,records,error))return false;
  if(!found){error="Original v2eventmanager record table absent";return false;}
  if(!level_app->files->cache.read("data/pydata/v2eventmanager_pyarraynames.bin",found,names,error))return false;
  if(!found){error="Original v2eventmanager names table absent";return false;}
  auto events=std::make_shared<dh2::loader::GameEventTablesV50>();
  if(!events->initialize(records.data(),records.size(),names.data(),names.size(),error))return false;
  immutable->game_events=std::move(events);
 }
 result->game_event_tables=immutable->game_events;
 result->read=[files=level_app->files](const std::string& name,bool& found,std::vector<std::uint8_t>& bytes,std::string& e){return files->cache.read(name,found,bytes,e);};
 result->read_admitted_v81=[files=level_app->files](const std::string& name,bool& found,std::vector<std::uint8_t>& bytes,const std::function<bool(std::uint32_t,std::string&)>& before,std::string& e){return files->cache.read_admitted(name,found,bytes,before,e);};
 result->archive=[files=level_app->files](dh2::assets::ZipAssetPackV1& archive,std::string& e){return files->cache.borrow_archive_v55(archive,e);};
 std::shared_ptr<dh2::input::SourceInputManagerV60> input;
 if(!borrow_actual_input_manager_v60(input,error)||!input||
    !prepare_source_application_player_manager_v59(application,result->player_manager,error))return false;
 if(!result->player_manager->source_first_local_add_prefix(input->first_local_services(),error))return false;
 dh2::player::PlayerInfoFieldsV1* actual_local{};
 if(!result->player_manager->get_local_player(0,false,actual_local,error)||!actual_local||actual_local->character660){
  if(error.empty())error="Prior Character still owns source660; require actual unload";return false;
 }
 // NativeStartGame carries the real menu-selected slot and validated file.
 // Publish that input on the SAME first local receiver before _LoadPlayer;
 // do not derive it from class IDs or install a default slot during C1.
 if(!result->player_manager->publish_selected_profile_v68(0,profile->metadata.slot,profile->metadata.character_row,error))return false;
 dh2::camera::CameraApplicationBindingsV23 camera_bindings;
 if(!application->native_camera_services_v20()){
  camera_bindings.actual_roots=std::make_shared<dh2::world::GameObjectSceneRootRegistryV1>();
  camera_bindings.actual_dictionary=immutable->animation_dictionary.get();
  camera_bindings.actual_dictionary_lease=immutable->animation_dictionary;
  camera_bindings.actual_file_system=std::shared_ptr<void>(level_app->files,assets);
  camera_bindings.backend.configured_backend=camera_bindings.actual_file_system;
  camera_bindings.backend.viewport=borrow_actual_camera_viewport_v20;camera_bindings.actual_lg_devices=borrow_actual_camera_lg_device_v20;
  camera_bindings.read=[read=result->read](const std::string& name,std::vector<std::uint8_t>& bytes,std::string& e){
   bool found{};if(!read(name,found,bytes,e))return false;if(!found){e="Source camera resource absent: "+name;return false;}e.clear();return true;
  };
  auto debug=std::make_shared<SourceApplicationDebugTransportV61>();debug->files=level_app->files;debug->actual=world->debug;
  camera_bindings.debug_after_add=[debug](std::string& e){return debug->trace(e);};
 }
 if(!borrow_gameplay_camera_services_v20(application,std::move(camera_bindings),result->camera_application,error))return false;
 // Publish the SceneManager that the SAME Application camera service actually
 // constructed/retained. The fresh canonical World was allocated before that
 // service, so its reference slot is still empty on the first native launch.
 const auto roots=result->camera_application?result->camera_application->roots():nullptr;
 if(!roots||(canonical->scene_roots_v20&&(canonical->scene_roots_v20.get()!=roots.get()||
    canonical->scene_roots_v20.owner_before(roots)||roots.owner_before(canonical->scene_roots_v20)))){
  error="Required SAME Application SceneManager publication before tweaker PostInit";return false;
 }
 canonical->scene_roots_v20=roots;
 if(!install_campaign_player_light_tweakers_v90(result,error)||
    !install_campaign_no_room_primitive_v89(result,error)||
    !install_campaign_noncharacter_producer_v89(result,error)||
    !install_campaign_generated_room_behavior_v93(result,error)||
    !install_source_campaign_gameplay_inputs_v107(result,error))return false;
 error.clear();return true;
}
SourceCampaignRendererServicesV61 renderer_source_services_v61(){
 return {renderer_gs_globals_v27,create_renderer_source_borrow_v61};
}
}
