// Include inside SourceRuntime's existing anonymous model_renderer namespace
// after Source candidate/factory definition and before RendererSourceCampaign.
struct SourceCharacterWarmStartV95 {
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::weak_ptr<RendererSourceCandidateV55> candidate;
 std::weak_ptr<dh2::loader::CanonicalLevelContextV1> level;
 dh2::loader::EarlyLoadingDebugV46 trace;
 std::size_t character_cursor{};bool traced{},complete18{},complete32{},failed18{},failed32{},busy{};
 std::string error18,error32;
 dh2::loader::LifecycleStepV36 reject(std::uint32_t stage,std::string& e,const char* leaf){
  auto& failed=stage==18?failed18:failed32;auto& saved=stage==18?error18:error32;
  if(!failed){failed=true;saved=e.empty()?std::string("Required original source loading body: ")+leaf:e;}
  e=saved;return dh2::loader::LifecycleStepV36::failed;
 }
 bool current(std::uint32_t stage,std::shared_ptr<SourceWorldBorrowV61>& out,
  std::shared_ptr<RendererSourceCandidateV55>& factory,std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& receiver,std::string& e)const{
  out=world.lock();factory=candidate.lock();receiver=level.lock();
  if(!out||!out->owner||!out->application||!out->canonical_world||!factory||!factory->factory||!factory->preparation||!receiver||
   factory->preparation->level()!=receiver||factory->canonical!=out->canonical_world||
   &factory->preparation->manager()!=&factory->canonical->manager||receiver->constructor_fields_v3().field130!=stage){
   if(e.empty())e="Retired or mismatched SAME Character/warm-start Level/World/manager";return false;
  }return true;
 }
 dh2::loader::LifecycleStepV36 load_character_states(std::string& e){
  using dh2::loader::LifecycleStepV36;
  if(failed18){e=error18;return LifecycleStepV36::failed;}
  if(busy)return reject(18,e,"non-reentrant LoadCharStates");
  busy=true;struct Guard{bool& busy;~Guard(){busy=false;}} guard{busy};
  std::shared_ptr<SourceWorldBorrowV61> source;std::shared_ptr<RendererSourceCandidateV55> factory;std::shared_ptr<dh2::loader::CanonicalLevelContextV1> receiver;
  if(!current(18,source,factory,receiver,e))return reject(18,e,"actual source state18");
  if(complete18){e.clear();return LifecycleStepV36::complete;}
  if(!traced){if(!dh2::loader::early_loading_trace_v46(trace,e)||failed18)return reject(18,e,"fresh original state18 Debug prefix");traced=true;}
  //Canonical characters() is the SAME manager Add/AsChar list60 projection,
  //distinct from source tracked list70. No name/type/current-state filter.
  //Re-read its live size each turn so native tail appends are visited in order.
  while(character_cursor<factory->canonical->manager.characters().size()){
   const auto id=factory->canonical->manager.characters()[character_cursor];
   if(id){SourceCampaignCharacterBorrowV62 actual;
    if(!borrow_source_campaign_character_v62(source->owner,id,actual,e)||!actual.character||!actual.character->actor)
     return reject(18,e,"SAME published Character list60 receiver");
    auto actor=actual.character->actor;const auto* preset=actor->source_string(0x13cc);
    if(!actor->object||actor->object->identity!=id||!actor->machine||actor->machine->native_fsm().character!=id||!preset)
     return reject(18,e,"actual Character13cc and SAME embedded FSM4fc");
    //GetPreSetAIState3a5784 compares actual std::string contents, not strcmp
    //against a copied property label. Even an embeddedNUL has native length.
    const std::int32_t selected=*preset=="Limbus"?0:*preset=="PreSpawn"?17:3;
    if(actor->machine->initialize_level_preset_v95(selected)<0){e=actor->machine->error();return reject(18,e,"actual SM_SetPreSpawnState / SM_SetIdleState(false)");}
    if(failed18||!current(18,source,factory,receiver,e))return reject(18,e,"same actor startup continuation");
    //Original traverses next only AFTER native state callbacks. Tail append is
    //supported; deletion of its current native list node is outside that safe
    //domain and must not silently skip the shifted next vector projection.
    if(character_cursor>=factory->canonical->manager.characters().size()||factory->canonical->manager.characters()[character_cursor]!=id)
     return reject(18,e,"live Character list60 node retained across state body");
   }
   ++character_cursor;
  }
  complete18=true;e.clear();return LifecycleStepV36::complete;
 }
 dh2::loader::LifecycleStepV36 warm_start(std::string& e){
  using dh2::loader::LifecycleStepV36;
  if(failed32){e=error32;return LifecycleStepV36::failed;}
  if(busy)return reject(32,e,"non-reentrant native warm-start");
  busy=true;struct Guard{bool& busy;~Guard(){busy=false;}} guard{busy};
  std::shared_ptr<SourceWorldBorrowV61> source;std::shared_ptr<RendererSourceCandidateV55> factory;std::shared_ptr<dh2::loader::CanonicalLevelContextV1> receiver;
  if(!current(32,source,factory,receiver,e))return reject(32,e,"actual source state32");
  if(complete32){e.clear();return LifecycleStepV36::complete;}
  //Original caches actual App10.Scene1c BEFORE three distinct GetLevelConfig
  //calls. Re-read config38 separately for R/G/B; alpha is literal1.0f.
  auto roots=source->canonical_world->scene_roots_v20;
  if(!roots)return reject(32,e,"SAME actual SceneManager1c");
  std::array<float,4> color;for(unsigned index=0;index<3;++index){
   auto fields=receiver->config_fields();const auto* config=fields.config38?factory->factory->modules->level_config(*fields.config38):nullptr;
   const auto* rgb=config?config->vector(0x1cc):nullptr;
   if(!rgb)return reject(32,e,"actual GetLevelConfig ambient1cc/1d0/1d4");
   color[index]=(*rgb)[index];
  }
  color[3]=1.0f;roots->source_set_ambient_v67(color);
  //3f7778 caches the SAME PFWorld receiver, then3f7780 writes its byte94.
  //This is not Application+94: all current navigation consumers borrow this
  //receiver's float90/byte94 projection without a frame-scratch copy.
  auto floors=factory->rooms?factory->rooms->world():nullptr;
  if(!floors||source->source_pf_floors_v115!=floors)
   return reject(32,e,"SAME actual PFWorld90/94 motion policy receiver");
  floors->source_motion_policy_v95.ignore_height_delta=1;
  //No RAII reset: native failure after this store retains94==1. The original
  //zero store happens only AFTER object/AI/physical bodies authentically finish.
  auto services=source->gameplay_services_v66;
  if(!services||!services->provider||services->actual_application!=source->application||
   services->actual_object_manager!=factory->canonical->manager_lease||!services->update_objects)
   return reject(32,e,"actual pre-state32 ObjectManager.Update provider");
  if(!services->update_objects(1.0f,e)||failed32||!current(32,source,factory,receiver,e))return reject(32,e,"SAME ObjectManager.Update(float1)");
  if(!source_campaign_ai_inc_queue_v105(source->owner,e)||failed32||!current(32,source,factory,receiver,e))return reject(32,e,"actual CharAI.IncUpdateQueue");
  if(!services->physical_update||!services->physical_update(e)||failed32||!current(32,source,factory,receiver,e))return reject(32,e,"actual PhysicalWorld.update with produced App8c");
  floors->source_motion_policy_v95.ignore_height_delta=0;
  //Only Camera128.virtual10 is in this native prefix. No zoom/overview,
  //absolute-node/updateFog/FX/Script/Save/sound/frame-envelope replay here.
  if(!receiver->constructor_fields_v3().field128||!services->camera_update)
   return reject(32,e,"actual positive Camera128 selected virtual10");
  if(!services->camera_update(e)||failed32||!current(32,source,factory,receiver,e))return reject(32,e,"SAME native Camera128 update");
  complete32=true;e.clear();return LifecycleStepV36::complete;
 }
};
static bool bind_source_stage18_stage32_v95(const std::shared_ptr<SourceWorldBorrowV61>& world,
 const std::shared_ptr<RendererSourceCandidateV55>& candidate,const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& level,
 dh2::loader::EarlyLoadingDebugV46 trace,dh2::loader::SourceLoadingInputsV43& loading,std::string& e){
 if(!world||!candidate||!candidate->preparation||candidate->preparation->level()!=level||!level){e="Required SAME actual Character/warm-start candidate";return false;}
 auto body=std::make_shared<SourceCharacterWarmStartV95>();body->world=world;body->candidate=candidate;body->level=level;body->trace=std::move(trace);
 if(!loading.external.stage_body[18])loading.external.stage_body[18]=[body](std::string& e){
  try{return body->load_character_states(e);}catch(const std::exception& exception){e=exception.what();return body->reject(18,e,"native Character state exception prefix");}
  catch(...){e="Actual state18 provider threw";return body->reject(18,e,"native Character state exception prefix");}
 };
 if(!loading.external.stage_body[32])loading.external.stage_body[32]=[body](std::string& e){
  try{return body->warm_start(e);}catch(const std::exception& exception){e=exception.what();return body->reject(32,e,"native warm-start exception prefix");}
  catch(...){e="Actual state32 provider threw";return body->reject(32,e,"native warm-start exception prefix");}
 };
 e.clear();return true;
}
