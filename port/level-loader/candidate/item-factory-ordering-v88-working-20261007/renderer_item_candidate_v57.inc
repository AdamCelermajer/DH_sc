// Include after RendererCanonicalLootConnectionV44 and the actual candidate
// definitions. Caller supplies its existing application Item owner lifetime.
// This route deliberately cannot create a pool before actual Level stage29.
static bool renderer_source_item_receiver_v57(RendererCanonicalLootConnectionV44& loot,
 const std::shared_ptr<void>& actual_item_owner_lease,
 RendererCanonicalWorldV4& candidate,const dh2::world::CanonicalFactoryEntryV1& entry,
 const dh2::world::CanonicalSourceObjectRequestV1& request,
 dh2::world::CanonicalClassReceiverV1& out,std::string& e){
 auto* gameplay=loot.gameplay();auto* owner=gameplay?gameplay->owner():nullptr;
 if(!actual_item_owner_lease||!owner||!owner->ready()||!owner->items()){
  e="Required actual application Item owner from completed source stage29; no early Item145 construction";return false;
 }
 dh2::world::CanonicalItemCandidateTransportV57 transport(*owner->items(),candidate.manager,candidate.properties,actual_item_owner_lease);
 return transport.construct(entry,request,out,e);
}

// Narrow actual-source admission after empty ItemManager/factory preparation.
// Legacy stage29 route above remains intact. This never constructs a second
// owner/pool, starts PreCache, calls InitOnce or replays inherited InitPost.
static bool renderer_source_item_receiver_v88(dh2::character::WorldLootGameplayV23& same,
 const std::shared_ptr<void>& actual_item_owner_lease,
 RendererCanonicalWorldV4& candidate,const dh2::world::CanonicalFactoryEntryV1& entry,
 const dh2::world::CanonicalSourceObjectRequestV1& request,
 dh2::world::CanonicalClassReceiverV1& out,std::string& e){
 if(!actual_item_owner_lease||!same.factory_prepared_v88()||!same.items()){
  e="Required genuinely prepared SAME source Item owner/factory before authored Item C1";return false;
 }
 // Existing V57 checks both actual manager and SAME property-map references,
 // exact Item340d38, retained declaration, actual Item/Handle/property receiver.
 dh2::world::CanonicalItemCandidateTransportV57 transport(*same.items(),candidate.manager,candidate.properties,actual_item_owner_lease);
 return transport.construct(entry,request,out,e);
}
