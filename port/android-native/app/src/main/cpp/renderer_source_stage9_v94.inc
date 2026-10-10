// Include inside source_campaign_runtime_v61.cpp's anonymous model_renderer
// namespace AFTER bind_source_stage8_v55 (same real candidate/factory types).
struct SourceLightsetStage9V94 {
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::weak_ptr<RendererSourceCandidateV55> candidate;
 std::weak_ptr<dh2::loader::CanonicalLevelContextV1> level;
 std::weak_ptr<dh2::resources::ContextResourceBudgetV37> device;
 std::uint64_t device_generation{};
 dh2::loader::Stage9TracePrefixV53 trace;
 std::unique_ptr<dh2::loader::Stage9BodyV53> body;
 bool failed{},busy{};std::string failure;
 dh2::loader::LifecycleStepV36 reject(std::string& e,const char* leaf){
  if(!failed){failed=true;failure=e.empty()?std::string("Required actual Stage9 source: ")+leaf:e;}
  e=failure;return dh2::loader::LifecycleStepV36::failed;
 }
 bool current(std::shared_ptr<SourceWorldBorrowV61>& actual,
  std::shared_ptr<RendererSourceCandidateV55>& producer,std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& context,std::string& e)const{
  actual=world.lock();producer=candidate.lock();context=level.lock();
  if(!actual||!actual->owner||!actual->canonical_world||!producer||!producer->canonical||!producer->factory||
   !producer->preparation||!context||producer->preparation->level()!=context||producer->canonical!=actual->canonical_world||
   &producer->canonical->manager!=&producer->preparation->manager()||context->constructor_fields_v3().field130!=9){
   if(e.empty())e="Retired or mismatched SAME Stage9 Level/canonical factory/manager";return false;
  }
  if(device_generation){auto expected=device.lock();auto current=dh2::android_resources::budget_lease_v39();
   if(!expected||expected!=current){if(e.empty())e="Stage9 actual Device owner changed";return false;}
   const auto state=expected->context_state_v41();
   if(!state.ready||state.generation!=device_generation){if(e.empty())e="Stage9 actual Device generation retired";return false;}
  }
  return true;
 }
 dh2::loader::LifecycleStepV36 step(std::string& e){
  using namespace dh2::loader;
  if(failed){e=failure;return LifecycleStepV36::failed;}
  if(busy)return reject(e,"non-reentrant native LightSet delivery");
  busy=true;struct Guard{bool& flag;~Guard(){flag=false;}} guard{busy};
  if(!trace.step(e)||failed)return reject(e,"original Stage9 Debug.GetInstance/GetSwitch prefix");
  std::shared_ptr<SourceWorldBorrowV61> actual;std::shared_ptr<RendererSourceCandidateV55> producer;std::shared_ptr<CanonicalLevelContextV1> context;
  if(!current(actual,producer,context,e))return reject(e,"SAME current Stage9 producer");
  if(!body){
   const auto f=context->config_fields();
   if(!f.config38||!*f.config38)return reject(e,"original positive Level.GetLevelConfig38 assertion edge");
   const auto* config=producer->factory->modules->level_config(*f.config38);
   if(!config)return reject(e,"actual factory-owned LevelConfig38");
   //Typed alias of SAME registered receiver, never another Config/C1.
   const dh2::world::CanonicalObjectBorrowV1* record{};std::int32_t key{};
   bool found=producer->canonical->manager.source_ordered_begin_v38(key,record);
   while(found&&(!record||record->identity!=*f.config38))found=producer->canonical->manager.source_ordered_next_v38(key,key,record);
   if(!found||!record||!record->lease)return reject(e,"published SAME LevelConfig manager record");
   auto owner=std::shared_ptr<dh2::world::CanonicalLevelConfigV1>(record->lease,const_cast<dh2::world::CanonicalLevelConfigV1*>(config));
   Stage9ConfigBorrowV53 fields;
   if(!borrow_stage9_config_v53(context,owner,fields,e))return reject(e,"SAME original Config2b8/2d0 CString fields");
   auto files=producer->preparation->module_files();
   if(!files||!files->native_source_bound())return reject(e,"existing production native copy/parser/journal relay");
   auto actual_device=dh2::android_resources::budget_lease_v39();
   if(!actual_device)return reject(e,"genuine retained actual Device owner");
   const auto device_state=actual_device->context_state_v41();
   if(!device_state.ready||!device_state.generation)return reject(e,"current actual Device context generation");
   device=actual_device;device_generation=device_state.generation;
   const auto weak_world=world;const auto weak_candidate=candidate;const auto weak_level=level;
   const auto weak_device=std::weak_ptr<dh2::resources::ContextResourceBudgetV37>(actual_device);const auto generation=device_generation;
   auto validate=[weak_world,weak_candidate,weak_level,weak_device,generation](std::string& e){
    auto actual=weak_world.lock();auto candidate=weak_candidate.lock();auto context=weak_level.lock();auto device=weak_device.lock();
    if(!actual||!candidate||!context||!candidate->factory||!candidate->preparation||candidate->preparation->level()!=context||
     candidate->canonical!=actual->canonical_world||context->constructor_fields_v3().field130!=9||!device||device!=dh2::android_resources::budget_lease_v39()){
     if(e.empty())e="Stage9 SAME World/Level/factory/Device owner retired";return false;
    }
    const auto state=device->context_state_v41();if(!state.ready||state.generation!=generation){if(e.empty())e="Stage9 Device generation retired";return false;}
    return true;
   };
   Stage9ServicesV53 services;services.actual_device_owner=actual_device;services.actual_file_loader_owner=files;services.validate_current=validate;
   services.is_fixed_pipeline=[validate](bool& value,std::string& e){
    if(!validate(e))return false;std::int32_t type{};
    //Source Device381668: actual driver virtual5c, then result&7. Existing
    //provider validates current EGL context/thread/generation and its selected
    //COpenGLES2Driver native5c result8; no guessed fixed-pipeline policy here.
    if(!borrow_actual_device_driver_type_v55(type,e)||!validate(e))return false;
    value=(static_cast<std::uint32_t>(type)&7u)!=0;return true;
   };
   const auto begun=std::make_shared<bool>(false); //delivery metadata, no Module/Level field
   const auto relay_failure_logged=std::make_shared<bool>(false); //one diagnostic per reached failure
   services.load_module_file_step=[files,begun,relay_failure_logged,validate](const std::string& uri,const char* tag,std::string& e){
    if(!validate(e))return LifecycleStepV36::failed;
    //Original Level.LoadFile(file,"Module") does not invoke Module.Load or
    //write18c/160. Begin a LEVEL occurrence in the SAME native relay instead.
    if(!*begun){if(!files->begin_level_file(e,true))return LifecycleStepV36::failed;*begun=true;}
    bool loaded{};
    if(!files->load_file(uri,tag,loaded,e)){
     if(!*relay_failure_logged){
      __android_log_print(ANDROID_LOG_ERROR,"DH2Native",
       "Stage9 Module file relay failed | uri=%s | root=%s | loaded=%d | nested=%s",
       uri.c_str(),tag?tag:"<null>",loaded?1:0,e.empty()?"<empty>":e.c_str());
      *relay_failure_logged=true;
     }
     return LifecycleStepV36::failed;
    }
    if(!validate(e))return LifecycleStepV36::failed;
    return loaded?LifecycleStepV36::complete:LifecycleStepV36::pending;
   };
   body=std::make_unique<Stage9BodyV53>(std::move(fields),std::move(services));
  }
  const auto result=body->step(e);
  if(failed){e=failure;return LifecycleStepV36::failed;}
  if(result==LifecycleStepV36::failed)return reject(e,"native LightSet source prefix");
  return result;
 }
};
static bool bind_source_stage9_v94(const std::shared_ptr<SourceWorldBorrowV61>& world,
 const std::shared_ptr<RendererSourceCandidateV55>& candidate,const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& level,
 dh2::loader::EarlyLoadingDebugV46 debug,dh2::loader::SourceLoadingInputsV43& loading,std::string& e){
 if(!world||!candidate||!candidate->preparation||candidate->preparation->level()!=level||!level||loading.external.stage_body[9]){
  e="Stage9 requires SAME retained candidate/Level and empty actual body slot";return false;
 }
 auto source=std::make_shared<SourceLightsetStage9V94>();source->world=world;source->candidate=candidate;source->level=level;
 source->trace=dh2::loader::Stage9TracePrefixV53(std::move(debug));
 loading.external.stage_body[9]=[source](std::string& e){
  try{return source->step(e);}catch(const std::exception& exception){e=exception.what();return source->reject(e,"native LightSet exception prefix");}
  catch(...){e="Actual Stage9 provider threw";return source->reject(e,"native LightSet exception prefix");}
 };
 //File leases/parse journals remain in preparation->module_files() and its
 //existing before-unload close/release path; no extra cleanup or D0 callback.
 e.clear();return true;
}

