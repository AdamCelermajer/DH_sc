// Include in model_renderer namespace after V69, the native lenders and
// environmental producer. One actual World owns this authority and its journal
// as siblings. Neither authority nor class callbacks retain the containing World.
struct RendererCampaignProducerV89 final:std::enable_shared_from_this<RendererCampaignProducerV89> {
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::weak_ptr<dh2::loader::SourceReleaseJournalV69> journal;
 std::weak_ptr<dh2::loader::CanonicalLevelContextV1> attempted_level;
 bool attempted{};std::string failure;
 bool prepare(const SourceCampaignCandidateBorrowV55& actual,
  RendererCampaignNonCharacterInputsV69& out,std::string& e){
  auto source=world.lock();
  const auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
  if(!source||!same(source->owner,actual.actual_world)||!same(source->application,actual.application)||
    !source->canonical_world||!same(source->canonical_world->manager_lease,actual.objects)||
    &source->canonical_world->properties!=actual.properties||!actual.level){
   e="Required SAME current World/App/Level/manager/property producer delivery";return false;
  }
  if(attempted){
   auto previous=attempted_level.lock();
   if(previous&&same(previous,actual.level)){
    e=failure.empty()?"Actual non-Character input composition is once per source Level":failure;return false;
   }
   auto previous_journal=journal.lock();
   if(previous_journal&&previous_journal->retained_prefix_count()){
    e="Previous source Level still owns admitted C1/native resource prefixes; require actual unload";return false;
   }
  }
  attempted=true;attempted_level=actual.level;failure.clear();
  RendererCampaignNonCharacterInputsV69 inputs;
  inputs.source.auxiliary.owner=shared_from_this();
  if(!prepare_campaign_environment_v89(actual,inputs,e)){failure=e;return false;}
  SourceCampaignItemLeavesV88 items;items.owner=source->files_owner;items.light_names=inputs.light_names;
  if(!prepare_source_campaign_items_v88(actual,std::move(items),e)){failure=e;return false;}
  // A genuine light environment packet must exist before its admission hooks
  // are installed; the environmental/class producers provide its native leaves.
  dh2::loader::ScopeV67 scope{actual.actual_world,actual.level,actual.objects,actual.properties,shared_from_this()};
  dh2::loader::SourceReleaseJournalLeavesV69 leaves;leaves.owner=shared_from_this();
  // Main publishes the genuine owning-thread/resource primitive provider.
  // Missing provider remains a reached native cleanup dependency; never create
  // a synthetic barrier, free callback or complete destruction certificate.
  {dh2::loader::NonCharacterNativeReleasePrimitivesV92 primitives;
   const bool produced=source->noncharacter_release_primitives_v92?
    source->noncharacter_release_primitives_v92(actual,primitives,e):build_campaign_release_primitives_v107(actual,primitives,e);
   if(!produced||!prepare_campaign_native_release_v92(actual,std::move(primitives),source->noncharacter_release_provider_v92,leaves,e)){failure=e;return false;}
  }
  std::shared_ptr<dh2::loader::SourceReleaseJournalV69> retained;
  if(!dh2::loader::SourceReleaseJournalV69::create(scope,actual.receiver_transport_v69,std::move(leaves),retained,e)){
   failure=e;return false;
  }
  source->noncharacter_release_journal_v89=retained;journal=retained;
  if(source->noncharacter_release_provider_v92&&!source->noncharacter_release_provider_v92->bind_journal(retained,e)){
   failure=e;return false;
  }
  if(!dh2::loader::bind_source_release_journal_v69(inputs.source,retained,e)){failure=e;return false;}
  if(!prepare_campaign_native_lenders_v89(actual,inputs,e)){failure=e;return false;}
  std::weak_ptr<dh2::loader::SourceReleaseJournalV69> weak=retained;
  inputs.after_catalog_composition_v89=[weak](const auto& owners,std::string& e){
   auto same=weak.lock();if(!same){e="Actual source release journal retired before XML";return false;}
   return same->attach_catalog_owners(owners,e);
  };
  out=std::move(inputs);e.clear();return true;
 }
};
bool install_campaign_noncharacter_producer_v89(const std::shared_ptr<SourceWorldBorrowV61>& world,std::string& e){
 if(!world||!world->owner||!world->application||!world->canonical_world||!world->read||
    world->noncharacter_inputs_v81||world->noncharacter_producer_v89||world->noncharacter_release_journal_v89){
  e="Required once-only producer enrollment on actual completed source World";return false;
 }
 auto producer=std::make_shared<RendererCampaignProducerV89>();producer->world=world;
 world->noncharacter_producer_v89=producer;
 std::weak_ptr<RendererCampaignProducerV89> weak=producer;
 world->noncharacter_inputs_v81=[weak](const auto& actual,auto& out,std::string& e){
  auto producer=weak.lock();if(!producer){e="Actual source input authority retired";return false;}
  return producer->prepare(actual,out,e);
 };
 e.clear();return true;
}
