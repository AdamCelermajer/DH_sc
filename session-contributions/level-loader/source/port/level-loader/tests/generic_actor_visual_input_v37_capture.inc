
 auto actual_chest_visual=containers.front()->visual();
 require(actual_chest_visual&&actual_chest_visual->ready(),"actor transport requires actually initialized first chest Visual");
 ActorVisualProducerV37 actor_stale_producer;require(actor_visual_producer_v37(containers.front(),actor_stale_producer,e),e);
 const auto chest_key=containers.front()->receiver().base().shared_handle().key;
 const auto npc_key=npc->actor->shared_handle().key;
 bool actor_foreign_mapping=false;
 auto actor_lookup=[&](const CanonicalObjectBorrowV1& object,ActorVisualProducerV37& out,std::string& e){
  if(object.identity==containers.front()->receiver().base().identity())return actor_visual_producer_v37(actor_foreign_mapping?containers.back():containers.front(),out,e);
  if(object.identity==npc->actor->canonical(npc->actor).identity)return actor_visual_producer_v37(npc->actor,{},out,e);
  e="Explicit host transport has no actual Visual lookup for this receiver";return false;
 };
 GenericActorVisualInputV37 actor_input({bytes,&fixture.manager,roots,actor_lookup});
 ActorVisualConsumeResultV37 actor_result;unsigned actor_reads=0;
 auto actual_actor_read=[&](const ActorVisualReadV37& read,std::string&){
  ++actor_reads;
  require(read.receiver.identity==containers.front()->receiver().base().identity(),"actor read substituted receiver");
  require(&read.scene==&actual_chest_visual->scene()&&&read.skinned_meshes==&actual_chest_visual->skinned_meshes(),"actor read copied native scene/skin state");
  require(read.bres.bytes==actual_chest_visual->bres().bytes&&read.root_identity==actual_chest_visual->root_identity(),"actor read substituted actual BRES/root");
  for(const auto& instance:read.scene.instances){assets::Mesh payload{};require(dh2_mesh_open(&payload,&read.bres,instance.geometry)==assets::Error::ok,"actual actor mesh payload is invalid");}
  return true;
 };
 require(actor_input.consume(chest_key,actual_actor_read,actor_result,e)&&e.empty(),e);
 const auto actor_visible_instance_count=actor_result.instances;
 require(actor_result.disposition==ActorVisualConsumptionV37::consumed_visual&&actor_visible_instance_count>0&&actor_reads==1,"actual chest Visual was not synchronously consumed");
 require(actor_input.consume(npc_key,actual_actor_read,actor_result,e)&&actor_result.disposition==ActorVisualConsumptionV37::no_visual_published&&actor_result.character_receiver&&!actor_result.character_animation_present&&actor_reads==1,"fresh Character NULL Visual/animation was fabricated");
 const auto preserved_actor_result=actor_result;
 actor_foreign_mapping=true;
 require(!actor_input.consume(chest_key,actual_actor_read,actor_result,e)&&actor_result.disposition==preserved_actor_result.disposition&&actor_reads==1,"actor input accepted foreign actual owner or mutated out");
 actor_foreign_mapping=false;
 auto actor_foreign_roots=std::make_shared<GameObjectSceneRootRegistryV1>();
 GenericActorVisualInputV37 actor_foreign_input({bytes,&fixture.manager,actor_foreign_roots,actor_lookup});
 require(!actor_foreign_input.consume(chest_key,actual_actor_read,actor_result,e)&&actor_reads==1,"actor input accepted foreign root registry");
 require(!actor_input.consume(0,actual_actor_read,actor_result,e)&&actor_reads==1,"actor input accepted reserved NULL key");
 require(!actor_input.consume(chest_key,{},actor_result,e),"published actor Visual consumed without endpoint");
 bool actor_nested_rejected=false;
 require(actor_input.consume(chest_key,[&](const ActorVisualReadV37& read,std::string& e){
  ActorVisualConsumeResultV37 nested;
  actor_nested_rejected=!actor_input.consume(chest_key,actual_actor_read,nested,e)&&e=="Actor Visual consume reentered";
  return read.root_identity==actual_chest_visual->root_identity();
 },actor_result,e)&&actor_nested_rejected&&e.empty(),e);
 const auto actor_preserved_instances=actor_result.instances;
 require(!actor_input.consume(chest_key,[](const ActorVisualReadV37&,std::string& e){e="explicit host consumer rejection";return false;},actor_result,e)&&actor_result.instances==actor_preserved_instances,"consumer rejection corrupted prior actor result");
 require(!actor_input.immutable_resource_pin_available()&&!actor_input.gameplay_readiness_claimed(),"actor input fabricated resource lifetime/readiness");
