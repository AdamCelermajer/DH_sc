// Inside model_renderer AFTER V69 and CampaignNonCharacterReleaseV92.
// Public headers are included at global namespace by model_renderer.cpp.
// Uses the original loader family and SAME V69 graph/journal, no native C1.
static bool append_campaign_projectile_catalog_v111(
 const std::shared_ptr<RendererCampaignNonCharacterV69>& self,
 const SourceCampaignCandidateBorrowV55& actual,
 dh2::loader::ProductionNonCharacterServicesV67& classes,std::string& e){
 if(!self||!self->current(e)||actual.actual_world!=self->scope.actual_world.lock()||!self->graph||!self->projectile_catalog_v111.expired()||
    !classes.auxiliary.retain_release_record||!classes.auxiliary.release_completed){
  if(e.empty())e="Required once-composed Projectile family/actual resource journal";return false;
 }
 auto scope=self->scope;scope.services_owner=self;
 dh2::loader::CatalogProjectileServicesV99 services;services.owner=self;
 services.actual_graph=self->graph;
 const auto weak=std::weak_ptr<RendererCampaignNonCharacterV69>(self);
 services.validate_current=[weak](const dh2::loader::AuxiliaryDeliveryV67& delivery,std::string& e){
  auto self=weak.lock();if(!self||!self->current(e))return false;
  if(delivery.actual_world!=self->scope.actual_world.lock()||delivery.level!=self->scope.level.lock()||
     delivery.objects!=self->scope.objects.lock()||delivery.properties!=self->scope.properties){
   e="Projectile family received foreign native candidate fields";return false;
  }
  e.clear();return true;
 };
 services.retain_release_record=classes.auxiliary.retain_release_record;
 services.release_completed=classes.auxiliary.release_completed;
 services.lend_platform=[weak](const dh2::loader::AuxiliaryDeliveryV67& delivery,
  dh2::world::CanonicalProjectileV99& projectile,
  dh2::world::GameObjectInitializationServicesV1& init,
  dh2::world::GameObjectSetPositionServicesV2& position,
  std::shared_ptr<dh2::world::ProjectileResourceServicesV99>& resources,std::string& e){
  auto self=weak.lock();RendererCampaignNonCharacterV69::Bound* bound{};
  if(!self||!self->current(e)||delivery.actual_world!=self->scope.actual_world.lock()||
     !self->ensure(projectile.base(),bound,e,false))return false;
  //CatalogProjectiles owns its one graph.bind AFTER this service loan. V69
  //prepares the original condition/random/PF/light/position leaves only.
  init=bound->initialization;position=bound->position;
  auto owner=std::make_shared<dh2::world::ProjectileResourceServicesV99>();owner->owner=self->files;
  const auto id=projectile.base().identity();
  owner->current=[weak,id](dh2::world::CanonicalProjectileV99& same,std::string& e){
   auto self=weak.lock();std::shared_ptr<void> pin;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
   if(!self||same.base().identity()!=id||!self->borrow(id,pin,base,e)||base!=&same.base()){
    if(e.empty())e="Replaced actual Projectile native resource receiver";return false;
   }
   e.clear();return true;
  };
  SourceCampaignCandidateBorrowV55 current;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!borrow_source_campaign_candidate_v55(current,e)||current.actual_world!=delivery.actual_world||
     !borrow_source_campaign_condition_world_v70(current,w,e)||!w->noncharacter_release_provider_v92){
   if(e.empty())e="Required actual Projectile qualified GameObjectD2 resource owner";return false;
  }
  //Use the existing complete qualified GameObjectD2 exactly once. The V99
  //receiver tracks its reached retirement prefix; the shared body owns the
  //actual visual/physics/Lua/PF/Condition/sound order and native resources.
  const auto world=std::weak_ptr<void>(delivery.actual_world);
  owner->game_object_d2=[world](auto& base,std::string& e){auto actual=world.lock();
   if(!actual){e="Retired SAME Projectile GameObjectD2 World";return false;}
   return source_campaign_gameobject_destroy_v107(actual,base,e);
  };
  const auto assertion=dh2::world::SourceAssertionProcessV76::borrow();
  owner->null_manager_assertion_mode=[assertion](std::int32_t& mode,std::string& e){
   if(!assertion||!assertion->source_level()){e="Required same process Projectile SetManager assertion mode";return false;}
   mode=*assertion->source_level();e.clear();return true;
  };
  owner->report_null_manager_assertion=[assertion](dh2::world::CanonicalProjectileV99&,std::string& e){
   return assertion&&assertion->report("..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Projectiles\\Projectile.cpp",0x44,"mgr",e);
  };
  //Inherited Projectile.InitPost does not construct a POProjectile. Positive
  //SetInfo physical ownership is published by that original later producer.
  resources=std::move(owner);e.clear();return true;
 };
 dh2::loader::PartV67 part;std::shared_ptr<dh2::loader::CatalogProjectilesV99> catalog;
 if(!dh2::loader::make_projectile_catalog_part_v99(std::move(scope),std::move(services),part,e,&catalog))return false;
 self->projectile_catalog_v111=catalog;classes.additional_parts_v101.push_back(std::move(part));
 e.clear();return true;
}
static bool publish_campaign_projectile_backend_v111(
 const std::shared_ptr<RendererCampaignNonCharacterV69>& self,
 const SourceCampaignCandidateBorrowV55& actual,std::string& e){
 auto catalog=self?self->projectile_catalog_v111.lock():nullptr;
 if(!self||!self->current(e)||!catalog){if(e.empty())e="Required actual composed Projectile canonical family";return false;}
 SourceCampaignProjectileClassServicesV111 source;source.owner=catalog;
 const auto weak=std::weak_ptr<dh2::loader::CatalogProjectilesV99>(catalog);
 source.borrow=[weak](const SourceCampaignCandidateBorrowV55&,
  const dh2::world::CanonicalObjectBorrowV1& object,bool laser,
  SourceCampaignProjectileClassLoanV111& out,std::string& e){
  auto catalog=weak.lock();std::shared_ptr<dh2::world::CanonicalProjectileV99> projectile;
  if(!catalog||!catalog->borrow_projectile_v99(object.identity,projectile,e)||!projectile||
     projectile->is_laser_type()!=laser||projectile.owner_before(object.lease)||object.lease.owner_before(projectile)){
   if(e.empty())e="Required SAME type-matched actual Projectile/Laser class loan";return false;
  }
  out.receiver=projectile;out.base=&projectile->base();
  const auto receiver=std::weak_ptr<dh2::world::CanonicalProjectileV99>(projectile);
  out.set_manager3e4fe4=[receiver](std::uintptr_t manager,std::string& e){auto actual=receiver.lock();
   if(!actual){e="Retired actual Projectile.SetManager receiver";return false;}return actual->set_manager(manager,e);};
  e.clear();return true;
 };
 return install_source_campaign_projectile_backend_v111(actual,std::move(source),e)&&publish_campaign_projectile_methods_v112(self,actual,e);
}
