from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
CPP=ROOT/'port/android-native/app/src/main/cpp'
BASE=ROOT/'port/level-loader/reports/campaign-runtime-v61/baseline'
prefix=(BASE/'renderer_source_candidate_v55.inc').read_text().split('struct RendererSourceFactoryCoreV55')[0]
# Prefix already opens its renderer-local anonymous namespace.
body=r'''
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
 result->debug_files=&world->debug_files;result->host_level=&world->host_level;result->files_directory=world->files_directory;
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 if(!borrow_actual_application_services_v5(application,error))return false;
 result->application=application;
 const auto canonical=world->canonical_world;
 auto manager=std::shared_ptr<dh2::world::CanonicalObjectManagerV1>(canonical,&canonical->manager);
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
 result->read=[files=level_app->files](const std::string& name,bool& found,std::vector<std::uint8_t>& bytes,std::string& e){return files->cache.read(name,found,bytes,e);};
 result->archive=[files=level_app->files](dh2::assets::ZipAssetPackV1& archive,std::string& e){return files->cache.borrow_archive_v55(archive,e);};
 std::shared_ptr<dh2::input::SourceInputManagerV60> input;
 if(!borrow_actual_input_manager_v60(input,error)||!input||
    !prepare_source_application_player_manager_v59(application,result->player_manager,error))return false;
 if(!result->player_manager->source_first_local_add_prefix(input->first_local_services(),error))return false;
 dh2::player::PlayerInfoFieldsV1* actual_local{};
 if(!result->player_manager->get_local_player(0,false,actual_local,error)||!actual_local||actual_local->character660){
  if(error.empty())error="Prior Character still owns source660; require actual unload";return false;
 }
 dh2::camera::CameraApplicationBindingsV23 camera_bindings;
 if(!application->native_camera_services_v20()){
  camera_bindings.actual_roots=std::make_shared<dh2::world::GameObjectSceneRootRegistryV1>();
  camera_bindings.actual_dictionary=&actor_clip_table;
  camera_bindings.actual_dictionary_lease=std::shared_ptr<void>(&actor_clip_table,[](void*){});
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
 error.clear();return true;
}
SourceCampaignRendererServicesV61 renderer_source_services_v61(){
 return {renderer_gs_globals_v27,create_renderer_source_borrow_v61};
}
}
'''
(CPP/'renderer_source_boundary_v61.inc').write_text(prefix+body)
print('Wrote thin boundary with actual renderer allocations and field references.')
