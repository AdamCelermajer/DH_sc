// Include inside model_renderer namespace after V69 composition, with
// source_campaign_module_rooms_v91.hpp and module_room_zone_connection_v91.hpp
// included outside the namespace. Caller retains connection as World sibling;
// its independent producer/services must not retain it strongly.
bool bind_campaign_generated_module_rooms_v91(const SourceCampaignCandidateBorrowV55& actual,
 dh2::loader::ScopeV67 scope,dh2::loader::ModuleRoomZoneLeavesV91 leaves,
 const std::shared_ptr<dh2::loader::SourceReleaseJournalV69>& journal,
 std::shared_ptr<dh2::loader::ModuleRoomZoneConnectionV91>& sibling,std::string& e){
 if(sibling){e="Generated rooms already composed for this Level";return false;}
 auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
 if(!same(scope.actual_world.lock(),actual.actual_world)||!same(scope.level.lock(),actual.level)||!same(scope.objects.lock(),actual.objects)||scope.properties!=actual.properties){e="Generated room source scope differs from actual campaign";return false;}
 if(leaves.module_visited_v104){e="Generated room Module visit borrower must bind once from actual runtime graph";return false;}
 std::weak_ptr<void> containing=actual.actual_world;
 leaves.module_visited_v104=[containing](std::uintptr_t id,std::uint8_t*& byte,std::shared_ptr<void>& pin,std::string& e){auto world=containing.lock();if(!world){e="Required SAME actual World for Module3fc borrower";return false;}return borrow_source_campaign_module_visited_v91(world,id,pin,byte,e);};
 // Retain failed publication prefix in external sibling, never retry fresh C1.
 if(!dh2::loader::ModuleRoomZoneConnectionV91::create(std::move(scope),actual.receiver_transport_v69,journal,std::move(leaves),sibling,e))return false;
 std::weak_ptr<dh2::loader::ModuleRoomZoneConnectionV91> weak=sibling;
 return bind_source_campaign_module_room_zones_v91(actual,
  [weak](const std::string& name,dh2::target_providers::Handle16& h,std::uint32_t& type,std::string& e){auto self=weak.lock();if(!self){e="Required live SAME generated room source owner";return false;}return self->spawn(name,h,type,e);},
  [weak](dh2::target_providers::Handle16& h,const std::array<float,6>& box,std::uintptr_t module,std::string& e){auto self=weak.lock();if(!self){e="Required live SAME generated room source owner";return false;}return self->init(h,box,module,e);},e);
}
// Used by Parent's existing Module/Block OBJS delivery, not a second directory.
bool make_campaign_module_saved_connection_v91(const std::shared_ptr<void>& actual_world,std::uintptr_t id,
 const std::shared_ptr<dh2::loader::ModuleRoomZoneConnectionV91>& rooms,
 dh2::loader::NonCharacterRestoreServicesV89 leaves,
 std::shared_ptr<dh2::loader::NonCharacterSaveConnectionV89>& out,std::string& e){
 std::shared_ptr<void> pin;dh2::world::CanonicalModuleV1* module{};
 if(!rooms||!borrow_source_campaign_module_v91(actual_world,id,pin,module,e)||!pin||!module){if(e.empty())e="Required actual Module record/RoomZone save owner";return false;}
 std::weak_ptr<dh2::loader::ModuleRoomZoneConnectionV91> weak=rooms;
 leaves.module_room_set_visited=[weak](dh2::target_providers::Handle16& h,std::uint8_t& value,std::string& e){auto self=weak.lock();if(!self){e="Required live SAME Module RoomZone restore owner";return false;}return self->restore_module_visited(h,value,e);};
 auto alias=std::shared_ptr<dh2::world::CanonicalModuleV1>(pin,module);
 return dh2::loader::make_noncharacter_receiver_save_v89(alias,[](auto& actual,auto& fields,std::string& e){return dh2::loader::module_save_fields_v91(actual,fields,e);},std::move(leaves),out,e);
}
