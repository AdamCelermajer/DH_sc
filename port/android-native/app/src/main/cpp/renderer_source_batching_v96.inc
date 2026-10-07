//Include inside SourceRuntime anonymous model_renderer namespace AFTER source
//debug helpers and renderer_source_stage34_v80.inc, BEFORE connect candidate.
static dh2::loader::BatchNodeBorrowV96 source_batch_node_loan_v109(dh2::world::RetainedSceneNodeBorrowV109 node){
 dh2::loader::BatchNodeBorrowV96 out;out.owner=node.owner;out.identity=node.identity;out.name24=node.name24;
 out.child_count=node.child_count;out.set_visible48=node.set_visible48;out.source_v111=node;
 out.child=[node=std::move(node)](std::size_t index,dh2::loader::BatchNodeBorrowV96& out,std::string& e){
  dh2::world::RetainedSceneNodeBorrowV109 child;if(!node.child||!node.child(index,child,e))return false;
  out=source_batch_node_loan_v109(std::move(child));return true;
 };
 return out;
}
struct SourceBatchingStagesV96 : std::enable_shared_from_this<SourceBatchingStagesV96> {
 std::weak_ptr<SourceWorldBorrowV61> world;std::weak_ptr<RendererSourceCandidateV55> candidate;
 std::weak_ptr<dh2::loader::CanonicalLevelContextV1> level;
 dh2::loader::EarlyLoadingDebugV46 loading_trace;
 std::array<bool,4> failed{},complete{};std::array<std::string,4> error;bool busy{};std::uint32_t active_phase{};std::weak_ptr<dh2::loader::BatchNodeCompilerSourceV96> active_compiler;
 using Step=dh2::loader::LifecycleStepV36;
 Step reject(std::uint32_t phase,std::string& e,const char* leaf){auto index=phase-21;
  if(!failed[index]){failed[index]=true;error[index]=e.empty()?std::string("Required original batching stage: ")+leaf:e;}e=error[index];return Step::failed;}
 bool current(std::shared_ptr<SourceWorldBorrowV61>& source,std::shared_ptr<RendererSourceCandidateV55>& producer,
  std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& receiver,std::string& e)const{
  source=world.lock();producer=candidate.lock();receiver=level.lock();
  if(!source||!source->owner||!source->application||!source->canonical_world||!producer||!producer->factory||!producer->preparation||!receiver||
   producer->preparation->level()!=receiver||producer->canonical!=source->canonical_world||&producer->canonical->manager!=&producer->preparation->manager()){
   if(e.empty())e="Retired or mismatched SAME batch Level/Scene/manager";return false;}
  SourceCampaignCandidateBorrowV55 actual;
  const auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
  if(!borrow_source_campaign_candidate_runtime_v61(actual,e)||!same(actual.actual_world,source->owner)||
   !same(actual.application,source->application)||!same(actual.level,receiver)||!same(actual.objects,producer->canonical->manager_lease)){
   if(e.empty())e="Batch callback replaced actual published campaign/App/Level/manager";return false;
  }
  if(busy){if(failed[active_phase-21]){e=error[active_phase-21];return false;}
   if(receiver->constructor_fields_v3().field130!=active_phase){if(e.empty())e="Actual batch phase changed during native delivery";return false;}
   if(auto expected=active_compiler.lock()){const auto& slot=receiver->batch_compiler_owner_slot_v96();
    if(!slot||slot.get()!=expected.get()||slot.owner_before(expected)||expected.owner_before(slot)||receiver->constructor_fields_v3().field158!=expected->identity()){if(e.empty())e="Actual compiler158 replaced during native delivery";return false;}}}return true;
 }
 bool debug(const char* key,bool& value,std::string& e)const{
  std::shared_ptr<SourceWorldBorrowV61> source;std::shared_ptr<RendererSourceCandidateV55> producer;std::shared_ptr<dh2::loader::CanonicalLevelContextV1> receiver;
  return source_debug_load_v55(world,e)&&current(source,producer,receiver,e)&&source_debug_switch_v55(world,key,value,e)&&current(source,producer,receiver,e);}
 bool borrow_object(const dh2::world::CanonicalObjectBorrowV1& supplied,dh2::loader::BatchObjectBorrowV96& out,std::string& e)const{
  // Keep the actual receiver receipt/pin through native callbacks even if
  // those callbacks remove or replace its manager map entry. Fields stay live.
  const auto object=supplied;
  std::shared_ptr<SourceWorldBorrowV61> source;std::shared_ptr<RendererSourceCandidateV55> producer;std::shared_ptr<dh2::loader::CanonicalLevelContextV1> receiver;
  if(!current(source,producer,receiver,e)||!object.lease||!object.identity)return false;
  auto transport=producer->transport;if(!transport){e="Required SAME actual source receiver transport";return false;}
  auto classes=transport->services();bool gameobject{};
  if(!classes.is_game_object||!classes.is_game_object(classes.context,object,gameobject,e)||!current(source,producer,receiver,e))return false;
  if(!gameobject){out={};e.clear();return true;} //Genuine selected NULL GameObject conversion; Config remains ObjectBase.
  dh2::loader::BatchObjectBorrowV96 b;b.owner=object.lease;b.identity=object.identity;std::uintptr_t character{};
  if(object.as_character&&!object.as_character(object.context,character,e))return false;
  if(!current(source,producer,receiver,e))return false;
  //An omitted cast still requires the genuine typed nonCharacter base receipt below; never infer a Character or fabricate conversion.
  if(character){SourceCampaignCharacterBorrowV62 same;
   if(character!=object.identity||!borrow_source_campaign_character_v62(source->owner,character,same,e)||!same.character||!same.character->actor)return false;
   dh2::world::GameObjectInitializationFieldsV62 fields;
   if(!same.character->actor->inherited_initialization_fields_v62(same.character,fields,e))return false;
   b.owner=same.character;b.name30=same.character->actor->source_string(0x30);b.archetype48=same.character->actor->source_string(0x48);
   b.deleted83=fields.byte(0x83);b.linked2fc=fields.byte(0x2fc);b.visual2d8=fields.pointer(0x2d8);
  }else{const dh2::world::CanonicalClassReceiverV1* dispatch{};
   if(!transport->receiver(object,dispatch,e)||!current(source,producer,receiver,e)||!dispatch||!dispatch->source_loading_fields_v95){if(e.empty())e="Required actual derived loading-field producer";return false;}
   dh2::world::CanonicalObjectLoadingFieldsV95 fields;
   if(!dispatch->source_loading_fields_v95(fields,e)||!current(source,producer,receiver,e)||!fields.receiver||!fields.gameobject_base||fields.gameobject_base->identity()!=object.identity||
    fields.receiver.owner_before(object.lease)||object.lease.owner_before(fields.receiver)){if(e.empty())e="Required SAME GameObject base, no Config projection";return false;}
   auto* base=fields.gameobject_base;b.owner=fields.receiver;b.name30=base->string(0x30);b.archetype48=fields.archetype48;
   b.deleted83=base->byte(0x83);b.linked2fc=base->byte(0x2fc);b.visual2d8=base->pointer(0x2d8);
  }
  //Loan missing cells honestly; the original stage reads each only at its reached branch. No insertion/default here.
  out=std::move(b);return true;
 }
 bool native_services(dh2::loader::BatchNativeServicesV96& out,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> source;std::shared_ptr<RendererSourceCandidateV55> producer;std::shared_ptr<dh2::loader::CanonicalLevelContextV1> receiver;
  if(!current(source,producer,receiver,e))return false;
  dh2::loader::BatchNativeServicesV96 s;
  if(source->batching_native_v96){SourceCampaignCandidateBorrowV55 actual;
   if(!borrow_source_campaign_candidate_v55(actual,e)||!current(source,producer,receiver,e)||actual.actual_world!=source->owner||actual.level!=receiver||!source->batching_native_v96(actual,s,e)||!current(source,producer,receiver,e))return false;
  }
  //The native compiler C1 is loader-owned even before a positive mesh/GPU leaf
  //is reached. Independent source authority is not a fake renderer owner.
  if(!s.provider)s.provider=source->files_owner;
  if(!s.provider){e="Required independent batch source authority";return false;}
  const auto same=[](const auto& a,const auto& b){return a&&b&&!a.owner_before(b)&&!b.owner_before(a);};
  if(same(s.provider,source)||same(s.provider,source->owner)||same(s.provider,receiver)||same(s.provider,producer->canonical->manager_lease)){
   e="Batch native authority must not retain containing World/Level/manager";return false;}
  const auto self=std::weak_ptr<SourceBatchingStagesV96>(shared_from_this());
  auto prior=s.validate_current;s.validate_current=[self,prior](std::string& e){auto p=self.lock();std::shared_ptr<SourceWorldBorrowV61> source;std::shared_ptr<RendererSourceCandidateV55> producer;std::shared_ptr<dh2::loader::CanonicalLevelContextV1> receiver;
   if(!p||!p->current(source,producer,receiver,e))return false;
   const auto phase=receiver->constructor_fields_v3().field130;
   if(phase<21||phase>24||p->failed[phase-21]){if(e.empty())e=phase>=21&&phase<=24?p->error[phase-21]:"Batch source delivery retired";return false;}
   if(prior&&!prior(e))return false;return p->current(source,producer,receiver,e);};
  if(!s.as_gameobject)s.as_gameobject=[self](const auto& object,auto& out,std::string& e){auto p=self.lock();if(!p){e="Retired SAME batch source object borrower";return false;}return p->borrow_object(object,out,e);};
  if(!s.object)s.object=[self](std::uintptr_t id,auto& out,std::string& e){auto p=self.lock();std::shared_ptr<SourceWorldBorrowV61> source;std::shared_ptr<RendererSourceCandidateV55> producer;std::shared_ptr<dh2::loader::CanonicalLevelContextV1> receiver;
   if(!p||!p->current(source,producer,receiver,e))return false;const dh2::world::CanonicalObjectBorrowV1* object{};std::int32_t key{};
   bool found=producer->canonical->manager.source_ordered_begin_v38(key,object);while(found&&(!object||object->identity!=id))found=producer->canonical->manager.source_ordered_next_v38(key,key,object);
   if(!found||!object){e="Batch object no longer published in SAME manager";return false;}return p->borrow_object(*object,out,e);};
  if(!s.trace)s.trace=[self](const char* key,std::string& e){auto p=self.lock();bool ignored{};if(!p){e="Retired batch Debug owner";return false;}return p->debug(key,ignored,e);};
  if(!s.visual_root)s.visual_root=[self](const dh2::loader::BatchObjectBorrowV96& object,dh2::loader::BatchNodeBorrowV96& out,std::string& e){
   out={};auto p=self.lock();std::shared_ptr<SourceWorldBorrowV61> source;std::shared_ptr<RendererSourceCandidateV55> producer;
   std::shared_ptr<dh2::loader::CanonicalLevelContextV1> receiver;
   if(!p||!p->current(source,producer,receiver,e)||!object.owner||!object.visual2d8){if(e.empty())e="Required current batching Visual2d8 loan";return false;}
   const auto visual_id=*object.visual2d8;if(!visual_id){e.clear();return true;}
   dh2::world::RetainedSceneNodeBorrowV109 node;
   if(producer->factory->modules->module(object.identity)){
    auto visual=producer->graph?producer->graph->visual(visual_id):nullptr;
    if(!visual||visual->identity()!=visual_id){e="Required SAME Module batching visual";return false;}
    if(!visual->root_identity()){e.clear();return true;}
    if(!visual->borrow_scene_node_v109(visual->root_identity(),node,e))return false;
   }else{
    std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;
    if(object.archetype48&&(*object.archetype48=="Character"||*object.archetype48=="Player")){
     SourceCampaignCharacterBorrowV62 actor;
     if(!borrow_source_campaign_character_v62(source->owner,object.identity,actor,e)||!actor.character||!actor.character->visual)return false;
     visual=actor.character->visual->visual();
    }else if(!source->gameobject_graph_v68||!source->gameobject_graph_v68->borrow_visual(visual_id,visual,e))return false;
    if(!visual||reinterpret_cast<std::uintptr_t>(visual.get())!=visual_id){e="Required SAME actual batching VisualObject";return false;}
    if(!visual->root_identity()){e.clear();return true;}
    if(!visual->borrow_scene_node_v109(visual,visual->root_identity(),node,e))return false;
   }
   if(!p->current(source,producer,receiver,e)||*object.visual2d8!=visual_id){if(e.empty())e="Batching visual replaced during hierarchy loan";return false;}
   out=source_batch_node_loan_v109(std::move(node));e.clear();return true;
  };
  if(!s.clear_render_lists)s.clear_render_lists=[self](std::string& e){auto p=self.lock();std::shared_ptr<SourceWorldBorrowV61> source;
   std::shared_ptr<RendererSourceCandidateV55> producer;std::shared_ptr<dh2::loader::CanonicalLevelContextV1> receiver;
   return p&&p->current(source,producer,receiver,e)&&clear_source_scene_drawlist_loading_v109(source->owner,e);
  };
  if(!s.driver_type5c)s.driver_type5c=[](std::uint32_t& out,std::string& e){std::int32_t type{};if(!borrow_actual_device_driver_type_v55(type,e))return false;out=static_cast<std::uint32_t>(type);return true;};
  if(!s.is_faerie)s.is_faerie=[self](std::uintptr_t id,bool& value,std::string& e){auto p=self.lock();std::shared_ptr<SourceWorldBorrowV61> source;std::shared_ptr<RendererSourceCandidateV55> producer;std::shared_ptr<dh2::loader::CanonicalLevelContextV1> receiver;
   SourceCampaignCharacterBorrowV62 same;if(!p||!p->current(source,producer,receiver,e)||!borrow_source_campaign_character_v62(source->owner,id,same,e)||!same.character||!same.character->properties)return false;
   const auto* rows=same.character->design.ai();std::int32_t selected{};
   if(!rows||dh2_character_target_ai_id(&selected,same.character->properties->resolved.data(),static_cast<std::uint32_t>(rows->rows.size()))){e="Required original Character.GetCharAIId";return false;}
   const auto* row=dh2::data::ai_props(*rows,selected);if(!row){e="Original Character.GetCharType positive row unavailable";return false;}value=row->type==3;return true;};
  out=std::move(s);return true;
 }
 Step stage(std::uint32_t phase,std::string& e){
  if(phase<21||phase>24){e="Invalid actual batching phase";return Step::failed;}auto index=phase-21;
  if(failed[index]){e=error[index];return Step::failed;}if(busy){reject(active_phase,e,"non-reentrant batching stage");return reject(phase,e,"non-reentrant batching stage");}busy=true;active_phase=phase;active_compiler.reset();struct Guard{bool& b;~Guard(){b=false;}} guard{busy};
  std::shared_ptr<SourceWorldBorrowV61> source;std::shared_ptr<RendererSourceCandidateV55> producer;std::shared_ptr<dh2::loader::CanonicalLevelContextV1> receiver;
  if(!current(source,producer,receiver,e)||receiver->constructor_fields_v3().field130!=phase)return reject(phase,e,"SAME original state21..24");
  if(complete[index]){e.clear();return Step::complete;}
  if(!dh2::loader::early_loading_trace_v46(loading_trace,e)||!current(source,producer,receiver,e))return reject(phase,e,"original _LoadProcess batching trace prefix");
  auto roots=source->canonical_world->scene_roots_v20;
  if(phase==21){auto bag=source->stage34_native_v80;
   if(!roots||!bag||!bag->scene_virtual60||!bag->scene_virtual60(roots,0.0f,false,e)||!current(source,producer,receiver,e))return reject(phase,e,"actual Scene.update(0,false) BEFORE batch Debug gate");}
  bool disabled{};if(!debug("IsDisablingCompiledBatching",disabled,e)||!current(source,producer,receiver,e))return reject(phase,e,"fresh compiled-batching Debug gate");
  if(disabled){complete[index]=true;e.clear();return Step::complete;}
  auto fields=receiver->constructor_borrow_v3();auto& owner=receiver->batch_compiler_owner_slot_v96();
  if(phase==21){
   if(!dh2::loader::release_level_batch_compiler_v96(receiver,e)||!current(source,producer,receiver,e))return reject(phase,e,"old SAME original compiler158 D1");
   dh2::loader::BatchNativeServicesV96 native;if(!native_services(native,e)||!current(source,producer,receiver,e))return reject(phase,e,"actual batch resource primitive assembly");
   if(owner||fields.fields->field158)return reject(phase,e,"Level158 changed before original compiler publication");
   owner=std::make_shared<dh2::loader::BatchNodeCompilerSourceV96>(std::move(native));fields.fields->field158=owner->identity();
  }else{
   //Only original _LoadBatchMap has an explicit NULL158 early return.
   if(phase==23&&!fields.fields->field158&&!owner){complete[index]=true;e.clear();return Step::complete;}
   if(!owner||fields.fields->field158!=owner->identity())return reject(phase,e,"SAME actual BatchNodeCompiler158");
   auto compiler=owner;active_compiler=compiler; //Keep the delivered receiver alive through callbacks.
   const auto& native=compiler->native_services();
   if(phase==22){std::int32_t key{};const dh2::world::CanonicalObjectBorrowV1* object{};
    for(bool more=producer->canonical->manager.source_ordered_begin_v38(key,object);more;more=producer->canonical->manager.source_ordered_next_v38(key,key,object)){
     if(!object||!object->identity)continue; //Native NULL-map node => NULL conversion.
     dh2::target_providers::Handle16 handle;const dh2::world::CanonicalObjectBorrowV1* live{};
     if(!producer->canonical->manager.get_handle(key,handle,e)||!current(source,producer,receiver,e)||!producer->canonical->manager.resolve_handle_v4(handle,false,live,{},e)||!current(source,producer,receiver,e))return reject(phase,e,"actual ObjectHandle C1/GetHandle/conversion");
     if(!live)continue;dh2::loader::BatchObjectBorrowV96 actual;
     if(!native.as_gameobject||!native.as_gameobject(*live,actual,e)||!current(source,producer,receiver,e))return reject(phase,e,"actual selected ObjectHandle->GameObject");
     bool eligible{};if(!dh2::loader::original_batch_eligible_v96(actual,native,eligible,e)||!current(source,producer,receiver,e))return reject(phase,e,"original batch family/name/Faerie filters");
     if(eligible&&!compiler->append_object(actual,e))return reject(phase,e,"original compiler vector10 append");
     if(failed[index]||!current(source,producer,receiver,e))return reject(phase,e,"same ordered batch-list continuation");
    }
   }else if(phase==23){if(!compiler->build_map(e))return reject(phase,e,"original recursive node/GameObject map");}
   else{
    if(!roots||!native.clear_render_lists||!native.clear_render_lists(e)||!current(source,producer,receiver,e))return reject(phase,e,"actual Scene.clearRenderLists");
    roots->source_frame_fields_v67().batching43c_v96=1;roots->source_frame_fields_v67().native_batching43c_produced_v96=true;
    bool high{};if(!borrow_actual_device_high_performance_v54(high,e)||!current(source,producer,receiver,e))return reject(phase,e,"actual Device.IsHighPerformance");
    auto config_fields=receiver->config_fields();const auto* config=config_fields.config38?producer->factory->modules->level_config(*config_fields.config38):nullptr;
    const auto* vertices=config?config->integer(high?0x2a0:0x2ac):nullptr;const auto* indices=config?config->integer(high?0x2a4:0x2b0):nullptr;
    const auto* quantize=config?config->byte(high?0x2a8:0x2b4):nullptr;
    if(!vertices||!indices||!quantize)return reject(phase,e,"actual selected Config batch limits/quantize");
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","*** BUFFER SIZE: %d %d %d",*vertices,*indices,int(*quantize));
    dh2::loader::BatchDriverFieldsV96 driver;
    if(!native.driver_fields||!native.driver_fields(driver,e)||!current(source,producer,receiver,e)||!driver.owner||!driver.vertices22c||!driver.indices230)return reject(phase,e,"actual SAME native driver22c/230 fields");
    std::int32_t value{};if(!dh2::loader::original_batch_limit_v96(*indices,native,value,e)||!current(source,producer,receiver,e))return reject(phase,e,"actual index limit double1.02 conversion");*driver.indices230=value;
    if(!dh2::loader::original_batch_limit_v96(*vertices,native,value,e)||!current(source,producer,receiver,e))return reject(phase,e,"actual vertex limit double1.02 conversion");*driver.vertices22c=value;
    if(!compiler->compile(*quantize!=0,e)||!current(source,producer,receiver,e))return reject(phase,e,"original compiler.Compile");
    roots->source_frame_fields_v67().batching43c_v96=0;
    if(!compiler->finish_scene_attachment(e))return reject(phase,e,"actual compiled-root attach/visible suffix");
   }
  }
  if(!current(source,producer,receiver,e))return reject(phase,e,"same batching stage completion");
  complete[index]=true;e.clear();return Step::complete;
 }
};
static bool bind_source_batching_stages_v96(const std::shared_ptr<SourceWorldBorrowV61>& world,
 const std::shared_ptr<RendererSourceCandidateV55>& candidate,const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& level,
 dh2::loader::EarlyLoadingDebugV46 trace,dh2::loader::SourceLoadingInputsV43& loading,std::string& e){
 if(!world||!candidate||!candidate->preparation||candidate->preparation->level()!=level||!level){e="Required SAME actual batch source candidate";return false;}
 auto body=std::make_shared<SourceBatchingStagesV96>();body->world=world;body->candidate=candidate;body->level=level;body->loading_trace=std::move(trace);
 for(std::uint32_t phase=21;phase<=24;++phase)if(!loading.external.stage_body[phase])loading.external.stage_body[phase]=[body,phase](std::string& e){
  try{return body->stage(phase,e);}catch(const std::exception& exception){e=exception.what();return body->reject(phase,e,"native batch exception prefix");}
  catch(...){e="Actual native batching provider threw";return body->reject(phase,e,"native batch exception prefix");}
 };
 e.clear();return true;
}


