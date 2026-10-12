// Include in model_renderer namespace after actual WorldScriptContext,
// RendererCharacterCandidateV60 and campaign lookup definitions. Required
// headers are listed in HANDOFF. This builds services/graphs, never a class C1.
struct RendererCampaignNonCharacterInputsV69 {
 // Existing genuine source methods, decoded tables and native release journal.
 // Root composition below replaces only services whose real owners it binds.
 dh2::loader::ProductionNonCharacterServicesV67 source;
 dh2::world::ConditionDataInitServicesV3 conditions;
 std::shared_ptr<dh2::world::NativeConditionRuntimeV69> condition_runtime;
 std::shared_ptr<dh2::world::LightSetNameOwnerV3> light_names;
 dh2::loader::LevelPlayerPlacementServicesV68 placement;
 dh2::world::RetainedGameObjectDecorServicesV1 decor_native;
 dh2::world::CanonicalZonePhysicalServicesV82 zone_physical; // genuine peer/contact virtual leaves
 dh2::world::ZoneCollisionServicesV83 zone_collision; // actual public V83 transport plus incoming mesh/peer leaves
 std::vector<std::string> required_types;
 std::function<bool(dh2::loader::ProductionNonCharacterOwnersV67&,std::string&)> after_catalog_composition_v89;
 std::function<bool(dh2::world::CanonicalGameObjectBaseOwnerV1&,std::string&)> failed_spawn_mark;
 std::function<bool(dh2::world::CanonicalGameObjectBaseOwnerV1&,dh2::loader::GameObjectSourceFrameServicesV74&,std::string&)> lend_frame_services;
 CampaignGenericRuntimeReceiverServicesV69 runtime_receiver;
 dh2::loader::GameObjectArraysOwnerV81::Borrow declaration_arrays;
 // Real native facets not exported by generic GraphV68: no raw pointer casts.
 std::function<bool(dh2::world::CanonicalGameObjectBaseOwnerV1&,std::shared_ptr<void>&,
  dh2::physical::NativeBody*&,std::string&)> borrow_native_body;
 std::function<bool(dh2::world::CanonicalGameObjectBaseOwnerV1&,std::uintptr_t,float*&,std::string&)> attached_position;
};
struct RendererCampaignNonCharacterV69 final:std::enable_shared_from_this<RendererCampaignNonCharacterV69> {
 dh2::loader::ScopeV67 scope;
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 std::weak_ptr<dh2::floors::World> floors;
 std::weak_ptr<dh2::navigation::CampaignNavigationRegistryV64> navigation;
 std::weak_ptr<dh2::physical::NativeWorld> physics;
 std::unique_ptr<dh2::character::LootPhysicalAssociationsV49> physical_associations;
 std::map<std::uintptr_t,std::shared_ptr<dh2::world::CanonicalDecorPhysicalConnectionV49>> decor_connections;
 std::map<std::uintptr_t,std::shared_ptr<dh2::world::RetainedGenericAnimatorV21>> animators;
 std::weak_ptr<dh2::world::ModulePFRoomsV3> rooms;
 std::weak_ptr<dh2::world::SceneManagerMapOwnerV2> map;
 std::weak_ptr<dh2::loader::CanonicalReceiverTransportV1> transport;
 std::shared_ptr<dh2::android_ui::OriginalCacheAssetsV1> files; // independent APK/cache authority
 std::shared_ptr<dh2::world::CanonicalGameObjectGraphV68> graph;
 RendererCampaignNonCharacterInputsV69 native;
 // Weak constructor-record borrows only. Authentic Root journal/factory owns C1.
 std::vector<dh2::world::CanonicalBaseBorrowV68> borrows;
 std::vector<std::weak_ptr<dh2::loader::AuxiliaryReleaseRecordV67>> auxiliary_journal;
 std::map<std::uintptr_t,std::weak_ptr<dh2::loader::AuxiliaryResourcePrefixV82>> zone_resources;
 struct Bound {
  dh2::world::GameObjectInitializationServicesV1 initialization;
  dh2::world::GameObjectSetPositionServicesV2 position;
  std::shared_ptr<dh2::world::ConditionDataBindingV72> conditions;
 };
 std::map<std::uintptr_t,Bound> bound;
 std::set<std::uintptr_t> external_visuals;
 std::map<std::uintptr_t,std::weak_ptr<dh2::world::CanonicalOpenableGraphV21>> openable_graphs_v89;
 bool current(std::string& e)const{
  SourceCampaignCandidateBorrowV55 actual;if(!borrow_source_campaign_candidate_v55(actual,e))return false;
  auto world=scope.actual_world.lock();auto level=scope.level.lock();auto objects=scope.objects.lock();
  if(!world||!level||!objects||actual.actual_world.get()!=world.get()||actual.level.get()!=level.get()||actual.objects.get()!=objects.get()||actual.properties!=scope.properties||
   world.owner_before(actual.actual_world)||actual.actual_world.owner_before(world)||level.owner_before(actual.level)||actual.level.owner_before(level)||objects.owner_before(actual.objects)||actual.objects.owner_before(objects)){
   e="Non-Character actual source candidate/manager lifetime changed";return false;
  }return true;
 }
 bool borrow(std::uintptr_t id,std::shared_ptr<void>& pin,dh2::world::CanonicalGameObjectBaseOwnerV1*& base,std::string& e)const{
  pin.reset();base=nullptr;if(!current(e))return false;
  for(const auto& actual:borrows){std::shared_ptr<void> candidate;dh2::world::CanonicalGameObjectBaseOwnerV1* value{};std::string ignored;
   if(actual(candidate,value,ignored)&&candidate&&value&&value->identity()==id){pin=std::move(candidate);base=value;return true;}}
  e="Required SAME admitted canonical non-Character C1/runtime borrow";return false;
 }
 template<class Record>void admit_weak(const std::shared_ptr<Record>& record){std::weak_ptr<Record> weak=record;std::weak_ptr<RendererCampaignNonCharacterV69> self=shared_from_this();
  borrows.push_back([weak,self](auto& pin,auto*& base,std::string& e){auto owner=self.lock();auto r=weak.lock();if(!owner||!owner->current(e)||!r||!r->owner){if(e.empty())e="Unproduced/released actual auxiliary C1";return false;}pin=r;base=&r->owner->base();return true;});
 }
 bool admit_auxiliary(const std::shared_ptr<dh2::loader::AuxiliaryReleaseRecordV67>& r,std::string& e){
  if(!r||!r->record||!r->class_name){e="Required actual auxiliary pre-C1 release record";return false;}
  // Main receives this continuation at initial journal admission. Completed
  // authentic class destruction is never replayed for a retained resource.
  auto destroy=r->destroy_source;auto complete=std::make_shared<bool>(false);
  std::weak_ptr<dh2::loader::AuxiliaryReleaseRecordV67> journal=r;
  r->destroy_source=[journal,destroy,complete](std::string& e){auto r=journal.lock();
   if(!r||!destroy){e="Required authentic auxiliary destruction continuation";return false;}
   if(!*complete){if(!destroy(e))return false;*complete=true;}
   for(const auto& prefix:r->resources_v82)if(prefix&&!prefix->released){e="Required qualified native resource D0 before auxiliary journal retirement";return false;}
   e.clear();return true;};
  auxiliary_journal.push_back(r);
#define ACTUAL_RECORD(name,type) if(!std::strcmp(r->class_name,name)){admit_weak(std::static_pointer_cast<dh2::loader::type>(r->record));return true;}
  ACTUAL_RECORD("Dummy",CanonicalDummyRecordV16) ACTUAL_RECORD("SpawnPoint",CanonicalSpawnPointRecordV16)
  ACTUAL_RECORD("Decor",CanonicalDecorRecordV16) ACTUAL_RECORD("AnimatedDecor",CanonicalAnimatedDecorRecordV23)
  ACTUAL_RECORD("TriggerZone",CanonicalTriggerZoneRecordV22) ACTUAL_RECORD("CheckpointZone",CanonicalCheckpointRecordV26)
  ACTUAL_RECORD("Door",CanonicalDoorRecordV27) ACTUAL_RECORD("TriggerObject",CanonicalTriggerObjectRecordV28)
  ACTUAL_RECORD("TriggerZoneExitLevel",CanonicalExitZoneRecordV29) ACTUAL_RECORD("QuestMoveInZone",CanonicalQuestMoveRecordV31)
  ACTUAL_RECORD("SoundEmitter",CanonicalSoundEmitterRecordV32)
#undef ACTUAL_RECORD
  e="Auxiliary journal is not an actual admitted V67 class record";return false;
 }
 // Main calls at actual physical2dc D0, before class/PhysicsWorld retirement.
 bool release_zone_physical_v82(std::uintptr_t identity,bool& handled,std::string& e){
  handled=false;auto found=zone_resources.find(identity);if(found==zone_resources.end())return true;
  auto prefix=found->second.lock();if(!prefix){e="Retired POZone prefix before qualified release";return false;}
  handled=true;if(prefix->released){e.clear();return true;}
  if(!prefix->release_source){e="Required actual POZone D0 continuation";return false;}
  return prefix->release_source(e);
 }
 bool zone_prefixes_released(std::uintptr_t id,std::string& e)const{
  for(const auto& weak:auxiliary_journal){auto r=weak.lock();if(!r||!r->identity||r->identity()!=id)continue;
   for(const auto& prefix:r->resources_v82)if(prefix&&!prefix->released){e="Unreleased POZone prefix blocks actual C1 unpublication/retirement";return false;}}
  return true;
 }
 bool create_zone_physical_v82(dh2::world::CanonicalGameObjectBaseOwnerV1& supplied,bool trigger,std::string& e){
  std::shared_ptr<void> receiver;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  if(!borrow(supplied.identity(),receiver,base,e)||base!=&supplied)return false;
  std::shared_ptr<dh2::loader::AuxiliaryReleaseRecordV67> journal;
  for(const auto& weak:auxiliary_journal){auto r=weak.lock();if(r&&r->identity&&r->identity()==base->identity()){journal=std::move(r);break;}}
  auto world=physics.lock();if(!journal||!world){e="Required SAME admitted auxiliary journal and native PhysicalWorld";return false;}
  auto services=native.zone_physical;
  if(!services.owner||!services.peer_owner||!services.peer_visible80||!services.resolve_peer_handle||
     !services.collision_test_notification||!services.contact){e="Required actual POZone peer/handle/void notification/contact service owner";return false;}
  auto actual_world=scope.actual_world.lock();auto level=scope.level.lock();auto objects=scope.objects.lock();
  const auto same_owner=[&](const auto& containing){return containing&&!services.owner.owner_before(containing)&&!containing.owner_before(services.owner);};
  if(same_owner(actual_world)||same_owner(level)||same_owner(objects)){e="POZone services must retain independent authority and borrow containing scope weakly";return false;}
  std::weak_ptr<RendererCampaignNonCharacterV69> weak=shared_from_this();
  if(!services.debug_switch)services.debug_switch=[weak](const char* key,bool& value,std::string& e){auto self=weak.lock();
   auto actual=self?std::static_pointer_cast<WorldScriptContext>(self->scope.actual_world.lock()):nullptr;std::uint32_t result{};
   if(!self||!self->current(e)||!actual||!actual->debug||dh2_character_debug_load(actual->debug.get(),&actual->debug_files)!=1||
      dh2_character_debug_get(&result,actual->debug.get(),key,&actual->debug_files)!=1){if(e.empty())e="Required SAME source Debug Zone physical prefix";return false;}
   value=result!=0;return true;};
  if(!services.update_pf)services.update_pf=[weak](auto& base,std::string& e){auto self=weak.lock();return self&&self->pf(base,false,nullptr,0,e);};
  auto previous=services.destroy_previous;
  services.destroy_previous=[weak,previous](std::uintptr_t id,std::string& e){auto self=weak.lock();bool handled{};
   if(!self||!self->release_zone_physical_v82(id,handled,e))return false;if(handled)return true;
   if(!previous){e="Required actual previous non-Zone PhysicalObject D0";return false;}return previous(id,e);};
  // Wrapper construction has no native body. Journal admission precedes its
  // first native construct/assign and retains every failed source prefix.
  auto zone=std::make_shared<dh2::world::CanonicalZonePhysicalV82>(*base,*world,receiver,std::move(services));
  auto prefix=std::make_shared<dh2::loader::AuxiliaryResourcePrefixV82>();prefix->backing=world;prefix->receiver=zone;
  prefix->identity=reinterpret_cast<std::uintptr_t>(zone.get());prefix->source_owner8=zone->source_owner8();
  std::weak_ptr<dh2::world::CanonicalZonePhysicalV82> actual=zone;
  std::weak_ptr<dh2::loader::AuxiliaryResourcePrefixV82> state=prefix;
  prefix->release_source=[actual,state](std::string& e){auto zone=actual.lock();auto prefix=state.lock();
   if(!zone||!prefix){e="Released SAME POZone prefix";return false;}if(prefix->released){e.clear();return true;}
   if(!zone->release(e))return false;prefix->assigned=false;prefix->released=true;return true;};
  journal->resources_v82.push_back(prefix);zone_resources[prefix->identity]=prefix;
  if(!zone->construct(trigger,e))return false;
  const bool assigned=zone->assign(e);prefix->assigned=zone->assigned();
  if(assigned&&!prefix->assigned)prefix->released=true; // MP_NoPhysics actual fresh-body D0
  return assigned;
 }
 bool bind_visual_collision_zone_v82(dh2::world::CanonicalGameObjectBaseOwnerV1& supplied,const char* name,
  std::uintptr_t& node,std::shared_ptr<void>& lease,std::string& e){
  std::shared_ptr<void> receiver;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  return borrow(supplied.identity(),receiver,base,e)&&base==&supplied&&bind_retained_colzone_v91(*this,supplied,name,node,lease,e);
 }
 void bind_zone_startup_v82(dh2::world::ZoneStartupServicesV76& output){
  std::weak_ptr<RendererCampaignNonCharacterV69> weak=shared_from_this();output.owner=shared_from_this();
  if(!output.create_zone_physical)output.create_zone_physical=[weak](auto& base,bool trigger,std::string& e){auto self=weak.lock();return self&&self->create_zone_physical_v82(base,trigger,e);};
  if(!output.bind_visual_collision_zone)output.bind_visual_collision_zone=[weak](auto& base,const char* name,auto& node,auto& lease,std::string& e){auto self=weak.lock();return self&&self->bind_visual_collision_zone_v82(base,name,node,lease,e);};
 }
 void bind_trigger_zone_resources_v82(std::uintptr_t id,dh2::world::TriggerZoneServicesV22& output){
  std::weak_ptr<RendererCampaignNonCharacterV69> weak=shared_from_this();
  auto get=[weak,id](auto& pin,auto*& base,std::string& e){auto self=weak.lock();return self&&self->borrow(id,pin,base,e);};
  if(!output.spawn_roll_probability)output.spawn_roll_probability=container_spawn_roll_v89(get);
  if(!output.set_bounding_box)output.set_bounding_box=[weak,get](const std::array<float,6>& box,bool absolute,std::string& e){
   auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};Bound* bound{};
   if(!self||!get(pin,base,e)||!self->ensure(*base,bound,e))return false;
   if(absolute){e="Required original Trigger Zone relative bounding box";return false;}
   return dh2::world::game_object_relative_box_v3(*base,box.data(),bound->initialization.update_pf_object,e);};
  if(!output.create_zone_physical)output.create_zone_physical=[weak,id](bool trigger,std::string& e){auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};return self&&self->borrow(id,pin,base,e)&&self->create_zone_physical_v82(*base,trigger,e);};
  if(!output.bind_visual_collision_zone)output.bind_visual_collision_zone=[weak,id](const char* name,auto& node,auto& lease,std::string& e){auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};return self&&self->borrow(id,pin,base,e)&&self->bind_visual_collision_zone_v82(*base,name,node,lease,e);};
 }
 bool clear_conditions_at_object_base_dtor(std::uintptr_t id,std::string& e){auto found=bound.find(id);if(found==bound.end()||!found->second.conditions){e.clear();return true;}return found->second.conditions->clear(e);}
 // Qualified visual cleanup only; authentic class/base D0 and manager/transport
 // unpublication remain the incoming real journal's source order.
 bool release_graph_resources(std::uintptr_t id,std::string& e){
  const auto physical=decor_connections.find(id);if(physical!=decor_connections.end()){if(!physical->second->release(e))return false;decor_connections.erase(physical);}
  std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};if(!borrow(id,pin,base,e))return false;
  const auto visual_id=*base->pointer(0x2d8);const auto animated=animators.find(visual_id);if(animated!=animators.end()){if(!animated->second->set_callbacks({},e))return false;animators.erase(animated);}
  if(external_visuals.count(id)){if(!graph->retire_observed_visual(id,e))return false;external_visuals.erase(id);return true;}
  return graph->release_resources(id,e);
 }
 struct RuntimeLease {
  std::shared_ptr<void> receiver,physical,target;
  dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  std::weak_ptr<RendererCampaignNonCharacterV69> owner;
  std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;
  CampaignGenericRuntimeEnvironmentV69 environment;
  CampaignGenericRuntimeReceiverServicesV69 methods;
  CampaignAuxiliaryRuntimeBorrowV69 auxiliary;
  CampaignCameraRuntimeBorrowV69 camera;
  const float* target_position{};
  dh2::actor::RuntimePolicy policy{};
  dh2::subobjects::Services subobjects{};
  dh2::subobjects::AuxiliaryBranchServicesV69 auxiliary_source{};
  dh2::navigation::ControllerSourceServicesV69 path_source{};
  dh2::actor::GenericTargetPositionServicesV80 target_source{};
  std::string source_error;
  bool live(){auto self=owner.lock();return self&&self->current(source_error);}
  bool camera_borrow(){camera={};if(!methods.camera||!methods.camera(camera,source_error)||!camera.receiver||!camera.enabled25){if(source_error.empty())source_error="Required SAME actual campaign camera resource";return false;}return live();}
  static int borrow_target_position(void* opaque,const float** out){
   auto& same=*static_cast<RuntimeLease*>(opaque);if(!out||!same.live())return 1;auto slot=same.base->pointer(0x180);
   if(!slot){same.source_error="Required actual target180 cell at target phase";return 1;}
   if(!*slot){same.target.reset();*out=nullptr;return 0;}
   std::shared_ptr<void> pin;const float* position{};
   if(!same.methods.target_position||!same.methods.target_position(*same.base,*slot,pin,position,same.source_error)||!pin||!position)return 1;
   same.target=std::move(pin);*out=position;return 0;
  }
  static int borrow_auxiliary_branch(void* opaque,dh2::subobjects::AuxiliaryBranchV69* out){
   auto& same=*static_cast<RuntimeLease*>(opaque);if(!out||!same.live())return 1;
   auto slot=same.base->pointer(0x2e0);CampaignAuxiliaryRuntimeBorrowV69 current;
   if(!slot||!*slot||!same.methods.auxiliary||!same.methods.auxiliary(*same.base,*slot,current,same.source_error)||!same.live()||!current.receiver||current.identity!=*slot||!current.type4||(*current.type4==2&&(!current.mode3c||!current.position_c))){if(same.source_error.empty())same.source_error="Required fresh SAME auxiliary2e0 branch after Update";return 1;}
   // Retain this genuine fresh owner through subsequent camera deliveries; the
   // delta reads actual position_c after camera_position, not a copied pose.
   same.auxiliary=std::move(current);out->type=*same.auxiliary.type4;out->mode=out->type==2?*same.auxiliary.mode3c:0;out->position_c=same.auxiliary.position_c;return 0;
  }
  static int borrow_skip_boundary(void* opaque,std::uint32_t* out){
   auto& same=*static_cast<RuntimeLease*>(opaque);auto self=same.owner.lock();if(!out||!self||!same.live())return 1;
   auto world=std::static_pointer_cast<WorldScriptContext>(self->scope.actual_world.lock());std::uint32_t disabled{};
   if(!world||!world->debug||dh2_character_debug_load(world->debug.get(),&world->debug_files)!=1||dh2_character_debug_get(&disabled,world->debug.get(),"DisableWallAvoidance",&world->debug_files)!=1||!same.live()){if(same.source_error.empty())same.source_error="Required source Debug.DisableWallAvoidance at reached boundary";return 1;}
   *out=disabled!=0;return 0;
  }
  static std::uint32_t invoke(void* opaque,std::uint32_t event,float* values){
   auto& same=*static_cast<RuntimeLease*>(opaque);using namespace dh2::subobjects;
   constexpr auto failure=UINT32_MAX;if(!same.live())return failure;
   switch(event){
    case visual_update: // actual VisualObject.Update470cf0 is literal BX LR
    case visual_apply_rotation: // actual470ccc is literal BX LR
     return 1;
    case get_speed: // actual GameObject.GetSpeed3400e4 observer (literal4)
     if(values)*values=4.f;return 1;
    case visual_sync_scaling:
     if(!same.visual)return 1;
     return dh2::loader::retained_visual_sync_scaling_v69(*same.visual,same.base->vector3(0x120),same.source_error)?1:failure;
    case auxiliary_update:{
     auto slot=same.base->pointer(0x2e0);auto& aux=same.auxiliary;
     if(!slot||*slot!=aux.identity||!aux.receiver||!aux.update){same.source_error="Required live SAME auxiliary2e0 virtual0c receiver";return failure;}
     if(!aux.update(same.source_error)||!same.live())return failure;return 1;
    }
    case camera_get:
     if(!values||!same.camera_borrow())return failure;values[0]=*same.camera.enabled25!=0?1.f:0.f;return 1;
    case camera_position:
     if(!values||!same.camera.receiver||!same.camera.position||!same.camera.position(values,same.source_error)||!same.live())return failure;return 1;
    case camera_set_free:
     if(!values||!same.camera.receiver||!same.camera.set_free||!same.camera.set_free(values[0]!=0.f,same.source_error)||!same.live())return failure;return 1;
    default:same.source_error="Unexpected external generic V67 subobject event "+std::to_string(event);return failure;
   }
  }
 };
 bool lend_runtime(dh2::world::CanonicalGameObjectBaseOwnerV1& supplied,dh2::loader::GameObjectRuntimeBorrowV74& out,std::string& e){
  std::shared_ptr<void> receiver;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  if(!borrow(supplied.identity(),receiver,base,e)||base!=&supplied)return false;
  auto lease=std::make_shared<RuntimeLease>();lease->receiver=receiver;lease->base=base;lease->owner=shared_from_this();lease->methods=native.runtime_receiver;
  SourceCampaignCandidateBorrowV55 actual;if(!borrow_source_campaign_candidate_v55(actual,e))return false;
  const auto& path=base->runtime().path;
  if(path.count>path.capacity||(path.count&&!path.segments)){e="Malformed SAME retained source path before environment delivery";return false;}
  // Scratch copies exactly PathObject.count segments. All supported V67 source
  // virtual7c bodies return0: no avoidance actor scratch is consumed. The actor
  // kernel's one-self Stop view is independent of avoidance workspace counts.
  if(!borrow_source_campaign_frame_environment_v76(actual,path.count,0,lease->environment,e)||!current(e))return false;
  auto& environment=lease->environment;auto geometry=floors.lock();auto registry=navigation.lock();
  if(!environment.actual_scope||!geometry||!registry||environment.geometry!=&geometry->collision_world||environment.graph!=&geometry->graph||environment.registry!=&registry->registry()||!environment.motion_policy||!environment.workspace){
   e="Required SAME concrete V76 campaign frame environmental receipt";return false;
  }  const auto* physical=base->pointer(0x2dc);const auto* visual=base->pointer(0x2d8);const auto* auxiliary=base->pointer(0x2e0);const auto* target=base->pointer(0x180);
  if(!physical||!visual||!auxiliary||!target){e="Required SAME produced physical/visual/auxiliary/target runtime cells";return false;}
  dh2::loader::GameObjectRuntimeBorrowV74 prepared{};auto& request=prepared.request;auto& frame=request.actor;
  if(*physical&&!physical_body(*base,lease->physical,frame.native_body,e))return false;
  if(*physical&&(!lease->physical||!frame.native_body)){e="Unretained actual runtime physical2dc";return false;}
  if(*visual&&!actual_visual(*visual,lease->visual,e))return false;
  if(*target&&(!lease->methods.owner||!lease->methods.target_position)){e="Required actual target180 late cache borrower";return false;}
  if(*auxiliary){
   auto& aux=lease->auxiliary;if(!lease->methods.owner||!lease->methods.auxiliary||!lease->methods.auxiliary(*base,*auxiliary,aux,e)||!aux.receiver||aux.identity!=*auxiliary||!aux.update){if(e.empty())e="Required SAME actual auxiliary2e0 source method/cells";return false;}
   lease->policy.has_auxiliary=1; // actual presence; fresh post-Update source continuation supplies branch cells
  }
  // V67 source vtables share exact34005c..3400a4 bodies. Presence/absence is
  // taken from the same produced source pointer cells, not default callbacks.
  request.actual_virtual_policy={0,1,0,0,1,1,base->lifecycle().static84?2u:0u};request.actual_rotation_speed=-0.75f;
  lease->policy.path={1,0,0,static_cast<std::uint32_t>(bool(*physical))}; // source_path supplies reached Debug fact
  lease->policy.validating_camera=0;lease->policy.virtual_speed=4.f;
  lease->subobjects={lease.get(),RuntimeLease::invoke};
  lease->path_source={lease.get(),RuntimeLease::borrow_skip_boundary};request.source_path=&lease->path_source;
  lease->auxiliary_source={lease.get(),RuntimeLease::borrow_auxiliary_branch};request.source_auxiliary=&lease->auxiliary_source;
  lease->target_source={lease.get(),RuntimeLease::borrow_target_position};request.source_target=&lease->target_source;
  frame.state=&base->runtime();frame.key=base->identity();frame.geometry=environment.geometry;frame.graph=environment.graph;frame.registry=environment.registry;
  frame.motion_policy=environment.motion_policy;frame.workspace=environment.workspace;frame.avoidance=nullptr; // recovered virtual7c=0; actor kernel supplies SAME self Stop view
  frame.policy=&lease->policy;frame.services=&lease->subobjects;frame.target_absolute_position=lease->target_position;frame.dt_ms=environment.dt_ms;
  if(lease->visual&&lease->visual->root_identity()){
   request.visual.context=lease.get();request.visual.root=&lease->visual->binding().root;
   request.visual.update_world=[](void* opaque,std::string& e){auto& same=*static_cast<RuntimeLease*>(opaque);return same.live()&&same.visual->binding().update_world(same.visual->scene(),e);};
   request.visual.set_rotation=[](void* opaque,const float* p,std::string& e){auto& same=*static_cast<RuntimeLease*>(opaque);return same.live()&&dh2::loader::retained_visual_sync_rotation_v69(*same.visual,p,e);};
  }
  if(!current(e))return false;prepared.actual_scope=std::move(lease);out=std::move(prepared);e.clear();return true;
 } bool set_object_position(std::uintptr_t id,const float* p,bool destination,std::string& e){
  if(!p||!current(e))return false;SourceCampaignCharacterBorrowV62 character;
  auto world=scope.actual_world.lock();if(!borrow_source_campaign_character_v62(world,id,character,e))return false;
  return character.character->set_position({p[0],p[1],p[2]},destination,e);
 }
 bool set_object_rotation(std::uintptr_t id,const float* p,std::string& e){
  if(!p||!current(e))return false;SourceCampaignCharacterBorrowV62 character;
  auto world=scope.actual_world.lock();if(!borrow_source_campaign_character_v62(world,id,character,e))return false;
  const auto record=character.character;const auto actor=record?record->actor:nullptr;
  if(!actor||!actor->machine){e="Required SAME source Character520 for SpawnPoint placement";return false;}
  dh2::world::GameObjectInitializationFieldsV62 fields;if(!actor->inherited_initialization_fields_v62(actor,fields,e))return false;
  dh2::loader::GameObjectSetRotationBorrowV69 b;b.receiver=record;b.rotation16c=fields.vector3(0x16c);b.heading178=fields.scalar(0x178);b.visual2d8=fields.pointer(0x2d8);
  // Character virtual70=3a2e68 reads the SAME FSM State56.flags520 bit4.
  b.updating_visual_with_rotation=[record](bool& enabled,std::string& e){if(!record->actor||!record->actor->machine){e="Released SAME Character flags520";return false;}enabled=(record->actor->machine->state().flags&0x10u)==0;e.clear();return true;};
  b.sync_rotation=[record](std::uintptr_t id,const float* rotation,std::string& e){auto visual=record->visual?record->visual->visual():nullptr;
   dh2::world::GameObjectInitializationFieldsV62 live;if(!record->actor||!record->actor->inherited_initialization_fields_v62(record->actor,live,e))return false;
   auto slot=live.pointer(0x2d8);if(!slot||*slot!=id||!visual||reinterpret_cast<std::uintptr_t>(visual.get())!=id){e="Required SAME retained Character VisualObject2d8";return false;}
   return dh2::loader::retained_visual_sync_rotation_v69(*visual,rotation,e);};
  return dh2::loader::game_object_set_rotation_v69(b,p,e);
 }
 bool source_visibility(dh2::world::CanonicalGameObjectBaseOwnerV1& base,std::string& e){
  const auto id=*base.pointer(0x2d8);if(!id)return true;
  std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;if(!graph->borrow_visual(id,visual,e))return false;
  auto& f=base.lifecycle();if(!f.visible_written){e="Actual non-Character visible80 has no source producer";return false;}
  bool visible=f.visible80!=0;if(visible&&!f.non_zonable2ed&&f.zoning2ee&&!f.entered2f0)visible=false;
  return visual->set_root_local_visibility_v3(visible,e);
 }
 bool physical_body(dh2::world::CanonicalGameObjectBaseOwnerV1& base,std::shared_ptr<void>& pin,dh2::physical::NativeBody*& body,std::string& e){
  const auto* slot=base.pointer(0x2dc);const auto found=slot?zone_resources.find(*slot):zone_resources.end();
  if(found!=zone_resources.end()){auto prefix=found->second.lock();if(!prefix||!prefix->receiver||prefix->released){e="Released SAME Zone native facet";return false;}
   auto zone=std::static_pointer_cast<dh2::world::CanonicalZonePhysicalV82>(prefix->receiver);std::shared_ptr<void> receiver;
   if(!zone->borrow_native(receiver,body,e))return false;
   struct FacetPin {std::shared_ptr<dh2::loader::AuxiliaryResourcePrefixV82> prefix;std::shared_ptr<void> receiver;};
   pin=std::make_shared<FacetPin>(FacetPin{prefix,std::move(receiver)});return true;
  }
  const auto openable=openable_graphs_v89.find(base.identity());if(openable!=openable_graphs_v89.end()){
   auto graph=openable->second.lock();if(!graph){e="Released SAME Openable physical owner";return false;}
   return graph->borrow_native_body_v69(*base.pointer(0x2dc),pin,body,e);
  }
  const auto at=decor_connections.find(base.identity());if(at!=decor_connections.end())return at->second->borrow_native_body(*base.pointer(0x2dc),pin,body,e);
  if(!native.borrow_native_body){e="Required actual non-Decor native physical facet borrower";return false;}return native.borrow_native_body(base,pin,body,e);
 }
 template<class Getter,class Member>auto inherited_container_call_v89(Getter get,Member member){
  std::weak_ptr<RendererCampaignNonCharacterV69> weak=shared_from_this();
  return [weak,get,member](auto&&... args)->bool{auto& e=std::get<sizeof...(args)-1>(std::forward_as_tuple(args...));
   auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};Bound* bound{};
   if(!self||!get(pin,base,e)||!base||!self->ensure(*base,bound,e))return false;
   const auto& callback=bound->initialization.*member;if(!callback){e="Required genuine reached inherited container initialization leaf";return false;}
   return callback(std::forward<decltype(args)>(args)...);};
 }
 template<class Getter>void bind_container_initialization_v89(Getter get,dh2::world::GameObjectInitializationServicesV1& init){
  init.difficulty_names=native.source.auxiliary.difficulty_names.get();init.sound_names=native.source.auxiliary.sound_names.get();
#define INHERITED_CONTAINER(field) if(!init.field)init.field=inherited_container_call_v89(get,&dh2::world::GameObjectInitializationServicesV1::field)
  INHERITED_CONTAINER(condition_init);INHERITED_CONTAINER(check_spawn_probability);INHERITED_CONTAINER(device_high_performance);
  INHERITED_CONTAINER(load_visual);INHERITED_CONTAINER(set_position);INHERITED_CONTAINER(visual_sync);INHERITED_CONTAINER(set_visible);
  INHERITED_CONTAINER(init_pf_object);INHERITED_CONTAINER(update_pf_object);INHERITED_CONTAINER(light_set_id);
  INHERITED_CONTAINER(visual_set_light_set);INHERITED_CONTAINER(visual_root);INHERITED_CONTAINER(node_from_name);
#undef INHERITED_CONTAINER
 }
 template<class Getter>auto container_spawn_roll_v89(Getter get){
  std::weak_ptr<RendererCampaignNonCharacterV69> weak=shared_from_this();
  return [weak,get](std::int32_t& roll,std::int32_t& probability,std::string& e){auto self=weak.lock();
   std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};Bound* bound{};
   if(!self||!get(pin,base,e)||!base||!self->ensure(*base,bound,e)||!bound->initialization.check_spawn_probability||
      !bound->initialization.check_spawn_probability(roll,e))return false;
   auto value=base->integer(0x274);if(!value){e="Required SAME actual probability274 reread";return false;}probability=*value;return true;};
 }
 bool actual_visual(std::uintptr_t visual,std::shared_ptr<dh2::world::RetainedGameObjectVisualV1>& out,std::string& e){return current(e)&&graph->borrow_visual(visual,out,e);}
 bool animator(std::uintptr_t visual,std::shared_ptr<dh2::world::RetainedGenericAnimatorV21>& out,std::string& e){auto found=animators.find(visual);if(found!=animators.end()){out=found->second;return true;}
  std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> actual;if(!actual_visual(visual,actual,e))return false;
  auto controller=std::make_shared<dh2::world::RetainedGenericAnimatorV21>(actual);if(!controller->initialize(e))return false;animators.emplace(visual,controller);out=std::move(controller);return true;
 }
 bool decor(dh2::world::CanonicalGameObjectBaseOwnerV1& base,dh2::world::DecorServicesV15& output,std::string& e){
  auto found=decor_connections.find(base.identity());if(found==decor_connections.end()){
   std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* same{};if(!borrow(base.identity(),pin,same,e)||same!=&base)return false;
   auto world=physics.lock();if(!world||!physical_associations){e="Required actual native PhysicalWorld and canonical peer association";return false;}
   auto source=native.decor_native;source.owner=files;std::weak_ptr<RendererCampaignNonCharacterV69> weak=shared_from_this();const auto id=base.identity();
   source.debug_switch=[weak](const char* key,bool& value,std::string& e){auto self=weak.lock();auto native_world=self?self->scope.actual_world.lock():nullptr;auto world=std::static_pointer_cast<WorldScriptContext>(native_world);std::uint32_t result{};
    if(!self||!self->current(e)||!world||!world->debug||dh2_character_debug_load(world->debug.get(),&world->debug_files)!=1||dh2_character_debug_get(&result,world->debug.get(),key,&world->debug_files)!=1){if(e.empty())e="Required same actual Debug physical prefix";return false;}value=result!=0;return true;};
   source.update_pf=[weak,id](std::string& e){auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};return self&&self->borrow(id,pin,base,e)&&self->pf(*base,false,nullptr,0,e);};
   auto visual=[weak](std::uintptr_t id){std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> result;std::string e;auto self=weak.lock();if(self)self->actual_visual(id,result,e);return result;};
   auto connection=std::make_shared<dh2::world::CanonicalDecorPhysicalConnectionV49>(base,*world,*physical_associations,pin,std::move(visual),std::move(source));found=decor_connections.emplace(id,std::move(connection)).first;
  }
  found->second->bind(output);bind_retained_decor_floor_v77(std::weak_ptr<RendererCampaignNonCharacterV69>(shared_from_this()),output);output.owner=shared_from_this();return true;
 }
 bool pf(dh2::world::CanonicalGameObjectBaseOwnerV1& base,bool initialize,const float* p,float radius,std::string& e){
  auto geometry=floors.lock();auto registry=navigation.lock();if(!geometry||!registry||!current(e)){if(e.empty())e="Required SAME native PF world/registry";return false;}
  if(initialize){dh2::navigation::ObjectInitRequest q{&geometry->collision_world,&base.runtime().object,base.identity(),{p[0],p[1],p[2]},radius,base.lifecycle().static84,0};
   if(dh2_nav_init_object(&q)){e="Actual generic GameObject PF InitObject failed";return false;}return true;
  }
  if(base.runtime().object.motion.floor==UINT32_MAX)return true; // actual source393ea0 absent-floor exit
  dh2::physical::NativeBody* body{};std::shared_ptr<void> physical_pin;
  if(*base.pointer(0x2dc)){
   if(!physical_body(base,physical_pin,body,e)||!physical_pin||!body){if(e.empty())e="Required typed native physical2dc borrower";return false;}
  }
  // Execute actual generic GameObject PF control using the existing owner;
  // only its recovered virtual b4/b8/bc differ from Character (literal0/0/0).
  struct Context {dh2::world::CanonicalGameObjectBaseOwnerV1* base;dh2::physical::NativeBody* body;} context{&base,body};
  dh2::character::WorldNpcObjectServicesV1 services;services.context=&context;
  services.physical=[](void* p){return static_cast<Context*>(p)->body;};
  services.is_pf_obstacle=[](void*,bool& value,std::string& e){value=false;e.clear();return true;};
  services.obstacle_weight=[](void*,float& value,std::string& e){value=0;e.clear();return true;};
  services.obstacle_extent=services.obstacle_weight;
  services.absolute_aabb=[](void* p,float* box,std::string& e){std::copy_n(static_cast<Context*>(p)->base->absolute_aabb12c(),6,box);e.clear();return true;};
  dh2::character::CharacterWorldNpcObjectV1 source(base.lifecycle(),base.runtime().object,&geometry->collision_world,&registry->registry(),base.identity(),services);
  if(!source.update_pf()){e=source.error();return false;}return true;
 }
 bool ensure(dh2::world::CanonicalGameObjectBaseOwnerV1& supplied,Bound*& out,std::string& e){
  auto found=bound.find(supplied.identity());if(found!=bound.end()){out=&found->second;return true;}
  std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};if(!borrow(supplied.identity(),pin,base,e)||base!=&supplied)return false;
  auto self=shared_from_this();std::weak_ptr<RendererCampaignNonCharacterV69> weak=self;const auto id=base->identity();
  dh2::world::CanonicalBaseBorrowV68 actual=[weak,id](auto& pin,auto*& base,std::string& e){auto self=weak.lock();return self&&self->borrow(id,pin,base,e);};
  Bound item;
  // Preserve genuine incoming condition/spawn/PF/native leaves before adding graph.
  auto original=native.source.auxiliary.lend_initialization;
  if(original){dh2::loader::AuxiliaryDeliveryV67 d{scope.actual_world.lock(),scope.level.lock(),scope.objects.lock(),scope.properties};if(!original(d,*base,item.initialization,e))return false;}
  auto& init=item.initialization;auto& position=item.position;
  // Graph retained service bags use independent leaf owner, never its own bundle.
  init.owner=files;position.owner=files;
  init.difficulty_names=native.source.auxiliary.difficulty_names.get();init.sound_names=native.source.auxiliary.sound_names.get();
  if(!init.condition_init){if(!dh2::world::ConditionDataBindingV72::create(id,actual,native.conditions,item.conditions,e))return false;init.condition_init=item.conditions->initialization();}
  if(!init.check_spawn_probability)init.check_spawn_probability=[weak,actual](std::int32_t& roll,std::string& e){auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};if(!self||!actual(pin,base,e))return false;
   auto app=self->application.lock();auto random=app?app->source_random_v62():nullptr;if(!app||!random){e="Required SAME Application source Random channels";return false;}
   dh2::world::GameObjectSpawnProbabilityBorrowV1 b;b.owner=self->files;b.cached_roll270=base->integer(0x270);b.probability274=base->integer(0x274);b.network_id108=base->integer(0x108);b.byte82=base->byte(0x82);
   std::int32_t owner_fc{};b.online_owner_fc=&owner_fc;b.random0=&random->channel(0);b.random1=&random->channel(1);
   b.handle_as_player_character=[actual](bool& player,std::string& e){std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};if(!actual(pin,base,e))return false;auto object=base->canonical(pin);std::uintptr_t character{};if(!object.as_character||!object.as_character(object.context,character,e))return false;
    // This is the actual non-Character GameObject virtual AsChar body, not PM fallback.
    if(character){e="Non-Character source receiver unexpectedly returned Character";return false;}player=false;return true;};
   b.online_byte5=[app,actual,&owner_fc](bool& online,std::string& e){std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};if(!actual(pin,base,e))return false;auto network=app->get_online_loading_v55();if(!network){e="Required actual GetOnline receiver";return false;}online=network->byte5()!=0;return base->source_online_owner_fc_v4(owner_fc,e);};
   b.set_visible_false=[weak,actual](std::string& e){auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};if(!self||!actual(pin,base,e)||!base->store_byte(0x80,0,e))return false;return self->source_visibility(*base,e);};
   b.object_base_delete=[actual](std::string& e){std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};if(!actual(pin,base,e))return false;base->source_delete_v4();return true;};
   if(self->native.failed_spawn_mark)b.mark_for_deletion=[weak,actual](std::string& e){auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};return self&&actual(pin,base,e)&&self->native.failed_spawn_mark(*base,e);};
   std::int32_t probability{};return dh2::world::game_object_check_spawn_probability_v1(b,roll,probability,e);
  };
  if(!init.device_high_performance)init.device_high_performance=[](bool& v,std::string& e){return borrow_actual_device_high_performance_v54(v,e);};
  if(!init.set_visible)init.set_visible=[weak,actual](bool value,std::string& e){auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};if(!self||!actual(pin,base,e))return false;
   // Whole source GameObject SetVisible stores same visible80 then actual SyncVisibility.
   if(!base->store_byte(0x80,value?base->lifecycle().enabled8a:0,e))return false;return self->source_visibility(*base,e);};
  if(!init.init_pf_object)init.init_pf_object=[weak,actual](bool,const float* p,float radius,std::uintptr_t identity,std::string& e){auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};return self&&actual(pin,base,e)&&base->identity()==identity&&self->pf(*base,true,p,radius,e);};
  if(!init.update_pf_object)init.update_pf_object=[weak,actual](std::string& e){auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};return self&&actual(pin,base,e)&&self->pf(*base,false,nullptr,0,e);};
  if(!init.light_set_id){const std::weak_ptr<dh2::world::LightSetNameOwnerV3> names=native.light_names;init.light_set_id=[names](const auto& name,std::int32_t& id,std::string& e){auto held=names.lock();if(!held){e="Released actual LightSet name owner";return false;}id=held->get_id(name);e.clear();return true;};}
  auto original_position=native.source.auxiliary.lend_position;
  if(original_position){dh2::loader::AuxiliaryDeliveryV67 d{scope.actual_world.lock(),scope.level.lock(),scope.objects.lock(),scope.properties};if(!original_position(d,*base,position,e))return false;position.owner=files;}
  if(!position.attached_position&&native.attached_position)position.attached_position=[weak,actual](std::uintptr_t id,float*& p,std::string& e){auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};return self&&actual(pin,base,e)&&self->native.attached_position(*base,id,p,e);};
  if(!position.physical_position)position.physical_position=[weak,actual](std::uintptr_t id,float x,float y,std::string& e){auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};if(!self||!actual(pin,base,e)||*base->pointer(0x2dc)!=id)return false;
   std::shared_ptr<void> native_pin;dh2::physical::NativeBody* body{};if(!self->physical_body(*base,native_pin,body,e)||!native_pin||!body)return false;const float xy[]{x,y};if(dh2_native_body_set_position(body,xy)<0){e="Actual generic physical SetPosition failed";return false;}return true;};
  if(!external_visuals.count(id)&&!graph->bind(id,actual,init,position,e))return false;
  // Restore SAME V67 boundary authority only AFTER graph stored acyclic bags.
  init.owner=files;position.owner=files;auto inserted=bound.emplace(id,std::move(item));out=&inserted.first->second;return true;
 }
};
struct RendererCampaignNonCharacterCompositionV69 {
 std::function<bool(std::uintptr_t,std::shared_ptr<dh2::world::CanonicalSpawnPointV15>&,std::string&)> borrow_spawn_point_v80;
 std::shared_ptr<RendererCampaignNonCharacterV69> services;
 std::shared_ptr<dh2::loader::ProductionNonCharacterCatalogV67> catalog;
 dh2::loader::ProductionNonCharacterOwnersV67 owners;
 SourceCampaignClassFactoryV60 factory;
};
bool compose_campaign_noncharacter_v69(AAssetManager* assets,const SourceCampaignCandidateBorrowV55& actual,
 RendererCampaignNonCharacterInputsV69 inputs,RendererCampaignNonCharacterCompositionV69& out,std::string& e){
 if(!assets||out.services||!actual.actual_world||!actual.level||!actual.objects||!actual.properties||!actual.roots||!actual.floors||!actual.navigation_registry){e="Required once-only genuine non-Character source candidate";return false;}
 // Actual methods are required, never replaced by successful/missing callback stubs.
 if(!inputs.source.auxiliary.retain_release_record||!inputs.source.auxiliary.release_completed||!inputs.source.auxiliary.difficulty_names||!inputs.source.auxiliary.sound_names){e="Required actual Root auxiliary journal and immutable Arrays names";return false;}
 if(!lend_campaign_condition_bag_v75(actual,inputs.conditions,inputs.condition_runtime,e))return false;
 auto self=std::make_shared<RendererCampaignNonCharacterV69>();self->scope={actual.actual_world,actual.level,actual.objects,actual.properties,{}};
 self->application=actual.application;self->physics=actual.physical_world;self->physical_associations=std::make_unique<dh2::character::LootPhysicalAssociationsV49>(*actual.objects);self->floors=actual.floors;self->navigation=actual.navigation_registry;self->rooms=actual.module_rooms_v69;self->map=actual.map_owner_v69;self->transport=actual.receiver_transport_v69;
 self->files=std::make_shared<dh2::android_ui::OriginalCacheAssetsV1>(assets);self->native=std::move(inputs);
 if(!bind_campaign_runtime_receivers_v80(std::weak_ptr<RendererCampaignNonCharacterV69>(self),self->native.runtime_receiver,e))return false;
 std::weak_ptr<RendererCampaignNonCharacterV69> weak=self;
 dh2::world::CanonicalGameObjectGraphServicesV68 graph;graph.owner=self->files;graph.roots=actual.roots;graph.visual.owner=self->files;
 graph.visual.read_asset=[files=self->files](const auto& path,auto& bytes,bool& found,auto& e){return files->read(path,found,bytes,e);};
 graph.colzone_inside_v91=[weak](auto& base,std::uintptr_t id,const auto& peer,bool& inside,std::string& e){auto self=weak.lock();return self&&self->current(e)&&retained_colzone_inside_v91(*self,base,id,peer,inside,e);};
 graph.native_body_v90=[weak](auto& base,auto& pin,auto*& body,std::string& e){auto self=weak.lock();return self&&self->current(e)&&self->physical_body(base,pin,body,e);};
 graph.current=[weak](std::string& e){auto self=weak.lock();return self&&self->current(e);};
 graph.is_animated=[](auto& base,bool& value,std::string& e){const auto* name=base.class_name20();if(!name){e="Actual canonical class name not produced";return false;}
  value=!std::strcmp(name,"AnimatedDecor")||!std::strcmp(name,"TriggerObject")||!std::strcmp(name,"Door")||!std::strcmp(name,"OpenableContainer")||!std::strcmp(name,"DestructibleContainer");e.clear();return true;};
 self->graph=std::make_shared<dh2::world::CanonicalGameObjectGraphV68>(std::move(graph));
 bind_campaign_zone_physical_v90(weak,self->native.zone_physical);

 if(!borrow_source_campaign_game_object_arrays_v81(actual,self->native.declaration_arrays,e))return false;
 auto classes=self->native.source;
 if(!bind_campaign_declaration_arrays_v81(std::weak_ptr<RendererCampaignNonCharacterV69>(self),classes,self->native.declaration_arrays,e))return false;
 auto admit=classes.auxiliary.retain_release_record;
 auto boundary=[weak](auto original){return [weak,original](const auto& d,auto& receiver,auto& output,std::string& e){auto self=weak.lock();if(!self||!self->current(e)||!original(d,receiver,output,e))return false;output.owner=self;
  using Services=std::decay_t<decltype(output)>;
  if constexpr(std::is_same_v<Services,dh2::world::AnimatedDecorServicesV23>){output.decor.owner=self;output.source_init.owner=self;}
  if constexpr(std::is_same_v<Services,dh2::world::TriggerZoneServicesV22>)self->bind_trigger_zone_resources_v82(receiver.base().identity(),output);
  if constexpr(std::is_same_v<Services,dh2::world::ExitZoneServicesV29>){output.trigger.owner=self;self->bind_trigger_zone_resources_v82(receiver.base().identity(),output.trigger);}
  if constexpr(std::is_same_v<Services,dh2::world::DoorServicesV27>||std::is_same_v<Services,dh2::world::TriggerObjectServicesV28>){output.startup.owner=self;output.startup.zone.owner=self;
   self->bind_zone_startup_v82(output.startup.zone);bind_startup_idle_animation_v86(weak,output.startup);bind_startup_sound_capture_v87(weak,output.startup);
   output.startup.zone.set_bounding_box=[weak](auto& base,const float* same,bool absolute,std::string& e){auto self=weak.lock();RendererCampaignNonCharacterV69::Bound* bound{};if(!self||!self->ensure(base,bound,e))return false;if(absolute||same!=base.relative_aabb144()){e="Required source Zone relative144,false receipt";return false;}return dh2::world::game_object_relative_box_v3(base,same,bound->initialization.update_pf_object,e);};}
  if constexpr(std::is_same_v<Services,dh2::world::CheckpointZoneServicesV26>||std::is_same_v<Services,dh2::world::QuestMoveZoneServicesV31>){
   output.startup.owner=self;self->bind_zone_startup_v82(output.startup);output.startup.set_bounding_box=[weak](auto& base,const float* same,bool absolute,std::string& e){auto self=weak.lock();RendererCampaignNonCharacterV69::Bound* bound{};if(!self||!self->ensure(base,bound,e))return false;if(absolute||same!=base.relative_aabb144()){e="Required source Zone relative144,false receipt";return false;}return dh2::world::game_object_relative_box_v3(base,same,bound->initialization.update_pf_object,e);};
  }
  return true;};};
#define BOUNDARY(field) if(classes.auxiliary.field)classes.auxiliary.field=boundary(classes.auxiliary.field)
 BOUNDARY(dummy);BOUNDARY(spawn_point);BOUNDARY(decor);BOUNDARY(trigger_zone);BOUNDARY(animated_decor);BOUNDARY(checkpoint);BOUNDARY(door);BOUNDARY(trigger_object);BOUNDARY(exit_zone);BOUNDARY(quest_move);BOUNDARY(sound);
#undef BOUNDARY
 classes.auxiliary.owner=self;
 classes.auxiliary.validate_current=[weak](const auto&,std::string& e){auto self=weak.lock();return self&&self->current(e);};
 classes.auxiliary.retain_release_record=[weak,admit](const auto& d,const auto& r,const auto& q,std::string& e){auto self=weak.lock();return self&&self->admit_auxiliary(r,e)&&admit(d,r,q,e);};
 auto release_complete=classes.auxiliary.release_completed;
 classes.auxiliary.release_completed=[weak,release_complete](const auto& d,std::uintptr_t id,std::string& e){auto self=weak.lock();return self&&self->zone_prefixes_released(id,e)&&release_complete(d,id,e);};
 classes.auxiliary.lend_initialization=[weak](const auto&,auto& base,auto& output,std::string& e){auto self=weak.lock();RendererCampaignNonCharacterV69::Bound* bound{};if(!self||!self->ensure(base,bound,e))return false;output=bound->initialization;output.owner=self;return true;};
 classes.auxiliary.lend_position=[weak](const auto&,auto& base,auto& output,std::string& e){auto self=weak.lock();RendererCampaignNonCharacterV69::Bound* bound{};if(!self||!self->ensure(base,bound,e))return false;output=bound->position;output.owner=self;return true;};
 // Script-ID lookup is the actual App-retained V52 manager/name owner. Scheduling
 // remains the genuine incoming StartScript/IsRunning services, never fabricated.
 auto source_spawn=classes.auxiliary.spawn_point;
 if(source_spawn)classes.auxiliary.spawn_point=[weak,source_spawn](const auto& d,auto& receiver,auto& output,std::string& e){auto self=weak.lock();if(!self||!source_spawn(d,receiver,output,e))return false;output.owner=self;
  output.script_id=[weak](const std::string& name,bool common,std::int32_t& id,std::string& e){auto self=weak.lock();if(!self||!self->current(e))return false;auto app=self->application.lock();auto scripts=app?app->source_script_manager_v52():nullptr;if(!scripts){e="Required actual App ScriptManager GetIDFromName";return false;}id=scripts->id_from_name(name.c_str(),common);e.clear();return true;};
  output.set_object_position=[weak](std::uintptr_t id,const float* p,bool destination,std::string& e){auto self=weak.lock();return self&&self->set_object_position(id,p,destination,e);};
  output.set_object_rotation=[weak](std::uintptr_t id,const float* p,std::string& e){auto self=weak.lock();return self&&self->set_object_rotation(id,p,e);};return true;};
 auto source_trigger=classes.auxiliary.trigger_object;
 if(source_trigger)classes.auxiliary.trigger_object=[weak,source_trigger](const auto& d,auto& receiver,auto& output,std::string& e){auto self=weak.lock();if(!self||!source_trigger(d,receiver,output,e))return false;output.owner=self;output.startup.owner=self;output.startup.zone.owner=self;
  output.startup.borrow_script_manager=[weak](auto& output,std::string& e){auto self=weak.lock();auto app=self?self->application.lock():nullptr;auto actual=app?app->source_script_manager_v52():nullptr;if(!self||!self->current(e)||!actual){if(e.empty())e="Required SAME App ScriptManager for actual TriggerObject startup";return false;}output=std::move(actual);e.clear();return true;};
  output.startup.borrow_conditions=[weak](auto& output,std::string& e){auto self=weak.lock();if(!self||!self->current(e)||!self->native.condition_runtime||!self->native.conditions.owner){if(e.empty())e="Required SAME native campaign condition service receipt";return false;}output=self->native.conditions;e.clear();return true;};
  return true;};
 auto source_decor=classes.auxiliary.decor;
 if(source_decor)classes.auxiliary.decor=[weak,source_decor](const auto& d,auto& receiver,auto& output,std::string& e){auto self=weak.lock();return self&&source_decor(d,receiver,output,e)&&self->decor(receiver.base(),output,e);};
 auto source_animated=classes.auxiliary.animated_decor;
 if(source_animated)classes.auxiliary.animated_decor=[weak,source_animated](const auto& d,auto& receiver,auto& output,std::string& e){auto self=weak.lock();if(!self||!source_animated(d,receiver,output,e))return false;output.owner=self;output.decor.owner=self;output.source_init.owner=self;
  if(!self->decor(receiver.base(),output.decor,e))return false;
  output.source_init.visual_sync=output.decor.visual_sync; // genuine ApplyMeshBox470a54, not base Sync
  output.source_init.visual_physical28=output.decor.visual_physical;output.source_init.construct_podecor=output.decor.construct_podecor;output.source_init.set_physical=output.decor.set_physical;
  output.source_init.animation_count=[weak](std::uintptr_t id,std::int32_t& count,std::string& e){auto self=weak.lock();std::shared_ptr<dh2::world::RetainedGenericAnimatorV21> actual;std::uint32_t value{};if(!self||!self->animator(id,actual,e)||!actual->count(value,e))return false;count=static_cast<std::int32_t>(value);return true;};
  output.source_init.animation_exists=[weak](std::uintptr_t id,const std::string& name,bool& found,std::string& e){auto self=weak.lock();std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;if(!self||!self->actual_visual(id,visual,e))return false;auto library=visual->named_animation(visual);std::int32_t index{};if(!library.timeline_find_name){e="Required actual timeline name lookup";return false;}if(!library.timeline_find_name(name.c_str(),index,e))return false;found=index>=0;return true;};
  output.source_init.play_index=[weak](std::uintptr_t id,std::int32_t index,bool loop,bool& played,std::string& e){auto self=weak.lock();std::shared_ptr<dh2::world::RetainedGenericAnimatorV21> actual;return self&&self->animator(id,actual,e)&&actual->play_index(static_cast<std::uint32_t>(index),loop,played,e);};
  output.source_init.play_name=[weak](std::uintptr_t id,const std::string& name,bool loop,bool& played,std::string& e){auto self=weak.lock();std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;return self&&self->actual_visual(id,visual,e)&&visual->play(name.c_str(),loop,played,e);};
  output.source_init.random=[weak](std::int32_t maximum,std::int32_t& value,std::string& e){auto self=weak.lock();auto app=self?self->application.lock():nullptr;auto random=app?app->source_random_v62():nullptr;if(!self||!self->current(e)||!random||dh2_loot_v2_random(&random->channel(0),static_cast<std::uint32_t>(maximum),&value)){if(e.empty())e="Required actual source shared Random for AnimatedDecor";return false;}return true;};
  const auto owner_id=receiver.base().identity();
  output.source_init.install_random_completion=[weak,owner_id](std::uintptr_t visual_id,std::function<bool(std::string&)> source,std::string& e){auto self=weak.lock();std::shared_ptr<dh2::world::RetainedGenericAnimatorV21> actual;if(!self||!self->animator(visual_id,actual,e))return false;std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;if(!self->actual_visual(visual_id,visual,e))return false;auto fields=visual->generic_callbacks_v69().fields();fields.completion_owner=self->files;
   fields.completion=[weak,owner_id,source=std::move(source)](auto&,std::string& e){auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};return self&&self->borrow(owner_id,pin,base,e)&&source(e);};return actual->set_callbacks(std::move(fields),e);};
  if(!output.source_init.update&&self->native.lend_frame_services){const auto id=receiver.base().identity();output.source_init.update=[weak,id](std::string& e){auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};if(!self||!self->borrow(id,pin,base,e))return false;dh2::loader::GameObjectSourceFrameServicesV74 frame;if(!self->native.lend_frame_services(*base,frame,e))return false;frame.owner=self->files;frame.lend_runtime=[weak](auto& same,auto& output,std::string& e){auto owner=weak.lock();return owner&&owner->lend_runtime(same,output,e);};return dh2::loader::game_object_source_frame_v74(*base,frame,e);};}
  return true;};
 auto openable_services=classes.containers.openable_services;
 if(openable_services)classes.containers.openable_services=[weak,openable_services](const auto& scope,const auto& q,const auto& slot,auto& output,std::string& e){auto self=weak.lock();if(!self||!self->current(e)||!openable_services(scope,q,slot,output,e))return false;
  output.world=self;
  auto get=[slot](auto& pin,auto*& base,std::string& e){auto actual=slot?slot->lock():nullptr;if(!actual){e="Unproduced/released SAME Openable C1";return false;}pin=actual;base=&actual->base();return true;};
  self->bind_container_initialization_v89(get,output.initialization);
  if(!output.container.spawn_roll_and_probability)output.container.spawn_roll_and_probability=self->container_spawn_roll_v89(get);
  // Use actual roots/physics already provided by genuine source container services.
  // Read from mounted APK cache; graph remains the sole Openable Visual C1.
  output.visual.owner=self->files;if(!output.visual.read_asset)output.visual.read_asset=[files=self->files](const auto& path,auto& bytes,bool& found,std::string& e){return files->read(path,found,bytes,e);};
  output.initialization.owner=self;output.position.owner=self;output.decor.owner=self;output.container.owner=self;
  if(!output.container.precache_complete_source_v42)output.container.precache_complete_source_v42=bind_openable_precache_slot_v45(slot,self->native.source.containers.openable_table);
  return true;};
 auto admit_openable=classes.containers.admit_openable;
 if(admit_openable)classes.containers.admit_openable=[weak,admit_openable](const auto& scope,const auto& q,const auto& actual,std::string& e){auto self=weak.lock();if(!self||!admit_openable(scope,q,actual,e)||!self->current(e))return false;
  std::weak_ptr<dh2::world::CanonicalOpenableGraphV21> receiver=actual;const auto id=actual->receiver().base().identity();
  dh2::world::CanonicalBaseBorrowV68 borrow=[weak,receiver](auto& pin,auto*& base,std::string& e){auto self=weak.lock();auto graph=receiver.lock();if(!self||!graph||!self->current(e))return false;pin=graph;base=&graph->receiver().base();return true;};
  self->borrows.push_back(borrow);self->openable_graphs_v89[id]=actual;
  auto visual=[receiver](auto& out,std::string& e){auto graph=receiver.lock();if(!graph){e="Released SAME Openable graph visual";return false;}out=graph->visual();e.clear();return true;};
  if(!self->graph->observe_visual(id,borrow,std::move(visual),e))return false;self->external_visuals.insert(id);return true;};
 auto destructible_services=classes.containers.destructible_services;
 if(destructible_services)classes.containers.destructible_services=[weak,destructible_services](const auto& scope,const auto& q,const auto& slot,auto& init,auto& output,std::string& e){auto self=weak.lock();if(!self||!self->current(e)||!destructible_services(scope,q,slot,init,output,e))return false;
  init.owner=self;output.owner=self;output.common.owner=self;
  auto get=[slot](auto& pin,auto*& base,std::string& e){auto actual=slot?slot->lock():nullptr;if(!actual){e="Unproduced/released SAME Destructible C1";return false;}pin=actual;base=&actual->base();return true;};
  self->bind_container_initialization_v89(get,init);
  if(!output.common.spawn_roll_and_probability)output.common.spawn_roll_and_probability=self->container_spawn_roll_v89(get);
  if(!output.common.precache_complete_source_v42)output.common.precache_complete_source_v42=bind_destructible_precache_slot_v45(slot,self->native.source.containers.destructible_table);
  return true;};
 auto admit_destructible=classes.containers.admit_destructible;
 if(admit_destructible)classes.containers.admit_destructible=[weak,admit_destructible](const auto& scope,const auto& q,const auto& actual,std::string& e){auto self=weak.lock();if(!self||!admit_destructible(scope,q,actual,e)||!self->current(e))return false;
  std::weak_ptr<dh2::world::CanonicalDestructibleContainerV16> receiver=actual;
  self->borrows.push_back([weak,receiver](auto& pin,auto*& base,std::string& e){auto self=weak.lock();auto r=receiver.lock();if(!self||!r||!self->current(e))return false;pin=r;base=&r->base();return true;});return true;};
 // Same independent V69 authority required by CatalogEnvironment exact-owner
 // admission. Clone the service packet only; names/nodes/native owners remain
 // SAME. All reached engine callbacks borrow V69 self weakly across delivery.
 if(classes.lights){
  auto lights=std::make_shared<dh2::loader::LightEnvironmentLeavesV67>(*classes.lights);lights->owner=self;
  auto guard_light=[weak](auto original){return [weak,original=std::move(original)](auto&&... args)->bool{
   std::string* error{};auto locate=[&](auto& value){if constexpr(std::is_same_v<decltype(value),std::string&>)error=&value;};(locate(args),...);
   if(!error)return false;auto owner=weak.lock();if(!owner){*error="Required live SAME V69 light delivery authority";return false;}
   if(!owner->current(*error)||!original(std::forward<decltype(args)>(args)...))return false;return owner->current(*error);
  };};
#define V69_LIGHT_LEAF(field) if(lights->field)lights->field=guard_light(std::move(lights->field))
  V69_LIGHT_LEAF(validate_current);V69_LIGHT_LEAF(bind_objectbase_dtor);V69_LIGHT_LEAF(first_scene_light);V69_LIGHT_LEAF(construct_light_node);V69_LIGHT_LEAF(attach_root);V69_LIGHT_LEAF(add_automatic);V69_LIGHT_LEAF(borrow_light_parameters);V69_LIGHT_LEAF(debug_switch);V69_LIGHT_LEAF(set_type);V69_LIGHT_LEAF(refresh_attachment);V69_LIGHT_LEAF(retain_release_record);V69_LIGHT_LEAF(release_completed);
#undef V69_LIGHT_LEAF
  classes.lights=std::move(lights);
 }
 RendererCampaignNonCharacterCompositionV69 prepared;prepared.services=self;
 if(!dh2::loader::make_source_campaign_noncharacter_factory_v67(actual,self,std::move(classes),self->native.required_types,prepared.catalog,prepared.factory,e,&prepared.owners))return false;
 if(self->native.after_catalog_composition_v89&&!self->native.after_catalog_composition_v89(prepared.owners,e))return false;
 // Enrollment precedes XML/Modules. Capture the original class metadata as
 // each real authored factory entry reaches dispatch; never hardcode a level.
 auto source_factory=std::move(prepared.factory);
 prepared.factory=[weak,source_factory](const auto& entry,const auto& request,auto& receiver,std::string& e){
  auto same=weak.lock();if(!same||!same->current(e)||!entry.name)return false;
  auto& names=same->native.required_types;
  if(std::find(names.begin(),names.end(),entry.name)==names.end())names.emplace_back(entry.name);
  return source_factory(entry,request,receiver,e);
 };
 bind_campaign_base_admission_v78(weak,prepared.factory);
 if(!bind_source_campaign_object_graph_v68(actual,self->graph,e))return false;
 // Preserve existing actual placement/save callbacks; replace only typed Spawn borrow.
 auto placement=self->native.placement;placement.provider=self;std::weak_ptr<dh2::loader::CatalogAuxiliaryV67> auxiliary=prepared.owners.auxiliary;
 prepared.borrow_spawn_point_v80=[auxiliary,weak](std::uintptr_t id,auto& out,std::string& e){auto self=weak.lock();auto owner=auxiliary.lock();return self&&self->current(e)&&owner&&owner->borrow_spawn_point(id,out,e);};
 placement.spawn_point=[auxiliary](std::uintptr_t id,dh2::loader::SpawnPointPlacementBorrowV68& out,std::string& e){auto owner=auxiliary.lock();std::shared_ptr<dh2::world::CanonicalSpawnPointV15> spawn;
  if(!owner||!owner->borrow_spawn_point(id,spawn,e))return false;dh2::loader::SpawnPointPlacementBorrowV68 result;result.receiver=spawn;result.identity=spawn->base().identity();std::weak_ptr<dh2::world::CanonicalSpawnPointV15> weak=spawn;
  result.source_enabled8a=[weak](std::uint8_t& v,std::string& e){auto same=weak.lock();return same&&same->read_bool(0x8a,v,e);};
  result.source_entrypoint374=[weak](std::int32_t& v,std::string& e){auto same=weak.lock();if(!same){e="Released SAME SpawnPoint374";return false;}v=same->entrypoint();e.clear();return true;};
  result.place_object=[weak](std::uintptr_t actor,std::string& e){auto same=weak.lock();return same&&same->place_object(actor,e);};out=std::move(result);return true;};
 if(!bind_source_campaign_player_placement_v68(actual,std::move(placement),e))return false;
 out=std::move(prepared);e.clear();return true;
}








