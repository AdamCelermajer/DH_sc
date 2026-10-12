
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
