// Same development World promotion. Include after EquipmentPlatform and globals.
// No source constructor/default/InitPost is replayed on adopted Characters.
namespace {
struct RendererCanonicalWorldV4 {
 WorldScriptContext* world;
 dh2::world::ObjectManagerLanguageRegistryV1 language_registry;
 dh2::world::CanonicalObjectManagerV1 manager;
 dh2::world::CanonicalPropertyMapV1 properties;
 dh2::world::CanonicalExistingActorPublicationV3 publication;
 std::shared_ptr<dh2::world::GameObjectSceneRootRegistryV1> scene_roots_v20;
 explicit RendererCanonicalWorldV4(WorldScriptContext& w):world(&w),
  manager({this,local_player,nullptr,nullptr,nullptr,assign_network,language_added}),
  properties({this,&dh2::world::canonical_vec3_origin_v1(),nullptr,point3_list_diagnostic}),
  publication(manager){}
 static bool point3_list_diagnostic(void*,const char* text,std::string& error){
  if(!text){error="Required source Point3D list diagnostic CString";return false;}
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original property parse diagnostic | %s",text);return true;
 }
 static bool language_added(void* p,std::int32_t key,const dh2::world::CanonicalObjectBorrowV1& actor,std::uintptr_t character,std::string& error){
  auto& self=*static_cast<RendererCanonicalWorldV4*>(p);
  if(!actor.type_f4){error="Required same canonical language type_f4";return false;}
  if(*actor.type_f4==14){error="Required actual type14 localization-valid819 owner";return false;}
  return self.language_registry.source_added({key,actor.identity,actor.lease,actor.type_f4,nullptr,character},error);
 }
 static bool local_player(void* p,std::uintptr_t& out,std::string& e){
  auto& self=*static_cast<RendererCanonicalWorldV4*>(p);
  if(!self.world->player_object||!player_skills_runtime||
     player_skills_runtime->world.get()!=self.world||!player_skills_runtime->player_manager){
   e="Required same World source PlayerManager local player owner";return false;
  }
  dh2::player::PlayerInfoFieldsV1* player{};
  if(!player_skills_runtime->player_manager->manager().get_local_player(0,true,player,e))return false;
  if(!player||player->character660!=self.world->player_object->identity){e="Source local PlayerInfo character660 differs from canonical player";return false;}
  out=player->character660;return true;
 }
 static bool assign_network(void* p,dh2::world::CanonicalObjectBorrowV1&,std::string& e){
  auto& self=*static_cast<RendererCanonicalWorldV4*>(p);
  if(!equipment_platform||equipment_platform->world.get()!=self.world){
   e="Required same World GetOnline for canonical Add";return false;
  }
  // AssignObjectNetworkId3431c0 reads GetOnline.byte5; offline returns
  // before actor/counter/transport mutations. This is the live shared fact.
  if(!equipment_platform->online_mode)return true;
  e="Required source online AssignObjectNetworkId actor/transport owner";return false;
 }
 bool adopt(const std::shared_ptr<MonsterScriptHandle>& npc,std::string& e){
  if(!npc||npc->context.get()!=world){e="Required same World retained NPC adoption";return false;}
  if(publication.attempted(npc->object->identity)){
   const auto* actual=manager.object(npc->shared_handle().key);
   if(!publication.completed_v4(npc->object->identity)||!actual||actual->identity!=npc->object->identity){e="Incomplete previous canonical NPC publication";return false;}
   return true;
  }
  dh2::world::CanonicalObjectBorrowV1 actor;
  auto normal=npc->canonical(npc);
  if(!normal.room64||!npc->canonical_adoption_v2(npc,npc->source_name(),
       npc->source_archetype(),*normal.room64,actor,e))return false;
  dh2::world::CanonicalExistingActorV3 input{actor,&npc->source_name(),&npc->source_archetype()};
  dh2::target_providers::Handle16 out{};
  return publication.publish(input,npc->source_name().c_str(),
      npc->source_archetype().c_str(),*actor.room64,true,out,e);
 }
};
void release_canonical_world_v4(const std::shared_ptr<WorldScriptContext>& world){
 if(!world)return;
 world->camera_config_v20.reset();
 // Drop all canonical receiver leases before releasing the facet's World pin.
 world->canonical_world.reset();
 if(world->player_canonical)world->player_canonical->release_after_unpublication();
 world->player_canonical.reset();
}
void publish_canonical_world_v4(const std::shared_ptr<WorldScriptContext>& world){
 std::string error;
 if(!world||!world->player_canonical||!world->player_object)
  throw std::runtime_error("Required same World player metadata before canonical publication");
 if(!world->canonical_world)world->canonical_world=std::make_shared<RendererCanonicalWorldV4>(*world);
 auto& canonical=*world->canonical_world;auto& player_meta=*world->player_canonical;
 if(!player_meta.bound){
  if(!player_meta.bind(world->player_canonical,world->player_object,prince_runtime,world,nullptr,error)||
     !player_meta.publish(world->player_canonical,canonical.publication,error))throw std::runtime_error(error);
 }else if(!canonical.publication.completed_v4(world->player_object->identity)||!canonical.manager.object(player_meta.handle.key))
  throw std::runtime_error("Incomplete retained canonical player publication");
 for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.kind==1)
  if(!canonical.adopt(actor.script,error))throw std::runtime_error(error);
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Canonical same World publication | actors %u | no constructor/default/VM replay | shared source Handle keys",canonical.manager.source_count50());
}
} // Same anonymous scope as the retained WorldScriptContext.
