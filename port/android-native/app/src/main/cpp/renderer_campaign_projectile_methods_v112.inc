//After V69/V111 inside model_renderer. Headers belong at namespace-global.
//All loans use existing canonical/Scene/PF/App owners, never a second pool.
static bool bind_campaign_projectile_environment_v112(const std::shared_ptr<RendererCampaignNonCharacterV69>&,
 dh2::world::ProjectileMethodServicesV112&,dh2::world::LaserProjectileServicesV112&,std::string&);
struct ProjectileObjectFieldsNativeV112 {
 std::shared_ptr<void> receiver;std::uintptr_t identity{};
 std::function<float*(std::uint32_t)> vector3;
 std::function<std::uintptr_t*(std::uint32_t)> pointer;
 std::function<std::uint8_t*(std::uint32_t)> byte;
 std::function<bool(std::int32_t&,std::string&)> dead;
};
static bool lend_campaign_projectile_methods_v112(
 const std::shared_ptr<RendererCampaignNonCharacterV69>& self,dh2::world::CanonicalProjectileV99& p,
 dh2::world::ProjectileMethodServicesV112& s,dh2::world::LaserProjectileServicesV112& laser,std::string& e){
 if(!self||!self->current(e)||!self->graph||!self->physical_associations){if(e.empty())e="Required SAME Projectile native campaign owners";return false;}
 const auto weak=std::weak_ptr<RendererCampaignNonCharacterV69>(self);const auto id=p.base().identity();
 s.owner=self->files;
 s.current=[weak,id](auto& p,std::string& e){auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  return self&&self->borrow(id,pin,base,e)&&base==&p.base();};
 s.object=[weak](std::uintptr_t id,dh2::world::ProjectileObjectV112& out,std::string& e){
  auto self=weak.lock();SourceCampaignCandidateBorrowV55 c;if(!self||!self->current(e)||!borrow_source_campaign_candidate_v55(c,e))return false;
  dh2::world::ObjectUpdateActorV102 object;if(!borrow_source_campaign_object_update_actor_v104(c,id,object,e))return false;
  std::uintptr_t character{};if(!object.object.as_character||!object.object.as_character(object.object.context,character,e))return false;
  auto fields=std::make_shared<ProjectileObjectFieldsNativeV112>();fields->identity=id;
  if(character){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(c.actual_world,character,actual,e)||!actual.character->actor)return false;
   dh2::world::GameObjectInitializationFieldsV62 f;if(!actual.character->actor->inherited_initialization_fields_v62(actual.character,f,e))return false;
   fields->receiver=actual.character;fields->vector3=f.vector3;fields->pointer=f.pointer;fields->byte=f.byte;
   fields->dead=[record=std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>(actual.character)](std::int32_t& out,std::string& e){auto actual=record.lock();
    if(!actual||!actual->life){e="Retired SAME Character.IsDead receiver";return false;}out=actual->life->dead;e.clear();return true;};
  }else{std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};if(!self->borrow(id,pin,base,e))return false;
   fields->receiver=pin;fields->vector3=[base](auto o){return base->vector3(o);};fields->pointer=[base](auto o){return base->pointer(o);};fields->byte=[base](auto o){return base->byte(o);};
   if(base->type_f4()==9||base->type_f4()==10){auto catalog=self->projectile_catalog_v111.lock();std::shared_ptr<dh2::world::CanonicalProjectileV99> projectile;
    if(!catalog||!catalog->borrow_projectile_v99(id,projectile,e))return false;
    fields->dead=[projectile=std::weak_ptr<dh2::world::CanonicalProjectileV99>(projectile)](std::int32_t& out,std::string& e){auto actual=projectile.lock();
     auto cell=actual?actual->source_byte_v99(0x3d0):nullptr;if(!cell){e="Retired Projectile.IsDead3d0";return false;}out=*cell;e.clear();return true;};
   }else fields->dead=[](std::int32_t& out,std::string& e){out=0;e.clear();return true;}; //selected ObjectBase340078
  }
  if(!fields->receiver||!fields->vector3||!fields->pointer||!fields->byte){e="Missing SAME GameObject field loans";return false;}
  out={fields,id,fields->pointer(0x2d8),fields->vector3(0x16c)};e.clear();return true;
 };
 s.look_at=[](const auto& object,auto& out,std::string& e){auto fields=std::static_pointer_cast<ProjectileObjectFieldsNativeV112>(object.owner);
  auto rotation=fields&&fields->identity==object.identity&&fields->vector3?fields->vector3(0x16c):nullptr;
  if(!rotation){e="Required SAME GameObject.GetLookAtVec Euler174";return false;}
  const float angle=rotation[2];out[0]=std::sin(angle);const float c=std::cos(angle);std::uint32_t bits{};std::memcpy(&bits,&c,4);bits^=0x80000000u;std::memcpy(&out[1],&bits,4);out[2]=0;e.clear();return true;
 };
 s.target_position=[](const auto& object,dh2::world::ProjectilePointBorrowV112& out,std::string& e){
  auto fields=std::static_pointer_cast<ProjectileObjectFieldsNativeV112>(object.owner);
  if(!fields||fields->identity!=object.identity){e="Foreign Projectile source GameObject target-position loan";return false;}
  const auto target=fields->pointer(0x180);if(!target){e="Required actual GameObject.target180 cell";return false;}
  const auto visible=*target?fields->byte(0x80):nullptr;
  if(*target&&!visible){e="Required produced GameObject.visible80 for GetTargetPosition";return false;}
  const float* xyz=fields->vector3(*target&&*visible?0x184:0x160);
  if(!xyz){e="Required SAME GetTargetPosition184/160 backing";return false;}
  out={object.owner,xyz};e.clear();return true;
 };
 s.object_virtual34=[](const auto& object,std::int32_t& out,std::string& e){auto fields=std::static_pointer_cast<ProjectileObjectFieldsNativeV112>(object.owner);
  return fields&&fields->identity==object.identity&&fields->dead&&fields->dead(out,e);};
 laser.object_position160=[](const auto& object,dh2::world::ProjectilePointBorrowV112& out,std::string& e){auto fields=std::static_pointer_cast<ProjectileObjectFieldsNativeV112>(object.owner);
  auto xyz=fields&&fields->identity==object.identity?fields->vector3(0x160):nullptr;if(!xyz){e="Required SAME peer position160";return false;}out={object.owner,xyz};e.clear();return true;};
 s.node_from_name=[weak](std::uintptr_t visual,const char* name,std::uintptr_t& out,std::string& e){auto self=weak.lock();std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> actual;
  return self&&self->actual_visual(visual,actual,e)&&actual->node_from_name(name,out,e);};
 auto find_node=[weak](std::uintptr_t id,std::shared_ptr<dh2::world::RetainedGameObjectVisualV1>& visual,std::string& e){auto self=weak.lock();
  std::vector<std::shared_ptr<dh2::world::RetainedGameObjectVisualV1>> visuals;if(!self||!self->current(e)||!self->graph->capture_visuals(visuals,e))return false;
  auto manager=self->scope.objects.lock();auto world=self->scope.actual_world.lock();const dh2::world::CanonicalObjectBorrowV1* object{};std::int32_t key{};
  bool next=manager&&manager->source_ordered_begin_v38(key,object);
  while(next){if(object&&object->as_character){std::uintptr_t character{};if(!object->as_character(object->context,character,e))return false;
    if(character){SourceCampaignCharacterBorrowV62 actual;if(!borrow_source_campaign_character_v62(world,character,actual,e))return false;
     auto v=actual.character&&actual.character->visual?actual.character->visual->visual():nullptr;if(v)visuals.push_back(std::move(v));}}
   next=manager->source_ordered_next_v38(key,key,object);}
  for(auto& candidate:visuals){const float* node{};std::string ignored;
   if(candidate->borrow_node_position_v80(id,node,ignored)){visual=candidate;e.clear();return true;}}
  e="Required SAME owned projectile scene child";return false;};
 s.node_absolute_position=[find_node](std::uintptr_t id,dh2::world::ProjectilePointBorrowV112& out,std::string& e){std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;const float* xyz{};
  if(!find_node(id,visual,e)||!visual->borrow_node_position_v80(id,xyz,e))return false;out={visual,xyz};return true;};
 laser.node_set_position=[find_node](std::uintptr_t id,const auto& p,std::string& e){std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;return find_node(id,visual,e)&&visual->source_node_set_position_v112(id,p.data(),e);};
 s.set_visual_index=[weak](auto& p,std::int32_t index,std::string& e){auto self=weak.lock();SourceCampaignCandidateBorrowV55 c;
  if(!self||!self->current(e)||!borrow_source_campaign_candidate_v55(c,e)||!c.fx_libraries)return false;
  auto* row=c.fx_libraries->animated_info_v88(index);
  if(!row||!row->file0){e="Actual Projectile AnimatedFXLibrary index outside source rows";return false;}
  return self->graph->source_set_visual_v112(p.base().identity(),row->file0->c_str(),nullptr,false,e);};
 s.set_visual_null=[weak](auto& p,std::string& e){auto self=weak.lock();return self&&self->current(e)&&self->graph->source_set_visual_null_v112(p.base().identity(),e);};
 s.retain_physical_prefix=[weak](const auto& prefix,std::string& e){auto self=weak.lock();if(!self||!self->current(e)||!prefix){e="Required fresh native PhysicalObject prefix";return false;}
  self->projectile_pending_prefixes_v112.push_back(prefix);e.clear();return true;};
 s.construct_physical=[weak](auto& p,auto& receipt,std::string& e){auto self=weak.lock();std::shared_ptr<void> receiver;dh2::world::CanonicalGameObjectBaseOwnerV1* same{};
  if(!self||!self->borrow(p.base().identity(),receiver,same,e)||same!=&p.base()||receipt.receiver)return false;
  auto world=self->physics.lock();std::shared_ptr<dh2::loader::AuxiliaryReleaseRecordV67> journal;
  for(auto& weak_record:self->auxiliary_journal){auto r=weak_record.lock();if(r&&r->identity&&r->identity()==p.base().identity()){journal=r;break;}}
  if(!world||!journal){e="Required SAME retained Projectile class/physics prefix journal";return false;}
  dh2::world::ProjectilePhysicalServicesV112 source;source.owner=self->files;source.peer_owner=self->native.zone_physical.peer_owner;source.peer_visible80=self->native.zone_physical.peer_visible80;
  source.debug_switch=[weak](const char* key,bool& out,std::string& e){auto self=weak.lock();auto world=std::static_pointer_cast<WorldScriptContext>(self?self->scope.actual_world.lock():nullptr);std::uint32_t value{};
   if(!self||!self->current(e)||!world||!world->debug||dh2_character_debug_load(world->debug.get(),&world->debug_files)!=1||dh2_character_debug_get(&value,world->debug.get(),key,&world->debug_files)!=1){if(e.empty())e="Required actual source Physical Debug switch";return false;}out=value!=0;return true;};
  source.on_collision=[weak,id=p.base().identity()](std::uintptr_t peer,const float* point,std::string& e){auto self=weak.lock();SourceCampaignCandidateBorrowV55 c;
   return self&&self->current(e)&&borrow_source_campaign_candidate_v55(c,e)&&source_campaign_projectile_collision_v112(c,id,peer,point,e);};
  auto physical=std::make_shared<dh2::world::CanonicalProjectilePhysicalV112>(p.base(),receiver,*world,*self->scope.objects.lock(),*self->physical_associations,std::move(source));
  auto prefix=std::make_shared<dh2::loader::AuxiliaryResourcePrefixV82>();prefix->backing=world;prefix->receiver=physical;prefix->identity=physical->identity();prefix->source_owner8=physical->source_owner8();
  const auto actual=std::weak_ptr<dh2::world::CanonicalProjectilePhysicalV112>(physical);const auto state=std::weak_ptr<dh2::loader::AuxiliaryResourcePrefixV82>(prefix);
  prefix->release_source=[actual,state](std::string& e){auto physical=actual.lock();auto prefix=state.lock();if(!physical||!prefix){e="Lost actual POProjectile release prefix";return false;}
   if(prefix->released){e.clear();return true;}if(!physical->destroy_source(e))return false;prefix->assigned=false;prefix->released=true;return true;};
  //Pin the actual failed C1 prefix BEFORE CreateBody/CreateShape can callback.
  journal->resources_v82.push_back(prefix);self->projectile_resources_v112.emplace(prefix->identity,prefix);
  receipt.backing=prefix;receipt.receiver=physical;receipt.identity=physical->identity();
  return physical->initialize(e);
 };
 s.set_physical=[weak](auto& p,auto& receipt,bool pinned,std::string& e){auto self=weak.lock();
  if(!self||!self->current(e)||!receipt.receiver||!receipt.backing){e="Required exact constructed POProjectile assignment";return false;}
  auto prefix=std::static_pointer_cast<dh2::loader::AuxiliaryResourcePrefixV82>(receipt.backing);
  auto physical=std::static_pointer_cast<dh2::world::CanonicalProjectilePhysicalV112>(receipt.receiver);
  if(prefix->identity!=receipt.identity||physical->identity()!=receipt.identity||!physical->constructed()||prefix->released){e="Changed POProjectile constructor receipt";return false;}
  auto world=std::static_pointer_cast<WorldScriptContext>(self->scope.actual_world.lock());std::uint32_t disabled{};
  if(!world||!world->debug||dh2_character_debug_load(world->debug.get(),&world->debug_files)!=1||dh2_character_debug_get(&disabled,world->debug.get(),"MP_NoPhysics",&world->debug_files)!=1){e="Required genuine SetPhysicalObject Debug prefix";return false;}
  if(disabled)return prefix->release_source(e); //source destroys NEW, leaves current2dc untouched
  auto* slot=p.base().pointer(0x2dc);if(!slot){e="Missing actual physical2dc slot";return false;}
  if(*slot!=receipt.identity){if(*slot){std::shared_ptr<void> pin;bool handled{};if(!self->destroy_owned_native_source_v92(p.base(),0x2dc,*slot,pin,handled,e)||!handled)return false;*slot=0;}
   *slot=receipt.identity;prefix->assigned=true;
   if(pinned&&dh2_native_body_pin(&physical->native())){e="Actual source PhysicalObject.pin failed";return false;}
  }
  return self->pf(p.base(),false,nullptr,0,e);
 };
 s.retire_assigned_physical_prefix=[weak](const auto& receipt,std::string& e){auto self=weak.lock();if(!self||!self->current(e)||!receipt||!receipt->backing)return false;
  auto prefix=std::static_pointer_cast<dh2::loader::AuxiliaryResourcePrefixV82>(receipt->backing);
  if(!prefix->assigned&&!prefix->released){e="Actual POProjectile has not reached source assignment/destruction";return false;}
  auto at=std::find(self->projectile_pending_prefixes_v112.begin(),self->projectile_pending_prefixes_v112.end(),receipt);
  if(at==self->projectile_pending_prefixes_v112.end()){e="Actual pending constructor receipt was replaced";return false;}
  self->projectile_pending_prefixes_v112.erase(at);
  if(prefix->released)return self->retire_projectile_prefix_storage_v112(prefix,e);
  e.clear();return true;};
 s.set_position=[](auto& p,const auto& position,bool destination,std::string& e){return p.set_position(position,destination,e);};
 s.set_destination=[](auto& p,const auto& xyz,std::string& e){auto* at=p.base().vector3(0x1a8);if(!at){e="Required SAME SetDestination1a8 cells";return false;}std::memcpy(at,xyz.data(),12);e.clear();return true;};
 s.set_rotation=[weak](auto& p,const auto& xyz,std::string& e){auto self=weak.lock();if(!self||!self->current(e))return false;
  dh2::loader::GameObjectSetRotationBorrowV69 b;b.receiver=self->files;b.rotation16c=p.base().vector3(0x16c);b.heading178=p.base().scalar(0x178);b.visual2d8=p.base().pointer(0x2d8);
  b.updating_visual_with_rotation=[](bool& value,std::string& e){value=true;e.clear();return true;}; //selected340074
  b.sync_rotation=[weak](std::uintptr_t id,const float*,std::string& e){auto self=weak.lock();std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> v;return self&&self->actual_visual(id,v,e)&&v->sync_rotation_v86(e);};
  return dh2::loader::game_object_set_rotation_v69(b,xyz.data(),e);};
 s.animator_virtual1c=[weak](std::uintptr_t id,unsigned group,unsigned loop,unsigned blend,unsigned flags,unsigned displacement,std::string& e){auto self=weak.lock();
  if(group||blend||flags||displacement){e="Required positive Projectile animator unsupported argument domain";return false;}
  std::shared_ptr<dh2::world::RetainedGenericAnimatorV21> animator;bool accepted{};return self&&self->animator(id,animator,e)&&animator->play_index(group,loop!=0,accepted,e);};
 s.animator_virtual18=[weak](std::uintptr_t id,unsigned group,std::int32_t& out,std::string& e){auto self=weak.lock();std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;
  return self&&self->actual_visual(id,visual,e)&&visual->source_current_clip_duration_v112(group,out,e);};
 s.visual_sync=[weak](std::uintptr_t id,std::string& e){auto self=weak.lock();std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;return self&&self->actual_visual(id,visual,e)&&visual->sync(e);};
 s.visual_sync_rotation=[weak](std::uintptr_t id,std::string& e){auto self=weak.lock();std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;return self&&self->actual_visual(id,visual,e)&&visual->sync_rotation_v86(e);};
 s.gameobject_update=[weak](auto& p,std::string& e){auto self=weak.lock();SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  return self&&self->current(e)&&borrow_source_campaign_candidate_v55(c,e)&&borrow_source_campaign_condition_world_v70(c,w,e)&&update_source_campaign_noncharacter_v104(w,p.base().identity(),e);};
 s.application=[weak](dh2::world::ProjectileApplicationV112& out,std::string& e){auto self=weak.lock();auto app=self?self->application.lock():nullptr;if(!self||!self->current(e)||!app)return false;
  out.owner=app;out.get_dt=[](std::uint32_t& dt,std::string& e){return borrow_application_dt_v93(dt,e);};return true;};
 s.pf_flying=[](auto& p,bool value,std::string& e){if(dh2_nav_object_set_flying(&p.runtime().object,value?1:0)){e="Actual PFObject.SetFlying failed";return false;}e.clear();return true;};
 s.pf_swimming=[](auto& p,bool value,std::string& e){if(dh2_nav_object_set_swimming(&p.runtime().object,value?1:0)){e="Actual PFObject.SetSwimming failed";return false;}e.clear();return true;};
 laser.set_flying=s.pf_flying;laser.set_swimming=s.pf_swimming;
 laser.character_from_handle=[weak](const auto& peer,std::uintptr_t& id,std::shared_ptr<void>& pin,std::string& e){auto self=weak.lock();auto manager=self?self->scope.objects.lock():nullptr;
  if(!self||!self->current(e)||!manager)return false;const dh2::world::CanonicalObjectBorrowV1* object{};std::int32_t key{};
  bool next=manager->source_ordered_begin_v38(key,object);while(next&&(!object||object->identity!=peer.identity))next=manager->source_ordered_next_v38(key,key,object);
  if(!next||!object||!object->shared_handle){e="Required actual peer Handle for Character conversion";return false;}
  auto handle=*object->shared_handle;const dh2::world::CanonicalObjectBorrowV1* resolved{};if(!manager->resolve_handle_v4(handle,false,resolved,{},e))return false;
  id=0;pin.reset();if(!resolved){e.clear();return true;}if(!resolved->as_character||!resolved->as_character(resolved->context,id,e))return false;if(id)pin=resolved->lease;return true;};
 laser.character_ai_is_enemy=[weak](std::uintptr_t owner,const auto& peer,bool& enemy,std::string& e){auto self=weak.lock();auto world=self?self->scope.actual_world.lock():nullptr;
  std::shared_ptr<dh2::character::skills::CharacterWorldRuntimeV1> targets;if(!self||!self->current(e)||!borrow_source_campaign_character_targets_v88(world,targets,e))return false;
  std::uintptr_t value{};if(targets->relationship(owner,peer.identity,true,&value)){e=targets->error();return false;}enemy=value!=0;return true;};
 laser.visual_set_visible=[weak](std::uintptr_t id,bool visible,std::string& e){auto self=weak.lock();std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;return self&&self->actual_visual(id,visual,e)&&visual->set_root_local_visibility_v3(visible,e);};
 laser.visual_set_parent=[weak](std::uintptr_t id,std::uintptr_t parent,std::string& e){auto self=weak.lock();std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;
  if(parent){e="Required positive Laser Visual.SetParent receiver";return false;}if(!self||!self->actual_visual(id,visual,e))return false;visual->source_set_parent_null_v112();e.clear();return true;};
 laser.visual_root8=[weak](std::uintptr_t id,std::uintptr_t& root,std::shared_ptr<void>& pin,std::string& e){auto self=weak.lock();std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;
  if(!self||!self->actual_visual(id,visual,e))return false;root=visual->root_identity();pin=visual;return true;};
 laser.visual_set_position=[weak](std::uintptr_t id,const auto& xyz,std::string& e){auto self=weak.lock();std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;return self&&self->actual_visual(id,visual,e)&&visual->source_root_set_position_v112(xyz.data(),e);};
 laser.root_virtual14=[weak](std::uintptr_t id,std::uint32_t time,std::string& e){auto self=weak.lock();std::vector<std::shared_ptr<dh2::world::RetainedGameObjectVisualV1>> visuals;
  if(!self||!self->current(e)||!self->graph->capture_visuals(visuals,e))return false;for(auto& visual:visuals)if(visual->root_identity()==id){
   SourceCampaignCandidateBorrowV55 current;if(!borrow_source_campaign_candidate_v55(current,e)||!current.roots)return false;
   return visual->source_scene_phase_v69(time,[roots=current.roots](std::string& e){roots->notify_visibility_changed_v3();e.clear();return true;},e);
  }
  e="Required SAME source Laser root.onAnimate receiver";return false;};
 auto assertion=dh2::world::SourceAssertionProcessV76::borrow();s.invalid_argument=[assertion](unsigned line,std::string& e){
  if(!assertion||!assertion->source_level()){e="Required actual Projectile assertion owner";return false;}auto mode=*assertion->source_level();
  if(mode==2){e="Original Projectile/Laser argument assertion fault";return false;}
  if(mode==1)return assertion->report("..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Projectiles\\Projectile.cpp",line,"projectile source parameters",e);
  e.clear();return true;};
 return bind_campaign_projectile_environment_v112(self,s,laser,e);
}
static bool publish_campaign_projectile_methods_v112(const std::shared_ptr<RendererCampaignNonCharacterV69>& self,
 const SourceCampaignCandidateBorrowV55& c,std::string& e){
 auto catalog=self?self->projectile_catalog_v111.lock():nullptr;if(!self||!catalog||!self->current(e))return false;
 SourceCampaignProjectileMethodsNativeV112 native;native.owner=self->files;
 const auto weak=std::weak_ptr<RendererCampaignNonCharacterV69>(self);const auto records=std::weak_ptr<dh2::loader::CatalogProjectilesV99>(catalog);
 native.borrow=[records](std::uintptr_t id,auto& out,std::string& e){auto catalog=records.lock();if(!catalog){e="Retired actual Projectile catalog";return false;}return catalog->borrow_projectile_v99(id,out,e);};
 native.lend=[weak](auto& p,auto& common,auto& laser,std::string& e){auto self=weak.lock();return self&&lend_campaign_projectile_methods_v112(self,p,common,laser,e);};
 native.visible=[weak](auto& p,bool visible,std::string& e){auto self=weak.lock();if(!self||!self->current(e)||!p.base().store_byte(0x80,visible?p.base().lifecycle().enabled8a:0,e))return false;return self->source_visibility(p.base(),e);};
 return install_source_campaign_projectile_methods_v112(c,std::move(native),e);
}
struct ProjectileFloorNativeV112 {std::shared_ptr<dh2::floors::World> world;std::uint32_t index{};};
struct ProjectileRoomNativeV112 {std::shared_ptr<dh2::world::ModulePFRoomV3> room;std::shared_ptr<dh2::floors::World> world;};
static bool bind_campaign_projectile_environment_v112(const std::shared_ptr<RendererCampaignNonCharacterV69>& self,
 dh2::world::ProjectileMethodServicesV112& s,dh2::world::LaserProjectileServicesV112& laser,std::string& e){
 const auto weak=std::weak_ptr<RendererCampaignNonCharacterV69>(self);
 auto floor_loan=[weak](std::uint32_t id,dh2::world::ProjectileFloorV112& out,std::string& e){auto self=weak.lock();auto world=self?self->floors.lock():nullptr;
  out={};if(!self||!self->current(e)||!world)return false;if(id==UINT32_MAX){e.clear();return true;}
  if(id>=world->records.size()||id>=world->collision_world.floor_count||!world->records[id]){e="Required SAME PF floor resource index";return false;}
  auto loan=std::make_shared<ProjectileFloorNativeV112>(ProjectileFloorNativeV112{world,id});
  out.owner=loan;out.identity=reinterpret_cast<std::uintptr_t>(world->records[id].get());out.flags24=&world->records[id]->flags.floor;e.clear();return true;};
 auto room_loan=[weak](std::uint32_t id,dh2::world::ProjectileRoomV112& out,std::string& e){auto self=weak.lock();auto rooms=self?self->rooms.lock():nullptr;auto floors=self?self->floors.lock():nullptr;
  out={};if(!self||!self->current(e)||!rooms||!floors)return false;if(id==UINT32_MAX){e.clear();return true;}
  if(id>=rooms->rooms().size()||!rooms->rooms()[id]||rooms->rooms()[id]->id!=id||id>=floors->collision_world.room_count){e="Required SAME PFRoom retained source resource";return false;}
  auto loan=std::make_shared<ProjectileRoomNativeV112>(ProjectileRoomNativeV112{rooms->rooms()[id],floors});
  out.owner=loan;out.identity=reinterpret_cast<std::uintptr_t>(loan->room.get());out.flags24=&loan->room->flags24;e.clear();return true;};
 s.world_floor_height=[weak,floor_loan](auto&,const auto& point,bool include_special,bool& found,float& height,auto& floor,std::string& e){auto self=weak.lock();auto world=self?self->floors.lock():nullptr;
  if(!self||!self->current(e)||!world)return false;dh2::navigation::HeightHit hit{};hit.height=height;
  const int status=dh2_nav_world_height(&hit,&world->collision_world,point.data(),include_special?1:0);
  if(status<0){e="Actual PFWorld.GetFloorHeightAt failed";return false;}found=status!=0;if(found)height=hit.height;
  return floor_loan(hit.floor,floor,e);};
 s.world_room_at=[weak,room_loan](auto&,const auto& point,auto& out,std::string& e){auto self=weak.lock();auto world=self?self->floors.lock():nullptr;
  if(!self||!self->current(e)||!world)return false;std::uint32_t selected=UINT32_MAX;
  const auto contains=[&](const dh2::octree::Box& box){for(unsigned k=0;k<3;++k)if(!(box.minimum[k]<=point[k]&&point[k]<=box.maximum[k]))return false;return true;};
  if(contains(world->collision_world.bounds))for(std::uint32_t i=0;i<world->collision_world.room_count;++i)if(contains(world->collision_world.rooms[i].bounds)){selected=i;break;}
  return room_loan(selected,out,e);};
 s.pf_room_floor=[room_loan,floor_loan](auto& p,auto& room,auto& floor,std::string& e){const auto& actual=p.runtime().object.motion;
  return room_loan(actual.room,room,e)&&floor_loan(actual.floor,floor,e);};
 s.can_path_on=[](auto& p,const auto& floor,bool& out,std::string& e){
  if(!floor.identity){out=false;e.clear();return true;}auto actual=std::static_pointer_cast<ProjectileFloorNativeV112>(floor.owner);
  if(!actual||!actual->world||actual->index>=actual->world->records.size()||reinterpret_cast<std::uintptr_t>(actual->world->records[actual->index].get())!=floor.identity||!floor.flags24){e="Foreign PFObject.CanPathOn floor loan";return false;}
  const auto type=*floor.flags24;out=!type||(p.runtime().object.motion.flags&type)==type;e.clear();return true;};
 s.floor_height=[](const auto& floor,const auto& point,float& height,std::string& e){auto actual=std::static_pointer_cast<ProjectileFloorNativeV112>(floor.owner);
  if(!actual||!actual->world||actual->index>=actual->world->collision_world.floor_count||
     reinterpret_cast<std::uintptr_t>(actual->world->records[actual->index].get())!=floor.identity){e="Foreign actual PFFloor.GetFloorHeightAt loan";return false;}
  dh2::navigation::HeightHit hit{};hit.height=height;const int status=dh2_nav_floor_height(&hit,actual->world->collision_world.floors[actual->index].selector,point.data());
  if(status<0){e="Actual PFFloor.GetFloorHeightAt failed";return false;}if(status)height=hit.height;e.clear();return true;};
 s.room_floor_height=[floor_loan](const auto& room,const auto& point,float& height,auto& floor,std::string& e){auto actual=std::static_pointer_cast<ProjectileRoomNativeV112>(room.owner);
  if(!actual||!actual->world||!actual->room||reinterpret_cast<std::uintptr_t>(actual->room.get())!=room.identity){e="Foreign actual PFRoom.GetFloorHeightAt loan";return false;}
  dh2::navigation::HeightHit hit{};hit.height=height;const int status=dh2_nav_room_height(&hit,&actual->world->collision_world,actual->room->id,point.data(),0);
  if(status<0){e="Actual PFRoom.GetFloorHeightAt failed";return false;}if(status)height=hit.height;return floor_loan(hit.floor,floor,e);};
 laser.play_anim_fx_set=[weak](std::int32_t id,const auto& point,std::uintptr_t anchor,std::uintptr_t data,std::string& e){auto self=weak.lock();auto world=self?self->scope.actual_world.lock():nullptr;
  if(!self||!self->current(e)||anchor||data){if(e.empty())e="Required positive Projectile effect anchor/set-data branch";return false;}
  std::shared_ptr<void> pin;dh2::fx::CharacterMeshFxOwnerV4* fx{};if(!borrow_source_campaign_fx_v77(world,pin,fx,e)||!fx)return false;
  return fx->play_set(id,point.data(),nullptr,0,nullptr,e);};
 auto fx=laser.play_anim_fx_set;
 //Keep one actual process owner; this is a row loan, no second table/parser.
 std::shared_ptr<dh2::android_ui::SourceProcessArraysV101> arrays;if(!borrow_process_spawn_arrays_v108(arrays,e))return false;
 s.impact_kind=[weak,arrays,fx](auto& p,std::int32_t kind,const auto& point,std::string& e){auto self=weak.lock();if(!self||!self->current(e))return false;
  if(kind==0){e.clear();return true;}dh2::world::ProjectileMethodServicesV112 rows;if(!bind_projectile_process_rows_v112(arrays,rows,e))return false;
  auto* cell=p.source_word_v99(0x374);if(!cell){e="Missing Projectile row374";return false;}std::int32_t index{};std::memcpy(&index,cell,4);
  dh2::world::ProjectileTableRowV112 actual;if(!rows.table_row(index,actual,e))return false;
  auto value=[&](unsigned offset,std::int32_t& out){std::uint32_t bits{};if(!actual.word(offset,bits,e))return false;std::memcpy(&out,&bits,4);return true;};
  std::int32_t effect{},sound{};
  if(kind==1){if(!value(8,effect)||!value(12,sound))return false;}
  else if(kind==2||kind==3){if(!value(0x30,sound))return false;
   if(sound==-1){if(!value(0x18,sound)||!value(0x14,effect))return false;}
   else {if(!value(0x2c,effect))return false;if(effect==-1&&!value(0x14,effect))return false;}
  }else if(!value(0x18,sound)||!value(0x14,effect))return false;
  if(effect!=-1&&!fx(effect,point,0,0,e))return false;
  if(sound!=-1){dh2::audio::AudioApplicationBorrowV42 manager;if(!borrow_actual_application_audio_v42(manager,e))return false;
   dh2::character::CombatSoundPlayV1 request;request.manager=manager.identity();request.target=p.base().identity();request.sound_id=sound;request.position=point;
   request.source_bool=false;request.source_integer=1;request.source_float0=request.source_float1=-1.f;
   if(!submit_campaign_audio_v46(dh2::audio::AudioCategoryV46::skill,manager,self->scope.actual_world.lock(),request,e))return false;
  }
  e.clear();return true;};
 const auto impact=s.impact_kind;const auto object=s.object;const auto target=s.target_position;const auto as_character=laser.character_from_handle;
 s.impact_object=[impact,object,target,as_character](auto& p,std::uintptr_t peer,const std::array<float,2>& xy,std::string& e){
  if(!peer){e="Original NULL HandleImpactFX peer would dereference after assertion";return false;}
  dh2::world::ProjectileObjectV112 actual;if(!object(peer,actual,e))return false;std::uintptr_t character{};std::shared_ptr<void> pin;
  if(!as_character(actual,character,pin,e))return false;dh2::world::ProjectilePointBorrowV112 point;if(!target(actual,point,e)||!point.owner||!point.xyz)return false;
  std::array<float,3> position{point.xyz[0],point.xyz[1],point.xyz[2]};
  if(!character){position[0]=xy[0];position[1]=xy[1];return impact(p,3,position,e);}return impact(p,4,position,e);};
 e.clear();return true;
}
