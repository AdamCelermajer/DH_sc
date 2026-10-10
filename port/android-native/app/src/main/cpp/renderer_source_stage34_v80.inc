// Include inside model_renderer namespace in source_campaign_runtime_v61.cpp
// after current SourceWorldBorrow/RendererSourceCandidate/private state types.
// Add stage_loader_stage34_v1.hpp and retained_character_position_owner_v7.hpp
// to that TU. This adapter creates no World/App/PM/Character/camera/Save owner.
template<class... Args>
static std::function<bool(Args...)> live_stage34_primitive_v89(
 std::weak_ptr<SourceWorldBorrowV61> weak,
 std::function<bool(Args...)> dh2::loader::Stage34ServicesV1::* member){
 return [weak,member,busy=std::make_shared<bool>(false)](Args... args)->bool{
  auto& error=std::get<sizeof...(Args)-1>(std::tie(args...));
  if(*busy){error="Stage34 native primitive reentered its forwarding boundary";return false;}
  auto world=weak.lock();auto source=world?world->stage34_native_v80:nullptr;
  if(!source||!(source.get()->*member)){error="Required actual late-enrolled Stage34 native primitive";return false;}
  auto call=source.get()->*member;
  *busy=true;struct Guard{bool& value;~Guard(){value=false;}}guard{*busy};
  return call(std::forward<Args>(args)...);
 };
}
bool compose_source_stage33_stage34_v80(const std::shared_ptr<SourceWorldBorrowV61>& source_world,
 const std::shared_ptr<RendererSourceCandidateV55>& candidate,
 const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& level,
 const dh2::loader::EarlyLoadingDebugV46& debug,
 dh2::loader::Stage33RestoreServicesV1 native33,dh2::loader::Stage34ServicesV1 native34,
 dh2::loader::Stage33RestoreServicesV1& out33,dh2::loader::Stage34ServicesV1& out34,std::string& e){
 using namespace dh2;using namespace dh2::loader;
 if(!source_world||!candidate||!level||!source_world->application||!source_world->player_manager||!candidate->canonical){
  e="Required SAME actual completed campaign Stage33/34 owners";return false;
 }
 auto owner33=native33.owner?native33.owner:source_world->files_owner;
 auto owner34=native34.owner?native34.owner:source_world->files_owner;
 const auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
 const auto block=[](const auto& a,const auto& b){return a&&b&&!a.owner_before(b)&&!b.owner_before(a);};
 if(!owner33||!owner34||block(owner33,source_world->owner)||block(owner33,source_world->application)||block(owner33,level)||
    block(owner34,source_world->owner)||block(owner34,source_world->application)||block(owner34,level)||
    (native34.checkpoint.owner&&(block(native34.checkpoint.owner,source_world->owner)||block(native34.checkpoint.owner,source_world->application)||block(native34.checkpoint.owner,level)))){
  e="Stage33/34 providers must retain an independent genuine engine/cache authority";return false;
 }
 const std::weak_ptr<SourceWorldBorrowV61> weak=source_world;
 const std::weak_ptr<RendererSourceCandidateV55> weak_candidate=candidate;
 const std::weak_ptr<CanonicalLevelContextV1> weak_level=level;
 const auto scope_world=[weak,weak_level,weak_candidate](std::shared_ptr<SourceWorldBorrowV61>& w,std::string& e){
  w=weak.lock();auto level=weak_level.lock();auto candidate=weak_candidate.lock();
  CanonicalCurrentLevelBorrowV1 current;
  if(!w||!level||!candidate||!borrow_current_native_level_v27(current,e)||current.level()!=level){
   if(e.empty())e="Required live SAME current Level/World for Stage33/34";return false;
  }return true;
 };
 const auto manager=[scope_world,same](const std::shared_ptr<void>& app,std::shared_ptr<player::ApplicationPlayerManagerBootstrapV59>& pm,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;if(!scope_world(w,e)||!same(app,w->application))return false;
  pm=w->player_manager;
  if(!pm||!pm->belongs_to_application(w->application)||pm!=w->application->source_player_manager_v59()||!pm->manager()){
   e="Required SAME actual App40 PlayerManager owner";return false;
  }return true;
 };
 const auto check_manager=[scope_world,block](const auto& receipt,std::shared_ptr<player::ApplicationPlayerManagerBootstrapV59>& pm,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;if(!scope_world(w,e))return false;pm=w->player_manager;
  if(!pm||!pm->belongs_to_application(w->application)||pm!=w->application->source_player_manager_v59()||!pm->manager()||receipt.identity!=reinterpret_cast<std::uintptr_t>(pm->manager())||
     !block(receipt.owner,pm)){e="Foreign Stage33/34 PlayerManager receipt";return false;}return true;
 };
 const auto local=[check_manager](const auto& m,std::int32_t index,bool create,RestoreLocalPlayerBorrowV1& out,std::string& e){
  std::shared_ptr<player::ApplicationPlayerManagerBootstrapV59> pm;if(!check_manager(m,pm,e))return false;
  player::PlayerInfoFieldsV1* info{};if(!pm->get_local_player(index,create,info,e))return false;
  if(!info){e="Actual GetLocalPlayer returned NULL before660 dereference";return false;}
  out={std::shared_ptr<void>(pm,info),reinterpret_cast<std::uintptr_t>(info),&info->character660};return true;
 };
 const auto count=[check_manager](const auto& m,bool include_loading,std::int32_t& out,std::string& e){
  std::shared_ptr<player::ApplicationPlayerManagerBootstrapV59> pm;
  return check_manager(m,pm,e)&&pm->manager()->num_local_players(include_loading,out,e);
 };
 const auto record=[scope_world](std::uintptr_t id,SourceCampaignCharacterBorrowV62& out,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;if(!scope_world(w,e)||!borrow_source_campaign_character_v62(w->owner,id,out,e))return false;
  if(!out.character||!out.character->actor||!out.character->actor->object||out.character->actor->object->identity!=id){
   e="Required SAME actual retained Character source record";return false;
  }return true;
 };
 const auto character=[record](std::uintptr_t id,RestoreCharacterBorrowV1& out,std::string& e){
  SourceCampaignCharacterBorrowV62 actual;if(!record(id,actual,e))return false;
  RestoreCharacterBorrowV1 value;value.owner=actual.character;value.identity=id;
  if(!actual.character->actor->inherited_initialization_fields_v62(actual.character->actor,value.fields_v80,e))return false;
  if(!value.same_source(id)){e="Required SAME constructor-backed retained Character fields";return false;}
  out=std::move(value);return true;
 };
 const auto online=[scope_world](std::uint8_t& out,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;if(!scope_world(w,e))return false;
  auto online=w->application->get_online_loading_v55();if(!online){e="Required actual GetOnline receiver";return false;}
  out=online->byte5();return true;
 };
 const auto objects=[scope_world,same](const std::shared_ptr<void>& app,std::shared_ptr<world::CanonicalObjectManagerV1>& out,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;if(!scope_world(w,e)||!same(app,w->application)||!w->canonical_world)return false;
  out=w->canonical_world->manager_lease;if(!out||out.get()!=&w->canonical_world->manager){e="Required SAME App38 ObjectManager";return false;}return true;
 };
 native33.owner=owner33;native34.owner=owner34;
 native33.actual_application=source_world->application;native34.actual_application=source_world->application;
 native33.debug=debug;native34.debug=debug;
 // This composition runs inside bootstrap.begin/connect_candidate, AFTER
 // actual GS/Level C1 but BEFORE adopt_into fills the World adapter slots.
 // Borrow the already constructed child from that SAME retained bootstrap;
 // never replay C1 or infer Save ownership from the not-yet-filled alias.
 auto gs=source_campaign_v55&&source_campaign_v55->world==source_world?
  source_campaign_v55->bootstrap.gs():nullptr;
 auto connection=gs&&gs->connection()?gs->connection()->level_connection():nullptr;
 if(!gs||!source_campaign_v55->globals||
    !gs->owns_globals_v50(source_campaign_v55->globals)||
    !same(gs->fields().level34,level)||!same(source_campaign_v55->globals->s_level,level)||
    !connection||!connection->complete()||!same(connection->candidate(),level)){
  e="Required SAME completed bootstrap Level C1 before Stage33/34 enrollment";return false;
 }
 const auto adopted=source_world->native_level_c1_v25?*source_world->native_level_c1_v25:nullptr;
 if(adopted&&!same(adopted,connection)){
  e="World Level C1 alias differs from SAME retained bootstrap child";return false;
 }
 auto bindings=connection->bindings();
 if(!bindings){e="Required actual completed Level C1 bindings before Stage33/34 enrollment";return false;}
 auto save=bindings->save();
 const auto& save_field=level->constructor_fields_v3().save_ec;
 if((save_field||save)&&(!same(save_field,save)||
    !save->owner().ready()||save->owner().fields().level8!=reinterpret_cast<const void*>(level->identity()))){
  e="Required SAME typed LevelEC save runtime from actual C1 bindings";return false;
 }
 native33.level_save=save;
 native34.checkpoint.actual_application=source_world->application;
 if(!native34.checkpoint.owner)native34.checkpoint.owner=owner34;
 native34.checkpoint.level_save=save;
 if(!native33.online_byte5)native33.online_byte5=online;
 if(!native34.online_byte5)native34.online_byte5=online;
 if(!native34.checkpoint.online_byte5)native34.checkpoint.online_byte5=online;
 if(!native33.floor_height)native33.floor_height=[scope_world,weak_candidate](const std::array<float,3>& point,float& height,bool& found,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;auto c=weak_candidate.lock();
  auto floors=c&&c->rooms?c->rooms->world():nullptr;
  if(!scope_world(w,e)||!floors||w->source_pf_floors_v115!=floors){
   if(e.empty())e="Required SAME retained PFWorld floor geometry lease";return false;
  }
  dh2::navigation::HeightHit hit{};hit.height=height;
  const int status=dh2_nav_world_height(&hit,&floors->collision_world,point.data(),0);
  if(status<0){e="Actual PFWorld.GetFloorHeightAt failed";return false;}
  found=status!=0;if(found)height=hit.height;e.clear();return true;
 };
 if(!native33.player_manager)native33.player_manager=[manager](const auto& app,RestorePlayerManagerBorrowV1& out,std::string& e){
  std::shared_ptr<player::ApplicationPlayerManagerBootstrapV59> pm;if(!manager(app,pm,e))return false;
  auto* fields=pm->manager()->source_frame_fields_v68();if(!fields){e="Required actual PlayerManager C1 frame cells";return false;}
  out={pm,reinterpret_cast<std::uintptr_t>(pm->manager()),&fields->byte6d0,nullptr};
  // Existing Root6d4/6d8 are pointer-width projections; DO NOT cast them as
  // three floats. Genuine client restore-point production remains Main's.
  return true;
 };
 if(!native34.player_manager)native34.player_manager=[manager](const auto& app,CheckpointPlayerManagerBorrowV1& out,std::string& e){
  std::shared_ptr<player::ApplicationPlayerManagerBootstrapV59> pm;if(!manager(app,pm,e))return false;
  auto* fields=pm->manager()->source_frame_fields_v68();if(!fields){e="Required actual PM719 C1 cell";return false;}
  out={pm,reinterpret_cast<std::uintptr_t>(pm->manager()),&fields->byte719};return true;
 };
 if(!native34.checkpoint.player_manager)native34.checkpoint.player_manager=native34.player_manager;
 if(!native33.local_player)native33.local_player=local;
 if(!native34.local_player)native34.local_player=local;
 if(!native34.checkpoint.local_player)native34.checkpoint.local_player=local;
 if(!native33.local_player_count)native33.local_player_count=count;
 if(!native34.local_player_count)native34.local_player_count=count;
 if(!native33.character)native33.character=character;
 if(!native34.character)native34.character=character;
 if(!native34.checkpoint.character)native34.checkpoint.character=character;
 if(!native33.retained_set_position_v80)native33.retained_set_position_v80=[record,block](const auto& c,const float* xyz,bool destination,std::string& e){
  SourceCampaignCharacterBorrowV62 actual;if(!c.same_source(c.identity)||!record(c.identity,actual,e)||(!block(c.owner,actual.character)&&!block(c.owner,actual.character->actor))||!actual.character->position){
   if(e.empty())e="Required existing SAME retained Character position owner";return false;
  }
  // Existing source kernel consumes original pointer, preserving aliases and
  // its genuine attachment/physics/PF/visual stores. No pose copy or new base.
  return actual.character->position->set_position(xyz,destination,e);
 };
 if(!native34.controller)native34.controller=[record,block](const auto& c,Stage34ControllerV1& out,std::string& e){
  SourceCampaignCharacterBorrowV62 actual;if(!c.same_source(c.identity)||!record(c.identity,actual,e)||(!block(c.owner,actual.character)&&!block(c.owner,actual.character->actor))||!actual.character->actor->controller)return false;
  auto* locked=actual.character->actor->controller->source_locked8_v80();if(!locked){e="Required actual active Character378 controller8 lock cell";return false;}
  out={actual.character,c.identity,locked};return true;
 };
 if(!native34.camera)native34.camera=[scope_world](std::uintptr_t id,std::shared_ptr<camera::GameplayCameraRuntimeV11>& out,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;if(!scope_world(w,e)||!w->camera_application)return false;
  auto session=w->camera_application->world();auto camera=session&&session->camera?session->camera->level():nullptr;
  if(!camera||reinterpret_cast<std::uintptr_t>(camera.get())!=id){e="Required SAME source Level128 CameraRuntime";return false;}
  out=std::move(camera);return true;
 };
 if(!native34.config)native34.config=[weak_candidate](std::uintptr_t id,std::shared_ptr<world::CanonicalLevelConfigV1>& out,std::string& e){
  auto candidate=weak_candidate.lock();const auto* config=candidate&&candidate->factory&&candidate->factory->modules?candidate->factory->modules->level_config(id):nullptr;
  if(!config||config->identity()!=id||!candidate->canonical){e="Required SAME published actual LevelConfig38";return false;}
  std::int32_t key{};const world::CanonicalObjectBorrowV1* object{};bool more=candidate->canonical->manager.source_ordered_begin_v38(key,object);
  while(more&&(!object||object->identity!=id))more=candidate->canonical->manager.source_ordered_next_v38(key,key,object);
  if(!more||!object||!object->lease){e="Required actual LevelConfig canonical lease";return false;}
  out=std::shared_ptr<world::CanonicalLevelConfigV1>(object->lease,const_cast<world::CanonicalLevelConfigV1*>(config));return true;
 };
 if(source_world->stage34_native_v80&&!source_world->stage34_native_v80->config)
  source_world->stage34_native_v80->config=native34.config;
 if(!native34.scene1c)native34.scene1c=[scope_world,same](const auto& app,std::shared_ptr<world::GameObjectSceneRootRegistryV1>& out,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;if(!scope_world(w,e)||!same(app,w->application)||!w->canonical_world)return false;
  out=w->canonical_world->scene_roots_v20;if(!out){e="Required SAME actual scene1c root owner";return false;}return true;
 };
 if(!native34.objects38)native34.objects38=objects;
 if(!native34.checkpoint.objects)native34.checkpoint.objects=objects;
 if(!native34.checkpoint.make_handle)native34.checkpoint.make_handle=[scope_world](const auto* actor,target_providers::Handle16& out,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;if(!scope_world(w,e)||!w->canonical_world||!actor||!actor->shared_handle){e="Required actual published actor handle C1 source";return false;}
  return w->canonical_world->manager.get_handle(actor->shared_handle->key,out,e);
 };
 // The typed auxiliary catalog is published after bootstrap assembles this
 // body. Borrow its existing live loan at the reached GetSpawnPoint leaf.
 if(!native34.checkpoint.spawn_point)native34.checkpoint.spawn_point=[scope_world](const auto& actor,auto& out,std::string& e){
  out.reset();std::shared_ptr<SourceWorldBorrowV61> world;
  if(!scope_world(world,e))return false;
  auto bag=world->stage34_native_v80;
  if(!bag||!bag->checkpoint.spawn_point){e="Required actual late-enrolled checkpoint SpawnPoint borrower";return false;}
  auto borrow=bag->checkpoint.spawn_point;
  return borrow(actor,out,e);
 };
 if(!native33.set_use_spawn_point)native33.set_use_spawn_point=[scope_world,record,block](const auto& c,bool value,std::string& e){
  SourceCampaignCharacterBorrowV62 actual;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!c.same_source(c.identity)||!record(c.identity,actual,e)||(!block(c.owner,actual.character)&&!block(c.owner,actual.character->actor))||!scope_world(w,e))return false;
  return source_campaign_character_save_use_spawn_point_v84(w->owner,c.identity,value,e);
 };
 if(!native34.set_level_id)native34.set_level_id=[scope_world,record,block](const auto& c,std::int32_t value,std::int32_t difficulty,std::string& e){
  SourceCampaignCharacterBorrowV62 actual;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!c.same_source(c.identity)||!record(c.identity,actual,e)||(!block(c.owner,actual.character)&&!block(c.owner,actual.character->actor))||!scope_world(w,e))return false;
  return source_campaign_character_save_level_id_v84(w->owner,c.identity,value,difficulty,e);
 };
 if(!native34.checkpoint.checkpoint_fields)native34.checkpoint.checkpoint_fields=[record,block](const auto& c,CheckpointCharacterFieldsV1& out,std::string& e){
  out={};SourceCampaignCharacterBorrowV62 actual;
  if(!c.same_source(c.identity)||!record(c.identity,actual,e)||(!block(c.owner,actual.character)&&!block(c.owner,actual.character->actor)))return false;
  auto actor=actual.character->actor;
  auto* checkpoint=actor->checkpoint1468_v83();auto* save_position=actor->save_position1474_v83();
  if(!checkpoint||!save_position){e="Required actual CharacterC1-produced1468/1474 fields; no adoption fabrication";return false;}
  out={actor,c.identity,checkpoint,save_position};e.clear();return true;
 };
 // Reuse the already reconstructed checkpoint persistence and actual player
 // IsDead notification through the same campaign owners as checkpoint zones.
 auto checkpoint_services=std::make_shared<world::ZoneCollisionServicesV83>();
 const auto checkpoint=[scope_world,checkpoint_services](world::ZoneCollisionServicesV83*& out,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;if(!scope_world(w,e))return false;
  if(!checkpoint_services->provider){
   SourceCampaignCandidateBorrowV55 actual;
   if(!borrow_source_campaign_candidate_runtime_v61(actual,e)||actual.actual_world!=w->owner||
      !bind_source_campaign_zone_services_v83(actual,*checkpoint_services,e))return false;
  }
  out=checkpoint_services.get();return true;
 };
 if(!native34.checkpoint.is_dead)native34.checkpoint.is_dead=[checkpoint,record,block](const auto& c,bool& dead,std::string& e){
  SourceCampaignCharacterBorrowV62 actual;
  if(!c.same_source(c.identity)||!record(c.identity,actual,e)||
     (!block(c.owner,actual.character)&&!block(c.owner,actual.character->actor)))return false;
  world::ZoneCollisionServicesV83* services{};world::ZonePlayerBorrowV83 player;
  if(!checkpoint(services,e)||!services->player||!services->player(c.identity,player,e)||!player.dead)return false;
  return player.dead(dead,e);
 };
 if(!native34.checkpoint.character_save_checkpoint)native34.checkpoint.character_save_checkpoint=[checkpoint,record,block](const auto& c,std::string& e){
  SourceCampaignCharacterBorrowV62 actual;
  if(!c.same_source(c.identity)||!record(c.identity,actual,e)||
     (!block(c.owner,actual.character)&&!block(c.owner,actual.character->actor)))return false;
  world::ZoneCollisionServicesV83* services{};
  return checkpoint(services,e)&&services->save_player_checkpoint&&services->save_player_checkpoint(c.identity,e);
 };
 // Incoming actual PlayerLightTweaker58/Scene.update(float,bool),
 if(!native34.scene_virtual60)native34.scene_virtual60=[scope_world](const auto& roots,float delta,bool optimized,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> world;
  if(!scope_world(world,e)||!world->canonical_world||roots!=world->canonical_world->scene_roots_v20){
   if(e.empty())e="Scene update addressed another actual App scene manager";return false;
  }
  dh2::world::SceneUpdateTransportV102 transport;transport.provider=world->files_owner;
  transport.timer=[](std::uint32_t& time,std::string& error){return borrow_application_time_v68(time,error);};
  return roots->source_update_v102(delta,optimized,transport,e);
 };
 //Stage21 calls the SAME Scene.update before its compiled-batching gate.
 //Expose the composed primitive on the existing public bag as well as the
 //private Stage34 body; preserve any previously enrolled native provider.
 if(source_world->stage34_native_v80&&!source_world->stage34_native_v80->scene_virtual60)
  source_world->stage34_native_v80->scene_virtual60=native34.scene_virtual60;
 //The original material/light refresh bodies are literal leaves. Supply
 //their real behavior after validating the same actual Level receiver.
 if(!native34.update_light_set)native34.update_light_set=[scope_world,weak_level](const auto& level,bool force,std::int32_t flags,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> world;if(!scope_world(world,e)||level!=weak_level.lock())return false;
  dh2::world::source_level_update_light_set_v103(force,flags);e.clear();return true;
 };
 if(!native34.update_material)native34.update_material=[scope_world,weak_level](const auto& level,bool first,bool second,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> world;if(!scope_world(world,e)||level!=weak_level.lock())return false;
  dh2::world::source_level_update_material_v103(first,second);e.clear();return true;
 };
 if(!native34.update_fog){
  dh2::world::LevelFogServicesV103 fog;fog.provider=owner34;
  const auto get_config=native34.config;
  fog.current_config=[scope_world,weak_level,get_config](auto& out,std::string& e){
   out.reset();std::shared_ptr<SourceWorldBorrowV61> world;auto level=weak_level.lock();
   if(!scope_world(world,e)||!level)return false;
   auto fields=level->constructor_borrow_v3();if(!fields.config38){e="Required actual Level38 fog slot";return false;}
   const auto id=*fields.config38;if(!id){e.clear();return true;}
   return get_config&&get_config(id,out,e);
  };
  fog.trace_level=[scope_world](std::string& e){
   std::shared_ptr<SourceWorldBorrowV61> world;std::uint32_t ignored{};
   if(!scope_world(world,e)||!world->debug||!world->debug_files||
      dh2_character_debug_load(world->debug.get(),world->debug_files)!=1||
      dh2_character_debug_get(&ignored,world->debug.get(),"isTracingLevel",world->debug_files)!=1){
    if(e.empty())e="Required original Level fog Debug transport";return false;
   }e.clear();return true;
  };
  fog.scene=[scope_world](auto& out,std::string& e){
   std::shared_ptr<SourceWorldBorrowV61> world;if(!scope_world(world,e)||!world->canonical_world)return false;
   out=world->canonical_world->scene_roots_v20;return bool(out);
  };
  native34.update_fog=[scope_world,weak_level,fog=std::move(fog)](const auto& level,std::uintptr_t,std::string& e){
   std::shared_ptr<SourceWorldBorrowV61> world;if(!scope_world(world,e)||level!=weak_level.lock())return false;
   return dh2::world::source_level_update_fog_v103(fog,e);
  };
 }
 // These primitives may be published after the Stage34 body is assembled.
 // Forward to the live native provider; never to this composed service copy.
 if(!native34.handle_no_room_objects)native34.handle_no_room_objects=
  live_stage34_primitive_v89(weak,&Stage34ServicesV1::handle_no_room_objects);
 if(!native34.fog_params58)native34.fog_params58=
  live_stage34_primitive_v89(weak,&Stage34ServicesV1::fog_params58);
 out33=std::move(native33);out34=std::move(native34);e.clear();return true;
}

bool bind_source_stage33_stage34_v80(const std::shared_ptr<SourceWorldBorrowV61>& world,
 const std::shared_ptr<RendererSourceCandidateV55>& candidate,
 const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& level,
 const dh2::loader::EarlyLoadingDebugV46& debug,
 dh2::loader::LifecycleServicesV36& loading,std::string& e){
 using namespace dh2::loader;
 if(!world||loading.stage_body[33]||loading.stage_body[34]){e="Stage33/34 source bodies require sole existing loader binding";return false;}
 Stage33RestoreServicesV1 native33=world->stage33_native_v80?*world->stage33_native_v80:Stage33RestoreServicesV1{};
 Stage34ServicesV1 native34=world->stage34_native_v80?*world->stage34_native_v80:Stage34ServicesV1{};
 Stage33RestoreServicesV1 services33;Stage34ServicesV1 services34;
 if(!compose_source_stage33_stage34_v80(world,candidate,level,debug,std::move(native33),std::move(native34),services33,services34,e))return false;
 // Modern binding is atomic; no native primitive or stage is invoked here.
 // Existing source callbacks outside33/34 remain on the original dispatcher.
 LifecycleServicesV36 pending;
 if(!bind_stage33_restore_v1(pending,level,std::move(services33),e)||!bind_stage34_v1(pending,level,std::move(services34),e))return false;
 loading.stage_body[33]=std::move(pending.stage_body[33]);loading.stage_body[34]=std::move(pending.stage_body[34]);return true;
}
