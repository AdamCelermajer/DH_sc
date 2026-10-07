// Include inside model_renderer namespace after public campaign helpers.
// Called at actual World enrollment, not first Character composition.
static bool install_campaign_no_room_primitive_v89(const std::shared_ptr<SourceWorldBorrowV61>& world,std::string& e){
 using namespace dh2;using namespace dh2::loader;
 if(!world||!world->application||!world->files_owner){e="Required actual World/App and independent native primitive authority";return false;}
 auto bag=world->stage34_native_v80?world->stage34_native_v80:std::make_shared<Stage34ServicesV1>();
 if(!bag->owner)bag->owner=world->files_owner;
 if(!bag->handle_no_room_objects)bag->handle_no_room_objects=[weak=std::weak_ptr<SourceWorldBorrowV61>(world)](
  const std::shared_ptr<dh2::world::CanonicalObjectManagerV1>& manager,std::string& e){
  auto scope=weak.lock();SourceCampaignCandidateBorrowV55 actual;
  if(!scope||!scope->canonical_world||!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world.get()!=scope->owner.get()||
     actual.actual_world.owner_before(scope->owner)||scope->owner.owner_before(actual.actual_world)||actual.objects!=manager||manager!=scope->canonical_world->manager_lease){if(e.empty())e="Required SAME actual App38 manager before HandleNoRoomObjects";return false;}
  auto transport=actual.receiver_transport_v69.lock();
  if(!transport){e="Required existing canonical receiver transport for virtual20";return false;}
  const auto services=transport->services();
  if(!services.is_game_object){e="Required actual source receiver IsGameObject dispatch";return false;}
  // Captured transport/actual receipts pin live constructor graph throughout
  // this synchronous body; no record/factory/World is retained on the manager.
  return manager->source_handle_no_room_objects_v89([transport,services](const auto& actor,bool& value,std::string& e){
   return services.is_game_object(services.context,actor,value,e);
  },e);
 };
 world->stage34_native_v80=std::move(bag);e.clear();return true;
}
