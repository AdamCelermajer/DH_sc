// Production camera connection to the existing selected development Level.
// Canonical GSLevel startup and non-camera scene render registration remain
// separate. All drawing/input share this actual camera's submitted matrices.
#include "renderer_gameplay_camera_actor_v20.inc"
#include "renderer_gameplay_camera_services_v20.inc"
#include "renderer_gameplay_camera_config_v20.inc"
std::shared_ptr<dh2::camera::GameplayCameraApplicationV23> gameplay_camera_application_v20;
dh2::camera::GameplayCameraPickingV20 gameplay_camera_picking_v20;
bool gameplay_camera_picking_ready_v20{};
std::uint32_t gameplay_camera_report_frames_v20{};
int gameplay_camera_viewport_width_v20{},gameplay_camera_viewport_height_v20{};
bool gameplay_camera_debug_switch_v20(const char* key,bool& value,std::string& error){
 if(!live_script_world||!key){error="Required current actual camera Debug owner/key";return false;}
 auto& w=*live_script_world;std::uint32_t word{};
 if(dh2_character_debug_load(w.debug.get(),&w.debug_files)!=1||
    dh2_character_debug_get(&word,w.debug.get(),key,&w.debug_files)!=1){error="Required source camera Debug delivery";return false;}
 value=word!=0;return true;
}
void begin_gameplay_camera_release_v20(){
 gameplay_camera_picking_ready_v20=false;
 if(!gameplay_camera_application_v20)return;
 std::string error;if(!gameplay_camera_application_v20->begin_world_release(error))
  throw std::runtime_error("Source camera release prefix: "+error);
}
void finish_gameplay_camera_release_v20(){
 std::string error;
 if(gameplay_camera_application_v20){
  gameplay_camera_application_v20->flush_animation_sets();
  if(!gameplay_camera_application_v20->release_world_owners(error))throw std::runtime_error("Source camera native release: "+error);
 }
 if(live_script_world){
  live_script_world->camera_session_v23.reset();
  if(live_script_world->player_canonical&&live_script_world->player_canonical->position_fields_v7().constructed&&
     !live_script_world->player_canonical->publish_physical2dc_v20(0,error))throw std::runtime_error(error);
 }
 level.native_camera_config_v20.reset();
}
void prepare_gameplay_camera_application_v27(){
 using namespace dh2::camera;std::string error;
 auto w=live_script_world;
 if(!w||!w->canonical_world||!equipment_platform||equipment_platform->world!=w||!world_mode)
  throw std::runtime_error("Required same World/Application before GS animation-set flush");
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> app;
 if(!borrow_actual_application_services_v5(app,error))throw std::runtime_error(error);
 CameraApplicationBindingsV23 application;
 if(!app->native_camera_services_v20()){
  // Source manager membership is now one retained Application service. It
  // owns camera roots; existing direct World draw lists still render geometry.
  application.actual_roots=std::make_shared<dh2::world::GameObjectSceneRootRegistryV1>();
  application.actual_dictionary=&actor_clip_table;
  application.actual_dictionary_lease=std::shared_ptr<void>(&actor_clip_table,[](void*){}); // process-static actual owner
  application.actual_file_system=std::shared_ptr<void>(equipment_platform->manager,[](void*){}); // Android owns AssetManager
  application.backend.configured_backend=application.actual_file_system;
  application.backend.viewport=borrow_actual_camera_viewport_v20;
  application.actual_lg_devices=borrow_actual_camera_lg_device_v20;
  application.debug_after_add=[](std::string& e){bool ignored{};return gameplay_camera_debug_switch_v20("isTracingAnimSetManager",ignored,e);};
  // Normal source camera mode0 returns before minimap/touch decoding. Any
  // reached other endpoint requires its real producer rather than a dummy.
  application.zoom.touch=[](const auto&,auto&,std::string& e){e="Required source Android Zoom touch event payload";return false;};
  application.zoom.map_byte1e4=[](bool&,std::string& e){e="Required same authored minimap byte1e4";return false;};
  application.zoom.map_point_inside=[](auto,auto,bool&,std::string& e){e="Required same authored minimap point query";return false;};
 }
 if(!borrow_gameplay_camera_services_v20(app,std::move(application),gameplay_camera_application_v20,error))throw std::runtime_error(error);
 w->canonical_world->scene_roots_v20=gameplay_camera_application_v20->roots();
}
void flush_gameplay_animation_sets_v27(){
 if(!gameplay_camera_application_v20||!gameplay_camera_application_v20->ready())throw std::runtime_error("Required prepared same Application animation manager");
 gameplay_camera_application_v20->flush_animation_sets();
}
void initialize_gameplay_camera_v20(const std::string& selected_mlx_uri){
 using namespace dh2::camera;std::string error;auto w=live_script_world;
 if(!w||!w->canonical_world||!equipment_platform||!world_mode||!native_actor_ready)
  throw std::runtime_error("Required same initialized gameplay World before camera load");
 prepare_gameplay_camera_application_v27();
 auto& canonical=*w->canonical_world;canonical.scene_roots_v20=gameplay_camera_application_v20->roots();
 CameraLevelConfigV19 configured;
 if(!w->camera_config_v20){
  std::vector<std::uint8_t> mlx;if(!read_gameplay_camera_resource_v20(selected_mlx_uri,mlx,error))throw std::runtime_error(error);
  w->camera_config_v20=std::make_unique<dh2::world::SelectedWorldCameraConfigV20>();
  dh2::world::SelectedWorldCameraServicesV20 services;services.world=w;
  services.debug_switch=gameplay_camera_debug_switch_v20;
  services.register_object=[w](auto receiver,const std::string& name,const std::string& type,std::string& e){
   if(!w->canonical_world){e="Required actual selected World canonical manager";return false;}
   // The selected MLX's LevelConfig precedes Modules. Source Level C1+18c
   // is -1 there. This is the explicit development pre-module transport;
   // complete canonical Level/GS startup is not represented by this store.
   dh2::target_providers::Handle16 published{};
   return w->canonical_world->manager.add(receiver,name.c_str(),type.c_str(),-1,true,published,e);
  };
  services.publish=[w](auto config,std::string& e){
   if(live_script_world!=w||!world_mode||!config){e="Selected camera config belongs to another development Level";return false;}
   level.native_camera_config_v20=std::move(config);return true;
  };
  if(!prepare_gameplay_camera_config_v20(*w->camera_config_v20,selected_mlx_uri,std::move(mlx),
       canonical.properties,canonical.manager,std::move(services),configured,error))throw std::runtime_error(error);
 }else{
  if(w->camera_config_v20->document().borrow().uri()!=selected_mlx_uri||!w->camera_config_v20->camera(configured,error))
   throw std::runtime_error(error.empty()?"Retained World camera layout identity differs":error);
  // GL restore adopts the same already initialized config; no constructor or
  // repeated InitPost/default/registration mutation is performed.
  level.native_camera_config_v20=w->camera_config_v20->config();
 }
 CameraLoadServicesV19 actor_load;CameraLayoutServicesV14 layout;
 actor_load.runtime.actor_services.target.source_origin=&dh2::world::canonical_vec3_origin_v1();
 actor_load.runtime.actor_services.target.application_dt=[](std::uint32_t& dt,std::string&){dt=current_application_dt;return true;};
 actor_load.runtime.actor_services.debug_infinite_zoom=[](bool& value,std::string& e){return gameplay_camera_debug_switch_v20("InfiniteZoom",value,e);};
 actor_load.runtime.actor_services.handle_centering=[weak=std::weak_ptr<WorldScriptContext>(w)](auto& point,std::string& e){
  auto actual=weak.lock();if(!actual||!actual->camera_session_v23){e="Required same World camera layout";return false;}
  return source_camera_centering_v14(point,actual->camera_session_v23->bindings.layout,e);
 };
 actor_load.runtime.actor_services.target.center_offset=[](float height,PointV2& point,std::string& e){
  auto actual=gameplay_camera_application_v20?gameplay_camera_application_v20->world():nullptr;
  if(!actual||!actual->camera||!actual->camera->level()){e="Required actual authored camera center-offset receiver";return false;}
  auto runtime=actual->camera->level();CameraBasisServicesV21 basis;
  if(!borrow_authored_camera_basis_v22(runtime->scene(),0,[runtime](float& fov,std::string& failure){CameraViewV11 view;if(!runtime->view(view,failure))return false;fov=view.fov;return true;},
     &dh2::world::canonical_vec3_origin_v1(),&dh2::world::canonical_vec3_k_v1(),basis,e))return false;
  bool available{};if(!source_camera_center_offset_v21(basis,height,point,available,e))return false;
  if(!available){e="Source authored camera center offset unavailable";return false;}return true;
 };
 RendererGameplayCameraActorServicesV20 actors;actors.world=w;
 actors.development_level_present=[weak=std::weak_ptr<WorldScriptContext>(w)](bool& present,std::string&){present=world_mode&&live_script_world==weak.lock()&&level.native_floor!=nullptr;return true;};
 actors.anchor_position=[](auto,const float*&,std::string& e){e="Required positive SAME Character AnchorBase position";return false;};
 actors.delete_anchor=[](auto,std::string& e){e="Required positive SAME Character AnchorBase destructor";return false;};
 if(!bind_gameplay_camera_actors_v20(actors,actor_load,layout,error))throw std::runtime_error(error);
 auto bind_picking=[](GameplayCameraPickingV20& picking,std::string& e){
  std::int32_t width{},height{};if(!borrow_actual_camera_viewport_v20(width,height,e))return false;
  CameraPickingViewV20 driver;driver.viewport_width=driver.render_width=width;driver.viewport_height=driver.render_height=height;
  driver.scene_manager_present=driver.camera_present=true;
  if(!gameplay_camera_application_v20){e="Required source camera picking Application";return false;}
  return gameplay_camera_application_v20->picking(driver,picking,e);
 };
 layout.get_screen_coord=[bind_picking](const auto& point,auto& result,std::string& e){GameplayCameraPickingV20 picking;return bind_picking(picking,e)&&picking.screen_coord(point,result,e);};
 layout.get_world_coord=[bind_picking](const auto& screen,float height,auto& point,std::string& e){GameplayCameraPickingV20 picking;bool hit{};if(!bind_picking(picking,e)||!picking.world_coord(screen,height,point,hit,e))return false;if(!hit){e="Original camera centering ray missed limited plane";return false;}return true;};
 CameraWorldBindingsV23 bindings;bindings.world_lease=w;bindings.actual_tables=&actor_animation_tables;
 bindings.design=w->settings;bindings.actors=std::move(actor_load.runtime.actor_services);bindings.layout=std::move(layout);
 bindings.local_character0=std::move(actor_load.local_character0);bindings.overview=std::move(actor_load.overview);
 bindings.overview.source_vec3_zero=&dh2::world::canonical_vec3_origin_v1();
 bindings.overview.pf_z_bounds=[](bool&,float&,float&,std::string& e){e="Required original PFWorld singleton overview bounds";return false;};
 bindings.overview.keyboard=[](const auto&,auto&,std::string& e){e="Required original keyboard event decoder";return false;};
 bindings.overview.first_gamepad=[](bool&,auto&,std::string& e){e="Required actual source first gamepad input";return false;};
 bindings.positive_skybox=[](const auto&,std::string& e){e="Required original positive skybox mesh/root rendering provider";return false;};
 if(!gameplay_camera_application_v20->load(configured,std::move(bindings),w->camera_session_v23,error))throw std::runtime_error("Source camera load: "+error);
 gameplay_camera_report_frames_v20=0;gameplay_camera_picking_ready_v20=false;
 gameplay_camera_viewport_width_v20=gameplay_camera_viewport_height_v20=0;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original gameplay camera loaded | layout %s | file %s | node %s | animset %s | planes %d %d | same Application SceneManager Zoom | development Level transport",selected_mlx_uri.c_str(),configured.camera.file.c_str(),configured.camera.node.c_str(),configured.camera.animation_set.c_str(),configured.camera.near_plane,configured.camera.far_plane);
}
Matrix submit_gameplay_camera_v20(int width,int height){
 using namespace dh2::camera;std::string error;
 if(!gameplay_camera_application_v20||!gameplay_camera_application_v20->loaded())throw std::runtime_error("Required loaded source gameplay camera");
 if(width<=1||height<=1)throw std::runtime_error("Required actual resized gameplay camera viewport");
 if(width!=gameplay_camera_viewport_width_v20||height!=gameplay_camera_viewport_height_v20){
  // Deliberate modern backend adaptation, after original Level.SetData:
  // preserve authored vertical FOV/follow/zoom and use the actual surface
  // aspect so wide Android screens do not stretch actors or floor tiles.
  // This is not a claim of an original automatic resize rule.
  auto runtime=gameplay_camera_application_v20->world()->camera->level();
  runtime->source_aspect_changed_v18(float(width)/float(height));
  gameplay_camera_viewport_width_v20=width;gameplay_camera_viewport_height_v20=height;
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Modern camera viewport policy | %d %d | same camera aspect %.9g | authored FOV preserved",width,height,float(width)/float(height));
 }
 if(current_application_tick&&!gameplay_camera_application_v20->update(error))throw std::runtime_error("Source camera update: "+error);
 if(!gameplay_camera_application_v20->scene_phase(std::uint32_t(scene_clock),error))throw std::runtime_error("Source camera scene phase: "+error);
 CameraViewV11 view;if(!gameplay_camera_application_v20->view(view,error))throw std::runtime_error(error);
 Matrix projection;auto gpu=view.projection;
 const GpuProjectionFieldsV12 landscape_driver{0,0,0}; // actual GLES target, configured landscape surface
 dh2_camera_gpu_projection_v12(&gpu,&landscape_driver);
 std::copy_n(gpu.values,16,projection.begin());std::copy_n(view.view.values,16,submitted_view.begin());submitted_eye=view.eye;
 CameraPickingViewV20 driver;driver.camera=view;driver.viewport_width=driver.render_width=width;driver.viewport_height=driver.render_height=height;
 driver.scene_manager_present=driver.camera_present=true;
 if(!gameplay_camera_picking_v20.bind(driver,error))throw std::runtime_error("Source camera picking bind: "+error);
 gameplay_camera_picking_ready_v20=true;
 if(gameplay_camera_report_frames_v20++==2){
  auto runtime=gameplay_camera_application_v20->world()->camera->level();auto& state=runtime->level()->fields();
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original camera frame | eye %.6g %.6g %.6g | target %.6g %.6g %.6g | fov %.9g aspect %.9g | zoom %.6g default distance %.6g | viewport %d %d | shared GPU and source picking",view.eye[0],view.eye[1],view.eye[2],view.target[0],view.target[1],view.target[2],view.fov,view.aspect,state.automatic_zoom8c,state.default_distance94,width,height);
 }
 return dh2::scene::multiply(projection,submitted_view);
}
bool gameplay_camera_ray_v20(std::int32_t x,std::int32_t y,std::array<float,3>& start,std::array<float,3>& end,std::string& error){
 if(!gameplay_camera_picking_ready_v20){error="Source gameplay camera has no submitted picking frame";return false;}
 dh2::camera::CameraRayV20 ray;if(!gameplay_camera_picking_v20.ray({x,y},ray,error))return false;start=ray.start;end=ray.end;return true;
}
