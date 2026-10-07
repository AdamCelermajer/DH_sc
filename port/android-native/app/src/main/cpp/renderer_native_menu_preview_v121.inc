// Included after renderer_source_boundary_v61.inc. This adapter uses the live
// native renderer/cache and the retained process SceneManager; no Level is made.
#include "renderer_menu_preview_domain_v121.hpp"
#include "source_process_objects_v121.hpp"
namespace {
struct NativeMenuRendererDomainV121 {
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 AAssetManager* assets{};std::string directory;
 std::shared_ptr<RendererLevelFilesV25> files;
 std::shared_ptr<RendererImmutableTablesV55> tables;
 std::shared_ptr<SourceApplicationDebugTransportV61> debug;
 std::weak_ptr<dh2::camera::GameplayCameraApplicationV23> cameras;
 std::shared_ptr<dh2::character::CharacterCandidateCacheV62> character_cache;
 std::shared_ptr<dh2::character::CharacterAnimationSetCacheV6> animation_manager=std::make_shared<dh2::character::CharacterAnimationSetCacheV6>();
 std::string character_cache_failure;
 bool current(std::string& e)const{
  auto app=application.lock();
  if(!app||!assets||!files||!tables||files->directory!=directory){e="Retired menu renderer resource domain";return false;}
  e.clear();return true;
 }
};
struct NativeMenuFxDebugV121 {
 std::shared_ptr<SourceApplicationDebugTransportV61> source;
 std::shared_ptr<dh2::fx::DebugModules> modules;
};
std::map<std::uintptr_t,std::weak_ptr<NativeMenuRendererDomainV121>> menu_renderer_domains_v121;
struct NativeMenuPropertyProviderV121 {
 std::shared_ptr<dh2::character::CharacterGameDesign> design;
 dh2::world::CanonicalPropertyMapV1 map;
 explicit NativeMenuPropertyProviderV121(std::shared_ptr<dh2::character::CharacterGameDesign> actual):design(std::move(actual)),
  map({this,&dh2::world::canonical_vec3_origin_v1(),nullptr,diagnostic,this,constant}){}
 static bool diagnostic(void*,const char* text,std::string& e){
  if(!text){e="Missing source Point3 list diagnostic";return false;}
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original property parse diagnostic | %s",text);e.clear();return true;
 }
 static bool constant(void* raw,const char* group,const char* key,std::int32_t& value,std::string& e){
  auto& self=*static_cast<NativeMenuPropertyProviderV121*>(raw);auto tables=self.design->borrow();const auto* source=tables.design();
  if(!source||!source->lookup||source->lookup(source->context,0,group,key,&value)){e="Missing actual process property constant";return false;}
  e.clear();return true;
 }
};
std::map<std::uintptr_t,std::weak_ptr<NativeMenuPropertyProviderV121>> menu_property_providers_v121;
// A real native Scene node over the renderer's one live authored scene graph.
// GPU draw views and animation remain in that graph's existing renderer domain.
// Source caller and parent references are tracked separately from host pins.
struct NativeMenuSceneNodeV121:std::enable_shared_from_this<NativeMenuSceneNodeV121> {
 std::shared_ptr<NativeMenuRendererDomainV121> domain;
 std::uint32_t flags11c{0x60f},references{1};std::uintptr_t parentec{},manager110{};
 bool alive{true},local_visible{true},parent_visible{true};
 bool drop(std::string& e){
  if(!alive||!references){e="Menu scene node reference already retired";return false;}
  if(references==1&&parentec){e="Menu scene node still owns its source parent";return false;}
  if(--references){e.clear();return true;}
  release_class_previews();release(draws,images);current_scene={};player=dh2::animation::Player{};
  menu_background=class_scene=false;enabled=false;alive=false;e.clear();return true;
 }
 dh2::world::GameObjectSceneRootBorrowV1 borrow(){
  auto self=shared_from_this();dh2::world::GameObjectSceneRootBorrowV1 root;
  root.owner=self;root.identity=reinterpret_cast<std::uintptr_t>(this);root.flags11c=&flags11c;root.parentec=&parentec;
  root.notify_visibility=[self](bool value,auto& e){if(!self->alive){e="Retired native menu scene visibility";return false;}self->parent_visible=value;e.clear();return true;};
  root.remove_animators=[self](auto& e){if(!self->alive){e="Retired native menu scene animator";return false;}player=dh2::animation::Player{};e.clear();return true;};
  root.scene_manager_changed=[self](auto id,auto& e){if(!self->alive){e="Retired native menu scene manager";return false;}self->manager110=id;e.clear();return true;};
  root.scene_phase_v69=[self](auto,auto& e){
   if(!self->alive||!self->domain->current(e))return false;
   // Rendering samples the SAME existing authored animation cursor. No second
   // animation clock or state/event delivery is introduced by registration.
   return player.sample(current_scene,sampled_ms,e);
  };
  root.acquire_parent_reference_v110=[self](auto& e){if(!self->alive||self->references==UINT32_MAX){e="Invalid native menu scene grab";return false;}++self->references;e.clear();return true;};
  root.release_parent_reference_v110=[self](auto& e){return self->drop(e);};
  return root;
 }
};
std::weak_ptr<NativeMenuSceneNodeV121> menu_scene_node_v121;
}
bool borrow_native_menu_preview_domain_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 AAssetManager* assets,const std::string& directory,MenuPreviewProcessDomainV121& out,std::string& e){
 out={};if(!app||!app->events14()||!assets||directory.empty()){e="Required actual menu Application/cache/private directory";return false;}
 try{
  auto& entry=menu_renderer_domains_v121[app->identity()];auto domain=entry.lock();
  if(domain&&(domain->application.lock()!=app||domain->assets!=assets||domain->directory!=directory)){e="Menu renderer process domain replaced without retirement";return false;}
  if(!domain){
   domain=std::make_shared<NativeMenuRendererDomainV121>();domain->application=app;domain->assets=assets;domain->directory=directory;
   domain->files=std::make_shared<RendererLevelFilesV25>(assets,directory);domain->files->actual_save_files=app->source_save_files_v61();
   domain->tables=source_immutable_tables_v55(assets);domain->debug=std::make_shared<SourceApplicationDebugTransportV61>();
   domain->debug->files=domain->files;domain->debug->actual=app->source_debug_services_v55();
   if(!domain->debug->actual){e="Required actual process Debug owner before menu preview";return false;}
   entry=domain;
  }
  out.owner=domain;
  std::shared_ptr<dh2::camera::GameplayCameraApplicationV23> camera;
  dh2::camera::CameraApplicationBindingsV23 bindings;
  if(!app->native_camera_services_v20()){
   bindings.actual_roots=std::make_shared<dh2::world::GameObjectSceneRootRegistryV1>();
   bindings.actual_dictionary=domain->tables->animation_dictionary.get();bindings.actual_dictionary_lease=domain->tables->animation_dictionary;
   bindings.actual_file_system=std::shared_ptr<void>(domain->files,assets);
   bindings.backend.configured_backend=bindings.actual_file_system;bindings.backend.viewport=borrow_actual_camera_viewport_v20;
   bindings.actual_lg_devices=borrow_actual_camera_lg_device_v20;
   bindings.read=[files=domain->files](const auto& name,auto& bytes,auto& e){bool found{};if(!files->cache.read(name,found,bytes,e))return false;if(!found){e="Missing authored process camera resource: "+name;return false;}return true;};
   bindings.debug_after_add=[debug=domain->debug](auto& e){return debug->trace(e);};
   bindings.zoom.touch=[](const auto& event,auto& out,auto& e){dh2::events::InputEventProjectionV120 payload;if(!event.input_v120||!event.input_v120(event.context,payload,e)){if(e.empty())e="Missing typed process Zoom input";return false;}out.x=payload.x8;out.y=payload.y_a;out.finger=payload.index_c;out.pressed=payload.pressed10!=0;return true;};
   bindings.zoom.map_byte1e4=[](auto&,auto& e){e="Required actual Map byte1e4 process borrower";return false;};
   bindings.zoom.map_point_inside=[](auto,auto,auto&,auto& e){e="Required actual Map render zone process borrower";return false;};
  }
  if(!borrow_gameplay_camera_services_v20(app,std::move(bindings),camera,e))return false;
  domain->cameras=camera;out.scene=camera->roots();
  std::shared_ptr<dh2::world::SourceProcessObjectsV121> objects;
  if(!dh2::world::SourceProcessObjectsV121::acquire(app,objects,e))return false;out.objects=objects->manager();
  auto& property_entry=menu_property_providers_v121[app->identity()];auto properties=property_entry.lock();
  if(!properties){properties=std::make_shared<NativeMenuPropertyProviderV121>(domain->tables->design);property_entry=properties;}
  out.properties=std::shared_ptr<dh2::world::CanonicalPropertyMapV1>(properties,&properties->map);
  out.physical=std::shared_ptr<dh2::physical::NativeWorld>(domain,&actor_world);
  out.design=domain->tables->design;out.settings=domain->tables->settings;out.skills=domain->tables->skills;out.effects=domain->tables->effects;
  out.animations=domain->tables->animation_dictionary;out.debug=domain->debug->actual;out.debug_files=&domain->debug->service;
  out.read=[files=domain->files](const auto& uri,auto& found,auto& bytes,auto& e){return files->cache.read(uri,found,bytes,e);};
  out.read_save=[files=domain->files](const auto& uri,auto& found,auto& bytes,auto& e){return files->saved(uri,found,bytes,e);};
  out.directory=[files=domain->files](auto& names,auto& e){return files->cache.directory(names,e);};
  if(!domain->character_cache){
   if(!domain->character_cache_failure.empty()){e=domain->character_cache_failure;return false;}
   dh2::character::ScriptAssetServicesV1 files;files.read=out.read;files.directory=out.directory;
   domain->character_cache=std::make_shared<dh2::character::CharacterCandidateCacheV62>(std::move(files));
   if(!domain->character_cache->load(e)){domain->character_cache_failure=e;return false;}
  }
  out.character_cache=domain->character_cache;out.animation_manager=domain->animation_manager;
  out.controller_blocked=&controller_global_blocked;
  if(!borrow_native_menu_preview_text_v121(app,out.localization,out.text_environment,out.text_owner,e))return false;
  auto fx=app->source_fx_libraries_v63();
  if(!fx){
   auto debug=std::make_shared<NativeMenuFxDebugV121>();debug->source=domain->debug;
   debug->modules=std::shared_ptr<dh2::fx::DebugModules>(dh2_fx_debug_modules_create(debug->source->actual.get(),&debug->source->service),dh2_fx_debug_modules_destroy);
   if(!debug->modules){e="Required actual process FX DebugModules constructor";return false;}
   dh2::fx::VisualFxLibraryDebugV63 service;service.application=app;
   service.invoke=[debug](auto operation,const char* name,auto& value,auto& e){
    if(dh2_fx_debug_preload_service(debug->modules.get(),operation,name,&value)!=0){e="Actual process FX Debug operation failed";return false;}
    e.clear();return true;
   };
   fx=std::make_shared<dh2::fx::VisualFxManagerLibrariesV63>(domain->tables->effects->borrow(),std::move(service));
   if(!app->publish_source_fx_libraries_v63(fx,e))return false;
  }
  if(!fx->belongs_to(app)){e="Process preview FX library belongs to another App";return false;}
  out.fx_libraries=fx;
  out.character_services=[weak=std::weak_ptr<dh2::application::ApplicationServicesOwnerV5>(app),inputs=out](auto slot,auto fresh,auto& services,auto& e){
   auto same=weak.lock();if(!same){e="Retired process Character services Application";return false;}
   return build_native_menu_preview_character_services_v122(same,inputs,slot,fresh,services,e);
  };
  e.clear();return true;
 }catch(const std::exception& failure){e=failure.what();return false;}
}
bool build_native_menu_preview_renderer_services_v121(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 AAssetManager* assets,const std::string& directory,NativeMenuPreviewServicesV121& out,std::string& e){
 MenuPreviewProcessDomainV121 domain;if(!borrow_native_menu_preview_domain_v121(app,assets,directory,domain,e))return false;
 auto native=std::static_pointer_cast<NativeMenuRendererDomainV121>(domain.owner);out.owner=native;out.scene=domain.scene;out.objects=domain.objects;
 out.current=[native](auto& e){return native->current(e);};
 out.camera_backend.configured_backend=native;out.camera_backend.viewport=borrow_actual_camera_viewport_v20;
 out.remove_all_scene_nodes=[native](auto& e){
  if(!native->current(e))return false;auto camera=native->cameras.lock();auto scene=camera?camera->roots():nullptr;
  if(!scene||scene->source_delivery_busy_v106()){e="Menu Scene.RemoveAll overlaps actual source delivery";return false;}
  for(auto id:scene->roots())if(!scene->remove_root_parent_reference_v1(id,e))return false;
  return scene->retire_source_cached_aliases_v106(e);
 };
 out.flush_objects=[native](auto& e){
  auto app=native->application.lock();std::shared_ptr<dh2::world::SourceProcessObjectsV121> objects;
  if(!native->current(e)||!dh2::world::SourceProcessObjectsV121::acquire(app,objects,e))return false;
  return objects->flush([](auto&,auto& e){if(model_publication_depth_v50||!actor_world.cleanup_delivery_idle_v106()){e="Menu ObjectManager.Flush overlaps native delivery";return false;}e.clear();return true;},e);
 };
 out.flush_animation_sets=[native](auto& e){auto camera=native->cameras.lock();if(!native->current(e)||!camera){if(e.empty())e="Lost process AnimSetManager";return false;}if(!native->animation_manager->source_flush_v121(e))return false;camera->flush_animation_sets();e.clear();return true;};
 // The native backend shares its actual texture ownership ledger instead of
 // maintaining a second untracked C2D cache. Sweep only genuinely unused GPU
 // allocations; retained UI/movie textures survive both cleanup operations.
 out.free_textures=[](auto& e){return clean_native_graphics_v50(e);};out.clean_glitch=out.free_textures;
 out.load_physics=[native](float x,float y,float right,float bottom,auto& e){if(!native->current(e)||!actor_world.cleanup_delivery_idle_v106()){if(e.empty())e="Menu physics load overlaps native delivery";return false;}const float bounds[]{x,y,right,bottom};actor_world.load(bounds);e.clear();return true;};
 out.construct_scene=[native](const char* file,MenuPreviewSceneV121& result,auto& e){
  result={};if(!file||!native->current(e))return false;
  if(auto previous=menu_scene_node_v121.lock();previous&&previous->alive){e="Previous authored menu scene has not reached D1";return false;}
  if(source_campaign_active_v55()){e="Menu scene construction precedes actual campaign retirement";return false;}
  try{auto bytes=read(native->assets,file,"models");const auto report=load(bytes.data(),bytes.size(),native->assets);
   if(report.find("3D upload OK")!=0){e=report;return false;}
   auto node=std::make_shared<NativeMenuSceneNodeV121>();node->domain=native;menu_scene_node_v121=node;
   result.owner=node;result.identity=reinterpret_cast<std::uintptr_t>(node.get());result.root=node->borrow();
   result.drop_constructor_reference=[node](auto& e){return node->drop(e);};
   menu_persona_class=-1;menu_background=true;e.clear();return true;
  }catch(const std::exception& failure){e=failure.what();return false;}
 };
 out.selected_scene_root=[native](auto& id,auto& e){
  id=0;if(!native->current(e))return false;
  if(!class_scene){e.clear();return true;}
  auto node=menu_scene_node_v121.lock();
  if(!node||!node->alive){e="Actual selection scene lacks its retained native root loan";return false;}
  id=reinterpret_cast<std::uintptr_t>(node.get());e.clear();return true;
 };
 out.remove_selected_root=[native](auto id,auto& e){
  auto camera=native->cameras.lock();auto roots=camera?camera->roots():nullptr;
  if(!native->current(e)||!roots){if(e.empty())e="Lost selection SceneManager";return false;}
  return roots->release_visual_root(id,e);
 };
 out.create_player=[weak=std::weak_ptr<dh2::application::ApplicationServicesOwnerV5>(app),assets,directory](auto slot,auto fresh,auto& result,auto& e){auto same=weak.lock();return same&&create_native_menu_preview_player_v121(same,assets,directory,slot,fresh,result,e);};
 return true;
}
