// Same development World promotion. Include after EquipmentPlatform and globals.
// No source constructor/default/InitPost is replayed on adopted Characters.
namespace {
struct RendererCanonicalWorldV4 {
 WorldScriptContext* world;
 dh2::world::ObjectManagerLanguageRegistryV1 language_registry;
 std::shared_ptr<dh2::world::SourceProcessObjectsV121> process_objects_v121;
 std::shared_ptr<dh2::world::CanonicalObjectManagerV1> manager_owner;
 dh2::world::CanonicalObjectManagerV1& manager;
 std::shared_ptr<void> services_alive_v121{std::make_shared<unsigned char>(0)};
 dh2::world::CanonicalPropertyMapV1 properties;
 dh2::world::CanonicalExistingActorPublicationV3 publication;
 std::shared_ptr<dh2::world::GameObjectSceneRootRegistryV1> scene_roots_v20;
 static std::shared_ptr<dh2::world::SourceProcessObjectsV121> actual_process_objects_v121(WorldScriptContext& w){
  auto app=w.source_application_v55.lock();if(!app)return {};
  std::shared_ptr<dh2::world::SourceProcessObjectsV121> out;std::string e;
  if(!dh2::world::SourceProcessObjectsV121::acquire(app,out,e))throw std::runtime_error(e);
  return out;
 }
 explicit RendererCanonicalWorldV4(WorldScriptContext& w):world(&w),
  process_objects_v121(actual_process_objects_v121(w)),
  manager_owner(process_objects_v121?process_objects_v121->manager():
   std::make_shared<dh2::world::CanonicalObjectManagerV1>(dh2::world::CanonicalObjectManagerServicesV1{this,local_player,nullptr,missing_name_debug,nullptr,assign_network,language_added})),
  manager(*manager_owner),
  properties({this,&dh2::world::canonical_vec3_origin_v1(),load_template_assertion,point3_list_diagnostic,this,constant_integer}),
  publication(manager){
  if(process_objects_v121){
   //Independent weak liveness loan: the process singleton never owns this
   //World or relies on a raw callback context after its real facade retires.
   auto pin=std::make_shared<std::weak_ptr<void>>(services_alive_v121);
   const std::weak_ptr<void> alive=services_alive_v121;std::string e;
   if(!process_objects_v121->bind_services({this,local_player,nullptr,missing_name_debug,nullptr,assign_network,language_added},pin,
    [alive](std::string& e){if(alive.expired()){e="Retired actual canonical ObjectManager service receiver";return false;}e.clear();return true;},e))
    throw std::runtime_error(e);
  }
 }
 static bool constant_integer(void* context,const char* group,const char* key,std::int32_t& value,std::string& error){
  auto& self=*static_cast<RendererCanonicalWorldV4*>(context);
  if(!self.world||!self.world->design||!group||!key){error="Required actual property PyDataConstants owner";return false;}
  const auto tables=self.world->design->borrow();const auto* constants=tables.design();
  if(!constants||!constants->lookup||constants->lookup(constants->context,0,group,key,&value)!=0){
   error=std::string("Required original property constant: ")+group+"."+key;return false;
  }
  error.clear();return true;
 }
 static bool load_template_assertion(void*,std::string& error){
  auto assertion=dh2::world::SourceAssertionProcessV76::borrow();
  if(!assertion||!assertion->source_level()){
   error="Required actual process PropertyMap assertion mode";return false;
  }
  const auto mode=*assertion->source_level();
  if(mode==2){
   // Original LoadTemplate stores through NULL in assertion mode 2. Preserve
   // that failure boundary without reproducing the process crash in native code.
   error="Original PropertyMap::LoadTemplate assertion(9) deliberate NULL-store";return false;
  }
  if(mode==1)return assertion->report("..\\..\\project_vs2005\\Game/..\\..\\sources\\Data\\PropertyMap\\PropertyMapTemplates.cpp",9,"false",error);
  error.clear();return true;
 }
 static bool point3_list_diagnostic(void*,const char* text,std::string& error){
  if(!text){error="Required source Point3D list diagnostic CString";return false;}
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original property parse diagnostic | %s",text);return true;
 }
 static bool language_added(void* p,std::int32_t key,const dh2::world::CanonicalObjectBorrowV1& actor,std::uintptr_t character,std::string& error){
  auto& self=*static_cast<RendererCanonicalWorldV4*>(p);
  if(!actor.type_f4){error="Required same canonical language type_f4";return false;}
  // ObjectManager's localization projection borrows the receiver's real +0x819
  // validity byte for type 14. Carry the canonical field through unchanged;
  // the registry only dereferences it when the source type is 14.
  return self.language_registry.source_added({key,actor.identity,actor.lease,actor.type_f4,
      actor.type14_localization_valid819,character},error);
 }
 static bool local_player(void* p,std::uintptr_t& out,std::string& e){
  auto& self=*static_cast<RendererCanonicalWorldV4*>(p);
  const std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> empty_source_app;
  const auto& source_app=self.world->source_application_v55;
  if(source_app.owner_before(empty_source_app)||empty_source_app.owner_before(source_app)){
   const auto app=source_app.lock();if(!app){e="Released actual source Application during local player lookup";return false;}
   const auto& pm=app->source_player_manager_v59();dh2::player::PlayerInfoFieldsV1* actual{};
   if(!pm||!pm->get_local_player(0,true,actual,e)||!actual){
    if(e.empty())e="Required actual startup Application PlayerInfo local alias";return false;
   }
   out=actual->character660;e.clear();return true;
  }
  if(!self.world->player_object||!player_skills_runtime||
     player_skills_runtime->world.get()!=self.world||!player_skills_runtime->player_manager){
   e="Required same World source PlayerManager local player owner";return false;
  }
  dh2::player::PlayerInfoFieldsV1* player{};
  if(!player_skills_runtime->player_manager->manager().get_local_player(0,true,player,e))return false;
  if(!player||player->character660!=self.world->player_object->identity){e="Source local PlayerInfo character660 differs from canonical player";return false;}
  out=player->character660;return true;
 }
 static bool missing_name_debug(void* p,std::string& e){
  auto& self=*static_cast<RendererCanonicalWorldV4*>(p);
  if(!self.world||!self.world->debug){e="Required SAME source ObjectManager Debug owner";return false;}
  std::uint32_t tracing{};
  if(dh2_character_debug_load(self.world->debug.get(),&self.world->debug_files)!=1||
     dh2_character_debug_get(&tracing,self.world->debug.get(),"isTracingObjectManager",&self.world->debug_files)!=1){
   e="Required original ObjectManager Debug load/query";return false;
  }
  // GetObjectByName's not-found path reaches this real policy query before
  // its optional trace formatting. This service has no name/stream receiver,
  // so it reports the source query result without fabricating a host log.
  (void)tracing;e.clear();return true;
 }
 static bool assign_network(void* p,dh2::world::CanonicalObjectBorrowV1&,std::string& e){
  auto& self=*static_cast<RendererCanonicalWorldV4*>(p);
  const std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> empty_source_app;
  const auto& source_app=self.world->source_application_v55;
  if(source_app.owner_before(empty_source_app)||empty_source_app.owner_before(source_app)){
   const auto app=source_app.lock();if(!app){e="Released actual source Application during network assignment";return false;}
   if(!app->get_online_loading_v55()->byte5()){e.clear();return true;}
   e="Required original online AssignObjectNetworkId source transport";return false;
  }
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
