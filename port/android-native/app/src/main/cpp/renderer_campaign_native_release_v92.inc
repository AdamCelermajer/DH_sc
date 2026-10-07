// Inside model_renderer AFTER V69 and BEFORE native lenders/producer.
// Outside namespace include noncharacter_native_release_v92.hpp,
// canonical_room_zone_factory_v3.hpp, canonical_light_point_factory_v53.hpp.
struct CampaignNonCharacterReleaseV92 final:std::enable_shared_from_this<CampaignNonCharacterReleaseV92> {
 dh2::loader::ScopeV67 scope;std::weak_ptr<dh2::loader::CanonicalReceiverTransportV1> transport;
 std::weak_ptr<RendererCampaignNonCharacterV69> composition;
 std::weak_ptr<dh2::loader::SourceReleaseJournalV69> journal;
 dh2::loader::NonCharacterNativeReleasePrimitivesV92 native;
 bool current(SourceCampaignCandidateBorrowV55& actual,std::string& e)const{
  auto world=scope.actual_world.lock();auto level=scope.level.lock();auto manager=scope.objects.lock();
  if(!world||!level||!manager||!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world.get()!=world.get()||actual.level!=level||actual.objects!=manager||actual.properties!=scope.properties||world.owner_before(actual.actual_world)||actual.actual_world.owner_before(world)){if(e.empty())e="Required SAME live native release scope";return false;}return true;
 }
 bool bind_journal(const std::shared_ptr<dh2::loader::SourceReleaseJournalV69>& actual,std::string& e){auto manager=scope.objects.lock();if(!journal.expired()||!actual||!manager||!actual->same_manager(*manager)){e="Native release journal must be SAME manager sibling, bound once";return false;}journal=actual;e.clear();return true;}
 bool attach(const SourceCampaignCandidateBorrowV55& expected,const RendererCampaignNonCharacterCompositionV69& actual,std::string& e){
  SourceCampaignCandidateBorrowV55 now;if(!current(now,e)||!composition.expired()||!actual.services||now.actual_world!=expected.actual_world||now.level!=expected.level||!actual.services->current(e)){if(e.empty())e="Native release must attach SAME V69 composition once before XML";return false;}
  composition=actual.services;e.clear();return true;
 }
 bool services(dh2::world::CanonicalGameObjectBaseOwnerV1& base,dh2::world::ColBoxServicesV71& out,std::string& e){
  SourceCampaignCandidateBorrowV55 actual;auto graph=composition.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* same{};
  if(!current(actual,e)||!graph||!graph->borrow(base.identity(),pin,same,e)||same!=&base||!native.owner){if(e.empty())e="Required SAME actual class/native release authority";return false;}
  if(!native.require_cleanup_delivery||!native.require_cleanup_delivery(base.identity(),e)){if(e.empty())e="Required Main owning-thread frame/contact/scene delivery barrier before D0";return false;}
  out=native.gameobject;out.owner=native.owner;std::weak_ptr<CampaignNonCharacterReleaseV92> weak=shared_from_this();
  out.validate_current=[weak](auto& base,std::string& e){auto self=weak.lock();auto graph=self?self->composition.lock():nullptr;SourceCampaignCandidateBorrowV55 actual;std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* same{};return self&&self->current(actual,e)&&graph&&graph->borrow(base.identity(),pin,same,e)&&same==&base;};
  auto other=out.destroy_native;
  out.destroy_native=[weak,other](auto& base,std::uint32_t offset,std::uintptr_t resource,std::shared_ptr<void>& pin,std::string& e){auto self=weak.lock();auto graph=self?self->composition.lock():nullptr;bool handled{};
   if(!self||!graph||!graph->destroy_owned_native_source_v92(base,offset,resource,pin,handled,e))return false;if(handled)return true;
   if(!other){e="Required real Main native D0 for unknown assigned visual/physical/attached domain";return false;}return other(base,offset,resource,pin,e);};
  auto other_prefix=out.destroy_unpublished_native_prefixes_v92;
  out.destroy_unpublished_native_prefixes_v92=[weak,other_prefix](auto& base,std::uint32_t offset,std::string& e){auto self=weak.lock();auto graph=self?self->composition.lock():nullptr;if(!self||!graph||!graph->discard_owned_unpublished_source_v92(base,offset,e))return false;return !other_prefix||other_prefix(base,offset,e);};
  if(!out.destroy_target_list)out.destroy_target_list=[](auto& base,std::string& e){return base.source_target_list304_v107().destroy(e);};
  if(!out.destroy_pf_object)out.destroy_pf_object=[weak,id=base.identity()](dh2::actor::RuntimeState& runtime,std::string& e){
   auto self=weak.lock();auto graph=self?self->composition.lock():nullptr;SourceCampaignCandidateBorrowV55 actual;
   std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
   if(!self||!graph||!self->current(actual,e)||!graph->borrow(id,pin,base,e)||!base||&base->runtime()!=&runtime||!actual.floors||!actual.navigation_registry){if(e.empty())e="Required SAME actual PFObject/registry at source D1";return false;}
   if(dh2_nav_drop_path(&runtime.path)){e="Actual PFObject DropPath rejected its live path storage";return false;}
   const dh2::navigation::ObstacleInitRequest request{&actual.floors->collision_world,&actual.navigation_registry->registry(),&runtime.object,id,0.f,0.f,0,0};
   if(dh2_nav_init_obstacle(&request)){e="Actual PFObject D1 obstacle unregistration failed";return false;}
   // Native route workspace is caller-owned, unlike original private80 small
   // buffer/list. Do not free a borrowed segment pointer or alias a second PF.
   runtime.path.segments=nullptr;runtime.path.capacity=0;e.clear();return true;
  };
  if(!out.stop_sound)out.stop_sound=[weak](std::int16_t sound,std::int32_t fade,std::string& e){
   //369ff4/36a008: actual signed negative Stop ID is a complete source return.
   if(sound<0){e.clear();return true;}auto self=weak.lock();SourceCampaignCandidateBorrowV55 actual;
   if(!self||!self->current(actual,e))return false;
   return stop_campaign_sound_v106(actual.actual_world,sound,fade,e);
  };
  auto prior_objectbase=out.destroy_object_base;
  out.destroy_object_base=[weak,prior_objectbase](auto& base,std::string& e){auto self=weak.lock();if(!self){e="Actual native release owner retired";return false;}
   if(prior_objectbase)return prior_objectbase(base,e);
   auto actual=self->native.object_base;auto graph=self->composition.lock();if(!graph){e="Actual same-class condition owner retired";return false;}
   if(!actual.owner)actual.owner=self->native.owner;
   if(!actual.conditions.owner)actual.conditions=graph->native.conditions;
   else {const auto& same=graph->native.conditions;if(actual.conditions.owner.owner_before(same.owner)||same.owner.owner_before(actual.conditions.owner)||actual.conditions.conditions!=same.conditions){e="ObjectBaseD2 must borrow SAME actual compiled-condition arena/table";return false;}}
   return dh2::loader::object_base_source_destroy_v92(base,actual,e);};
  return true;
 }
 bool destroy_gameobject(dh2::world::CanonicalGameObjectBaseOwnerV1& base,std::string& e){
  dh2::world::ColBoxServicesV71 exact;if(!services(base,exact,e))return false;std::shared_ptr<void> physical;
  auto world=scope.actual_world.lock();if(!world||!retire_source_campaign_batch_object_v113(world,base.identity(),e))return false;
  return dh2::world::game_object_source_destroy_v72(base,exact,physical,e);
 }
 bool openable_d0(const std::shared_ptr<dh2::world::CanonicalOpenableGraphV21>& actual,std::string& e){
  if(!actual){e="Required SAME actual admitted Openable graph";return false;}auto graph=composition.lock();
  if(!graph){e="Actual V69 release composition retired";return false;}
  auto i=graph->openable_graphs_v89.find(actual->receiver().base().identity());
  if(i==graph->openable_graphs_v89.end()||i->second.lock()!=actual){e="Openable D0 must use admitted SAME graph identity";return false;}
  dh2::world::ColBoxServicesV71 exact;if(!services(actual->receiver().base(),exact,e))return false;
  auto world=scope.actual_world.lock();if(!world||!retire_source_campaign_batch_object_v113(world,actual->receiver().base().identity(),e))return false;
  return dh2::loader::openable_source_destroy_v92(*actual,exact,native.container_network,e);
 }
 bool aliases_unpublished(std::uintptr_t id,std::string& e){
  SourceCampaignCandidateBorrowV55 actual;auto graph=composition.lock();
  if(!current(actual,e)||!graph||!native.require_external_aliases_unpublished||!native.require_external_aliases_unpublished(id,e)){if(e.empty())e="Required real Main draw/contact/network/observer alias absence";return false;}
  auto actual_journal=journal.lock();if(!actual_journal){e="Actual source release journal sibling missing";return false;}
  // LightPoint owns ObjectBase and its own real light/node/D0 journal, not the
  // generic GameObject graph. Source journal already checks completed native
  // light teardown; Main's alias query covers its scene/group/native facets.
  if(!actual_journal->light_identity(id)&&!graph->require_owned_aliases_unpublished_v92(id,e))return false;
  // Cache unpublication is a mutation prefix: Main rechecks actual outside
  // aliases afterwards; no weak_ptr expiry is a native quiescence certificate.
  return native.require_external_aliases_unpublished(id,e);
 }
 bool failed_constructor(const dh2::loader::SourceConstructorPrefixBorrowV89& p,std::string& e){
  SourceCampaignCandidateBorrowV55 actual;auto receiver_transport=transport.lock();
  if(!current(actual,e)||!receiver_transport||!p.record||p.constructor_state!=dh2::loader::CanonicalConstructorStateV89::failed||!p.class_name){if(e.empty())e="Required qualified SAME failed C1 record";return false;}
  if(!native.require_constructor_delivery||!native.require_constructor_delivery(p,e)){if(e.empty())e="Required actual constructor-frame/observer delivery barrier";return false;}
  if(receiver_transport->source_record_published_v92(p.record)){e="Actual transport still publishes failed constructor record";return false;}
  std::int32_t key{};const dh2::world::CanonicalObjectBorrowV1* object{};bool more=actual.objects->source_ordered_begin_v38(key,object);
  while(more){if(object&&object->lease&&!object->lease.owner_before(p.record)&&!p.record.owner_before(object->lease)){e="Actual manager publishes failed C1 control block";return false;}more=actual.objects->source_ordered_next_v38(key,key,object);}
  bool host_only=false;
#define PURE_C1(name,Record) if(!std::strcmp(p.class_name,name)){auto record=std::static_pointer_cast<dh2::loader::Record>(p.record);host_only=record&&record->constructor_state==dh2::loader::CanonicalConstructorStateV89::failed&&!record->owner;}
  PURE_C1("Dummy",CanonicalDummyRecordV16) PURE_C1("SpawnPoint",CanonicalSpawnPointRecordV16)
  PURE_C1("Decor",CanonicalDecorRecordV16) PURE_C1("AnimatedDecor",CanonicalAnimatedDecorRecordV23)
  PURE_C1("TriggerZone",CanonicalTriggerZoneRecordV22) PURE_C1("CheckpointZone",CanonicalCheckpointRecordV26)
  PURE_C1("Door",CanonicalDoorRecordV27) PURE_C1("TriggerObject",CanonicalTriggerObjectRecordV28)
  PURE_C1("TriggerZoneExitLevel",CanonicalExitZoneRecordV29) PURE_C1("QuestMoveInZone",CanonicalQuestMoveRecordV31)
  PURE_C1("SoundEmitter",CanonicalSoundEmitterRecordV32) PURE_C1("LightPoint",CanonicalLightPointRecordV53)
  PURE_C1("Projectile",CanonicalProjectileRecordV99) PURE_C1("LaserTypeProjectile",CanonicalProjectileRecordV99)
#undef PURE_C1
  if(!std::strcmp(p.class_name,"RoomZone")){auto record=std::static_pointer_cast<dh2::world::CanonicalRoomZoneRecordV3>(p.record);host_only=record&&record->constructor_started_v91&&record->constructor_failed_v91&&!record->constructor_completed_v91&&!record->receiver;}
  if(p.auxiliary_v92){if(p.auxiliary_v92->record.get()!=p.record.get()||p.auxiliary_v92->record.owner_before(p.record)||p.record.owner_before(p.auxiliary_v92->record)){e="Foreign source constructor journal qualification";return false;}
   for(const auto& resource:p.auxiliary_v92->resources_v82)if(resource&&!resource->released)host_only=false;
  }
  if(host_only){
   // These retained source C1s construct only host-owned base/string/member
   // storage; native scene/physics/Lua setup occurs AFTER C1 in InitPost.
   // Actual factory catch records failed ONLY after make_unique's C++ unwind.
   // No owner was published and no unreleased native prefix remains. This is
   // host prefix retirement, not an invented D0 for an unconstructed actor.
   e.clear();return true;
  }
  if(!native.unwind_constructor_prefix){e="Required real Main record-qualified native constructor/resource prefix unwind";return false;}
  return native.unwind_constructor_prefix(p,e);
 }
};
bool prepare_campaign_native_release_v92(const SourceCampaignCandidateBorrowV55& actual,
 dh2::loader::NonCharacterNativeReleasePrimitivesV92 primitives,
 std::shared_ptr<CampaignNonCharacterReleaseV92>& sibling,
 dh2::loader::SourceReleaseJournalLeavesV69& leaves,std::string& e){
 if(sibling||!actual.actual_world||!actual.level||!actual.objects||actual.receiver_transport_v69.expired()||!primitives.owner){e="Required fresh independent native release authority/scope";return false;}
 auto same=[](const auto& a,const auto& b){return a&&b&&!a.owner_before(b)&&!b.owner_before(a);};
 if(same(primitives.owner,actual.actual_world)||same(primitives.owner,actual.level)||same(primitives.owner,actual.objects)){e="Native release authority must not retain containing scope";return false;}
 auto self=std::make_shared<CampaignNonCharacterReleaseV92>();self->scope={actual.actual_world,actual.level,actual.objects,actual.properties,primitives.owner};self->transport=actual.receiver_transport_v69;self->native=std::move(primitives);
 std::weak_ptr<CampaignNonCharacterReleaseV92> weak=self;leaves={};leaves.owner=self->native.owner;
 leaves.require_native_aliases_unpublished=[weak](auto id,std::string& e){auto self=weak.lock();if(!self){e="Actual native release sibling retired";return false;}return self->aliases_unpublished(id,e);};
 leaves.openable_d0=[weak](const auto& graph,std::string& e){auto self=weak.lock();if(!self){e="Actual Openable native release sibling retired";return false;}return self->openable_d0(graph,e);};
 leaves.unwind_failed_constructor_prefix=[weak](const auto& prefix,std::string& e){auto self=weak.lock();if(!self){e="Actual failed C1 release sibling retired";return false;}return self->failed_constructor(prefix,e);};
 sibling=std::move(self);e.clear();return true;
}
// Call from CURRENT CampaignNativeLendersV89.make_lender after its actual
// original lender, before V69 boundary. Do not shortcut Zone/Trigger prefixes.
template<class Services>void bind_auxiliary_gameobject_destroy_v92(std::weak_ptr<CampaignNonCharacterReleaseV92> weak,Services& output){
 auto body=[weak](dh2::world::CanonicalGameObjectBaseOwnerV1& base,std::string& e){auto self=weak.lock();if(!self){e="Actual non-Character D2 release sibling retired";return false;}return self->destroy_gameobject(base,e);};
 if constexpr(std::is_same_v<Services,dh2::world::DummyContinuationServicesV14>||std::is_same_v<Services,dh2::world::SpawnPointServicesV15>||std::is_same_v<Services,dh2::world::DecorServicesV15>||std::is_same_v<Services,dh2::world::DestructibleContainerServicesV16>){if(!output.destroy_base)output.destroy_base=body;}
 if constexpr(std::is_same_v<Services,dh2::world::AnimatedDecorServicesV23>){if(!output.decor.destroy_base)output.decor.destroy_base=body;}
 if constexpr(std::is_same_v<Services,dh2::world::DestructibleContainerServicesV16>){if(!output.destroy_network)output.destroy_network=[weak](auto& network,std::uint32_t offset,std::string& e){auto self=weak.lock();if(!self||!self->native.container_network){e="Required actual positive Container native network tree D0";return false;}return self->native.container_network(network,offset,e);};}
}

bool source_campaign_gameobject_destroy_v107(const std::shared_ptr<void>& world,
 dh2::world::CanonicalGameObjectBaseOwnerV1& base,std::string& e){
 SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
 if(!borrow_source_campaign_candidate_v55(c,e)||c.actual_world!=world||!borrow_source_campaign_condition_world_v70(c,w,e)||!w->noncharacter_release_provider_v92){if(e.empty())e="Required actual source-qualified GameObjectD2 owner";return false;}
 return w->noncharacter_release_provider_v92->destroy_gameobject(base,e);
}
bool source_campaign_room_destroy_v106(const std::shared_ptr<void>& world,
 dh2::world::CanonicalRoomZoneV3& room,std::string& e){
 SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
 if(!borrow_source_campaign_candidate_v55(c,e)||c.actual_world!=world||!borrow_source_campaign_condition_world_v70(c,w,e)||!w->noncharacter_release_provider_v92||!require_source_campaign_class_delivery_v106(world,e))return false;
 auto release=w->noncharacter_release_provider_v92;const std::weak_ptr<CampaignNonCharacterReleaseV92> weak=release;
 dh2::world::RoomDestroyServicesV106 s;s.owner=release->native.owner;
 s.game_object_d2=[weak](auto& base,auto& e){auto self=weak.lock();if(!self){e="Retired Room qualified base D2 owner";return false;}return self->destroy_gameobject(base,e);};
 s.node384_drop=[](std::uintptr_t,std::string& e){e="Positive Room Zone node384 needs its actual retained colzone reference lease";return false;};
 return room.source_destroy_v106(s,e);
}
