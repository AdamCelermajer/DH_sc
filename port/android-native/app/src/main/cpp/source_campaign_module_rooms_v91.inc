// Include in source_campaign_runtime_v61.cpp's model_renderer namespace after
// private source_campaign_v55 and candidate types, beside module_conditions.
// Include source_campaign_module_rooms_v91.hpp before opening the namespace.
bool bind_source_campaign_module_room_zones_v91(const SourceCampaignCandidateBorrowV55& expected,
 decltype(dh2::world::CanonicalModuleGraphServicesV3::spawn_zone) spawn,
 decltype(dh2::world::CanonicalModuleGraphServicesV3::zone_init) init,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;
 if(!borrow_source_campaign_condition_world_v70(expected,world,e))return false;
 auto& state=*source_campaign_v55;
 if(state.failed||expected.level->constructor_fields_v3().field130>=7||!state.candidate||!state.candidate->graph){e="Generated rooms must bind actual runtime Module graph before XML C1";return false;}
 return state.candidate->graph->bind_generated_room_services_v91(std::move(spawn),std::move(init),e);
}
bool borrow_source_campaign_module_v91(const std::shared_ptr<void>& world,std::uintptr_t id,
 std::shared_ptr<void>& pin,dh2::world::CanonicalModuleV1*& actual,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;if(!borrow_source_campaign_candidate_runtime_v61(candidate,e))return false;
 if(!world||world.get()!=candidate.actual_world.get()||world.owner_before(candidate.actual_world)||candidate.actual_world.owner_before(world)||!source_campaign_v55->candidate->graph){e="Module borrow requires SAME actual runtime World/graph";return false;}
 return source_campaign_v55->candidate->graph->module_receiver_v91(id,pin,actual,e);
}
bool borrow_source_campaign_module_visited_v91(const std::shared_ptr<void>& world,std::uintptr_t id,
 std::shared_ptr<void>& pin,std::uint8_t*& byte,std::string& e){
 dh2::world::CanonicalModuleV1* actual{};if(!borrow_source_campaign_module_v91(world,id,pin,actual,e))return false;byte=&actual->source_visited3fc_v91();return true;
}

bool source_campaign_module_sync_visibility_v94(const std::shared_ptr<void>& world,
 std::uintptr_t id,std::string& e){
 std::shared_ptr<void> pin;dh2::world::CanonicalModuleV1* module{};
 if(!borrow_source_campaign_module_v91(world,id,pin,module,e)||!pin||!module)return false;
 auto graph=source_campaign_v55&&source_campaign_v55->candidate?source_campaign_v55->candidate->graph:nullptr;
 if(!graph){e="Required SAME retained Module graph for native SyncVisibility";return false;}
 // The actual graph pins its own SAME weak record, rereads2d8 and runs original
 // Module Visual4713d0/RootSetVisible. No generic V69 visual lookup or new tick.
 return graph->source_sync_visibility_v94(id,e);
}
