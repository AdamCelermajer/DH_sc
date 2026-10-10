// Private to the standalone campaign runtime. Every callback borrows the
// original candidate's PM/Character/Scene owners; no legacy Crypt transport.
bool source_load_camera_v67(const std::shared_ptr<SourceWorldBorrowV61>& world,
 const std::shared_ptr<RendererSourceCandidateV55>& candidate,
 const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& level,std::string& e){
 using namespace dh2::camera;
 if(!world||!candidate||!level||!world->settings||!world->camera_application||!world->camera_session||
    !world->player_manager||!candidate->factory){e="Required SAME actual campaign camera owners";return false;}
 const auto fields=level->config_fields();
 const auto* config=fields.config38?candidate->factory->modules->level_config(*fields.config38):nullptr;
 if(!config){e="Original _LoadCamera requires actual produced LevelConfig38";return false;}
 const auto* file=config->string(0x24c);const auto* set=config->string(0x264);
 const auto* node=config->string(0x27c);const auto* skybox=config->string(0x234);
 const auto* near=config->integer(0x294);const auto* far=config->integer(0x298);
 if(!file||!set||!node||!skybox||!near||!far){e="Required actual produced LevelConfig camera fields";return false;}
 CameraLevelConfigV19 input;input.camera={*file,*set,*node,*near,*far};input.skybox234=*skybox;
 CameraWorldBindingsV23 bindings;
 // Canonical owner and immutable cache outlive the session without retaining
 // the enclosing World through World->session->World. Actual World loans are
 // checked weakly at each callback; its destruction releases this session.
 bindings.world_lease=candidate->canonical->owner;
 if(!borrow_source_campaign_animation_tables_v67(world->owner,bindings.actual_tables_lease,bindings.actual_tables,e))return false;
 bindings.design=world->settings->borrow();
 const auto weak=std::weak_ptr<SourceWorldBorrowV61>(world);
 const auto level_weak=std::weak_ptr<dh2::loader::CanonicalLevelContextV1>(level);
 const auto actor=[weak](std::uintptr_t id,SourceCampaignCameraActorBorrowV67& out,std::string& e){
  auto world=weak.lock();if(!world){e="Actual camera Character World expired";return false;}
  return borrow_source_campaign_camera_actor_v67(world->owner,id,out,e);
 };
 const auto position=[actor](std::uintptr_t id,PointV2& out,std::string& e){
  SourceCampaignCameraActorBorrowV67 value;if(!actor(id,value,e))return false;
  if(!value.position160){e="Required actual Character position160";return false;}
  for(unsigned i=0;i<3;++i)out[i]=value.position160[i];e.clear();return true;
 };
 bindings.actors.actor_position=position;
 bindings.actors.actor_disabled81=[actor](std::uintptr_t id,bool& out,std::string& e){
  SourceCampaignCameraActorBorrowV67 value;if(!actor(id,value,e))return false;
  if(!value.disabled81){e="Required actual Character byte81";return false;}out=*value.disabled81!=0;e.clear();return true;
 };
 bindings.actors.target.source_origin=&dh2::world::canonical_vec3_origin_v1();
 bindings.actors.target.application_dt=borrow_application_dt_v93;
 bindings.actors.target.actor_anchor=[actor,weak](std::uintptr_t id,PointV2& out,std::string& e){
  SourceCampaignCameraActorBorrowV67 value;if(!actor(id,value,e))return false;
  CameraAnchorBorrowV6 b{value.receiver,value.identity,value.anchor2e0,value.position160,{}, {}};
  // Genuine C1 NULL anchors use position160. A real positive AnchorBase must
  // supply its actual c field through the retained type directory.
  b.anchor_position=[weak](std::uintptr_t id,const float*& point,std::string& e){auto world=weak.lock();
   if(!world){e="Actual camera anchor World expired";return false;}return source_campaign_anchor_position_v75(world->owner,id,point,e);};
  return source_camera_anchor_v6(b,out,e);
 };
 bindings.actors.debug_infinite_zoom=[weak](bool& value,std::string& e){return source_debug_switch_v55(weak,"InfiniteZoom",value,e);};
 bindings.actors.target.center_offset=[weak](float height,PointV2& point,std::string& e){
  auto world=weak.lock();auto session=world&&world->camera_application?world->camera_application->world():nullptr;
  auto runtime=session&&session->camera?session->camera->level():nullptr;
  if(!runtime){e="Required actual selected camera center-offset receiver";return false;}
  CameraBasisServicesV21 basis;
  if(!borrow_authored_camera_basis_v22(runtime->scene(),runtime->selected_camera_index_v67(),
     [runtime](float& fov,std::string& e){CameraViewV11 view;if(!runtime->view(view,e))return false;fov=view.fov;return true;},
     &dh2::world::canonical_vec3_origin_v1(),&dh2::world::canonical_vec3_k_v1(),basis,e))return false;
  bool available{};if(!source_camera_center_offset_v21(basis,height,point,available,e))return false;
  if(!available){e="Original camera center-offset unavailable";return false;}e.clear();return true;
 };
 const auto local=[weak](std::uintptr_t& out,std::string& e){
  auto world=weak.lock();dh2::player::PlayerInfoFieldsV1* record{};
  if(!world||!world->player_manager||!world->player_manager->get_local_player(0,true,record,e))return false;
  if(!record){e="Required actual local PlayerInfo0 before source660 dereference";return false;}
  out=record->character660;e.clear();return true;
 };
 bindings.local_character0=local;
 auto& layout=bindings.layout;layout.world=candidate->canonical->owner;
 layout.source_party_count6c4=[weak](std::int32_t& count,std::string& e){
  auto world=weak.lock();auto* field=world&&world->player_manager?world->player_manager->count_field():nullptr;
  if(!field){e="Required actual PM count6c4";return false;}count=*field;e.clear();return true;
 };
 layout.source_online5=[weak](std::uint8_t& value,std::string& e){
  auto world=weak.lock();auto online=world&&world->application?world->application->get_online_loading_v55():nullptr;
  if(!online){e="Required SAME actual Online.byte5";return false;}value=online->byte5();e.clear();return true;
 };
 layout.get_player=[weak,actor](unsigned index,CameraPartyActorV14& out,std::string& e){
  auto world=weak.lock();dh2::player::PlayerInfoFieldsV1* record{};
  if(!world||!world->player_manager||!world->player_manager->manager()||
     !world->player_manager->manager()->get_player(static_cast<std::int32_t>(index),false,record,e))return false;
  if(!record){e="Required actual source GetPlayer(index,false) receiver";return false;}
  out={};out.character=record->character660;if(!out.character){e.clear();return true;}
  SourceCampaignCameraActorBorrowV67 value;if(!actor(out.character,value,e))return false;
  if(!value.dead||!value.position160){e="Required SAME Character life/position for autozoom";return false;}
  out.dead=*value.dead!=0;for(unsigned i=0;i<3;++i)out.position160[i]=value.position160[i];e.clear();return true;
 };
 layout.num_local_players=[weak](std::int32_t& count,std::string& e){
  auto world=weak.lock();if(!world||!world->player_manager||!world->player_manager->manager()){e="Required actual PM local count";return false;}
  return world->player_manager->manager()->num_local_players(false,count,e);
 };
 layout.has_current_level=[level_weak](bool& present,std::string& e){
  dh2::loader::CanonicalCurrentLevelBorrowV1 current;if(!borrow_current_native_level_v27(current,e))return false;
  auto expected=level_weak.lock();present=current.level()!=nullptr;
  if(present&&current.level()!=expected){e="Camera centering addressed another current Level";return false;}e.clear();return true;
 };
 layout.level_slots190=[](std::array<std::uint8_t,4>&,std::string& e){
  // Captured LevelC1 has no190..193 stores. Single-local-player centering
  // returns before this read; positive multiplayer requires its real writer.
  e="Required actual multiplayer Level player-slot190 producer";return false;
 };
 const auto picking=[weak](GameplayCameraPickingV20& out,std::string& e){
  auto world=weak.lock();std::int32_t width{},height{};
  if(!world||!world->camera_application||!borrow_actual_camera_viewport_v20(width,height,e))return false;
  CameraPickingViewV20 driver;driver.viewport_width=driver.render_width=width;driver.viewport_height=driver.render_height=height;
  driver.scene_manager_present=driver.camera_present=true;return world->camera_application->picking(driver,out,e);
 };
 layout.get_screen_coord=[picking](const PointV2& p,std::array<float,2>& out,std::string& e){GameplayCameraPickingV20 source;return picking(source,e)&&source.screen_coord(p,out,e);};
 layout.get_world_coord=[picking](const std::array<float,2>& screen,float height,PointV2& out,std::string& e){
  GameplayCameraPickingV20 source;bool intersects{};
  if(!picking(source,e)||!source.world_coord(screen,height,out,intersects,e))return false;
  if(!intersects){e="Original camera ray missed source centering plane";return false;}e.clear();return true;
 };
 bindings.overview.source_vec3_zero=&dh2::world::canonical_vec3_origin_v1();
 bindings.overview.local_player0=[local,position](bool& present,PointV2& out,std::string& e){
  std::uintptr_t id{};if(!local(id,e))return false;present=id!=0;return !present||position(id,out,e);
 };
 bindings.overview.pf_z_bounds=[prepared=std::weak_ptr<dh2::loader::RetainedLevelModuleGraphV1>(candidate->preparation)](
   bool& present,float& low,float& high,std::string& e){
  auto graph=prepared.lock();auto floors=graph?graph->floor_world():nullptr;
  if(!floors||!floors->sewn){e="Required actual produced PFWorld collision bounds";return false;}
  present=true;low=floors->collision_world.bounds.minimum[2];high=floors->collision_world.bounds.maximum[2];e.clear();return true;
 };
 bindings.overview.first_gamepad=[weak](bool& present,CameraGamepadV18&,std::string& e){
  auto world=weak.lock();auto input=world&&world->application?world->application->source_input_manager_v60():nullptr;
  if(!input){e="Required SAME actual InputManager";return false;}
  const auto count=input->gamepad_count();
  for(std::int32_t i=0;i<count;++i){std::shared_ptr<const dh2::input::GamepadC1V60> pad;
   if(!input->get_gamepad(i,pad,e)||!pad)return false;
   if(pad->connected758){e="Required actual connected-gamepad Overview axis decoding";return false;}
  }
  present=false;e.clear();return true; // genuine no-connected-input loop
 };
 bindings.overview.keyboard=[](const auto&,auto&,std::string& e){e="Required original keyboard event payload";return false;};
 bindings.positive_skybox=[weak](const std::string& file,std::string& e){
  auto world=weak.lock();auto roots=world&&world->canonical_world?world->canonical_world->scene_roots_v20:nullptr;
  if(!world||!roots||!world->files_owner||!world->read){e="Required SAME campaign skybox file/root owners";return false;}
  if(!world->skybox_v124){
   SkyboxLoadServicesV24 services;services.actual_resource_owner=world->files_owner;services.same_roots=roots;
   services.read=[weak](const auto& path,auto& bytes,bool& found,auto& e){auto world=weak.lock();
    if(!world||!world->read){e="Retired actual skybox filesystem";return false;}
    return world->read(skybox_resource_uri_v124(path),found,bytes,e);
   };
   world->skybox_v124=std::make_shared<GameplaySkyboxPipelineV25>(std::move(services));
  }
  return world->skybox_v124->add(file,e);
 };
 bindings.publish_overview12c=[level_weak](std::uintptr_t id,std::string& e){auto actual=level_weak.lock();
  if(!actual||!id||actual->constructor_fields_v3().field12c){e="Invalid/replayed actual Level12c camera publication";return false;}
  actual->constructor_borrow_v3().fields->field12c=id;e.clear();return true;
 };
 bindings.publish_level128=[level_weak](std::uintptr_t id,std::string& e){auto actual=level_weak.lock();
  if(!actual||!id||actual->constructor_fields_v3().field128){e="Invalid/replayed actual Level128 camera publication";return false;}
  actual->constructor_borrow_v3().fields->field128=id;e.clear();return true;
 };
 return world->camera_application->load(input,std::move(bindings),*world->camera_session,e);
}
