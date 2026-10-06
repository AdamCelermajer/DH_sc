
 GenericActorVisualInputV37 actor_stale_input({bytes,&fixture.manager,roots,[&](const CanonicalObjectBorrowV1&,ActorVisualProducerV37& out,std::string&){out=actor_stale_producer;return true;}});
 require(!actor_stale_input.consume(chest_key,actual_actor_read,actor_result,e),"actor input accepted stale released Visual producer");
 require(actual_chest_visual->bres().bytes==nullptr&&actual_chest_visual->scene().instances.empty(),"actual Visual release retained borrowed payload");
 const auto prior_actor_reads=actor_reads;
 require(actor_input.consume(chest_key,actual_actor_read,actor_result,e)&&actor_result.disposition==ActorVisualConsumptionV37::no_visual_published&&actor_reads==prior_actor_reads,"fresh post-release lookup fabricated a Visual/read");
 std::cerr<<"GENERIC_ACTOR_VISUAL_V37 PASS actual_chest_instances="<<actor_visible_instance_count<<" actual_skin_ref=1 character_NULL_reported=1 foreign_owner_rejected=1 foreign_roots_rejected=1 stale_release_rejected=1 reentrancy_rejected=1 consumer_failure_preserves_out=1 immutable_pin=0 gameplay_ready=0\n";
