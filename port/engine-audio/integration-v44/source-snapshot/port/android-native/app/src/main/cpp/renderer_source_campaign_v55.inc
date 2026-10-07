// Include after renderer_source_candidate_v55.inc. This replaces only the
// Android development Play transport. Whole NativeStartGame/Application.LoadLevel
// save/global/GameSM prefixes remain separately required source work.
namespace {
struct RendererSourceCampaignV55 {
 std::shared_ptr<WorldScriptContext> world;
 std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50> profile;
 std::shared_ptr<RendererLevelApplicationV25> application;
 std::shared_ptr<RendererSourceCandidateV55> candidate;
 dh2::loader::GSLevelServicesV2<dh2::loader::CanonicalLevelContextV1> menu;
 dh2::loader::NativeRootGSBootstrapV55 bootstrap;
 bool failed{};std::string failure;
};
std::unique_ptr<RendererSourceCampaignV55> source_campaign_v55;

bool source_debug_load_v55(const std::weak_ptr<WorldScriptContext>& weak,std::string& error){
 auto world=weak.lock();
 if(!world||!world->debug){error="Required same Application Debug source owner";return false;}
 const auto result=dh2_character_debug_load(world->debug.get(),&world->debug_files);
 if(result!=1){error="Original DebugSwitches.load failed in bounded source domain: "+std::to_string(result);return false;}
 error.clear();return true;
}
bool source_debug_switch_v55(const std::weak_ptr<WorldScriptContext>& weak,
 const char* key,bool& value,std::string& error){
 auto world=weak.lock();std::uint32_t actual{};
 if(!world||!world->debug||!key){error="Required same actual Debug/key";return false;}
 const auto result=dh2_character_debug_get(&actual,world->debug.get(),key,&world->debug_files);
 if(result!=1){error="Original DebugSwitches.GetSwitch failed: "+std::to_string(result);return false;}
 value=actual!=0;error.clear();return true;
}

bool connect_source_campaign_candidate_v55(RendererSourceCampaignV55& state,
 const std::shared_ptr<dh2::loader::NativeGSLevelRuntimeV27>& gs,
 const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& actual,
 dh2::loader::NativeRootLoadingInputsV50& out,std::string& error){
 using namespace dh2;using namespace dh2::loader;
 auto current_world=state.world;const auto application=state.application->application;
 const auto weak=std::weak_ptr<WorldScriptContext>(current_world);
 RendererSourceCandidateInputsV55 candidate;
 candidate.world=current_world;candidate.level=actual;candidate.application=state.application;
 candidate.profile=state.profile;
 candidate.roots=std::make_shared<world::GameObjectSceneRootRegistryV1>();
 candidate.floors=std::make_shared<floors::World>();
 candidate.map=std::make_shared<world::SceneManagerMapOwnerV2>(candidate.roots);
 candidate.config.owner=state.application;
 candidate.config.debug_switch=[weak](const char* key,bool& value,std::string& e){return source_debug_switch_v55(weak,key,value,e);};
 // Actual arrays/sound catalog and remaining canonical class routes must be
 // supplied at their source call boundaries; no replacement LevelConfig or
 // Item is synthesized here. The factory retains each completed source prefix.
 candidate.config.set_level_config=[](std::uintptr_t,std::string& e){e="Required actual Arrays.Sounds/Level.SetLevelConfig producer";return false;};
 candidate.module_graph.read_asset=[files=state.application->files](const std::string& name,
   std::shared_ptr<const std::vector<std::uint8_t>>& bytes,bool& found,std::string& e){
  auto result=std::make_shared<std::vector<std::uint8_t>>();
  if(!files->cache.read(name,found,*result,e))return false;
  bytes=found?result:nullptr;e.clear();return true;
 };
 candidate.unknown_type_debug=[](const char* name,std::string& e){
  if(!name){e="Required unknown source factory type CString";return false;}
  __android_log_print(ANDROID_LOG_WARN,"DH2Native","Source unknown factory type | %s",name);e.clear();return true;
 };
 if(!RendererSourceCandidateV55::create(std::move(candidate),state.candidate,error))return false;
 if(!state.candidate->lend_loading_inputs_v55(out.source,error))return false;
 out.manager_freshness=state.candidate->manager_freshness;
 auto& fields=application->source_loading_v55();
 out.application.actual_owner=application;out.application.byte_ec=&fields.byte_ec;
 out.stage0_application={application,application,application->identity(),
  &fields.g_bigI9f6890,&fields.g_bigV9f688c,&fields.byte_b4};
 out.stage0.clean_glitch=[application](const Stage0ApplicationBorrowV46& actual_app,std::string& e){
  if(actual_app.application_owner.get()!=application.get()||actual_app.application_identity!=application->identity()){
   e="Foreign Application.CleanGlitch receiver";return false;
  }
  return model_renderer::clean_native_graphics_v50(e);
 };
 // This aliases the SAME process backend already used by native bodies. Stage5
 // invokes its actual load exactly once; no second PhysicalWorld is allocated.
 out.physical_world=std::shared_ptr<physical::NativeWorld>(application,&actor_world);
 out.prefix.actual_application_owner=application;out.prefix.cheat_global_owner=application;
 out.prefix.handle_cheats_inGame=&fields.handle_cheats_inGame9f640a;
 out.prefix.debug_owner=current_world->debug;
 out.prefix.debug_load=[weak](std::string& e){return source_debug_load_v55(weak,e);};
 out.prefix.debug_get_switch=[weak](const char* key,bool& value,std::string& e){return source_debug_switch_v55(weak,key,value,e);};
 out.prefix.debug_instance_get_switch=out.prefix.debug_get_switch;
 // Native log transport for original _DEBUG_OUT's formatted loading-step
 // message. No source loading mutation or work-completion counter is added.
 out.prefix.debug_out_loading_step=[](std::uint32_t phase,std::string& e){
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original loading step | %u",phase);e.clear();return true;
 };
 out.early_debug={current_world->debug,out.prefix.debug_instance_get_switch};
 out.frame.actual_debug_owner=current_world->debug;
 out.frame.debug_load=out.prefix.debug_load;
 out.frame.debug_get_switch=out.prefix.debug_get_switch;
 // Positive Lua_DumpCalls requires the original SetSwitch/save provider; it
 // is not silently downgraded to the missing-file false source branch.
 out.frame.debug_set_switch=[](const char*,bool,std::string& e){e="Required actual DebugSwitches.SetSwitch/save positive branch";return false;};
 if(!model_renderer::borrow_native_gs_frame_menu_v58(current_world,out.frame,error))return false;
 out.source.external.stage_body[2]=[weak,trace=out.early_debug](std::string& e){
  if(!early_loading_trace_v46(trace,e))return LifecycleStepV36::failed;
  auto world=weak.lock();if(!world){e="Released actual stage2 World";return LifecycleStepV36::failed;}
  for(const auto id:{2,1,3})if(!model_renderer::unload_native_menu_v58(world,id,e))return LifecycleStepV36::failed;
  e.clear();return LifecycleStepV36::complete;
 };
 out.source.external.publish_progress=[menu=state.menu,actual](std::int32_t phase,std::int32_t progress,std::string& e){
  CanonicalLevelContextV1::LoadingFieldsV26 fields;
  if(!actual->loading_fields_v26(fields,e)||!fields.state130||!fields.progress30||
     *fields.state130!=std::uint32_t(phase)||*fields.progress30!=std::uint32_t(progress)){
   if(e.empty())e="Source progress must observe SAME actual C1 scalar tail";return false;
  }
  if(phase==38){e="Required actual final source license global/Menu.HUD Push tail";return false;}
  std::uintptr_t receiver{},render{},character{};
  if(!menu.get_menu||!menu.get_menu("Loading",receiver,e))return false;
  if(!receiver){e.clear();return true;} // Genuine GetMenu NULL source branch.
  if(!menu.menu_render_fx||!menu.menu_render_fx(receiver,render,e)||
     !menu.check_menu_weak_proxy||!menu.check_menu_weak_proxy(receiver,e)||
     !menu.menu_character||!menu.menu_character(receiver,character,e)||
     !menu.invoke_as_no_arguments)return false;
  return menu.invoke_as_no_arguments(render,character,"onProgress",e);
 };
 // All source body/progress/tail authorities remain explicit. Later root
 // stages bind their genuine providers into this SAME candidate connection.
 (void)gs;
 error.clear();return true;
}
}

bool start_source_campaign_v55(AAssetManager* assets,
 std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50> profile,std::string& error){
 using namespace dh2;using namespace dh2::loader;
 if(source_campaign_v55){error="Source campaign already retained; require whole source unload before a new launch";return false;}
 if(!profile||profile->metadata.slot<0||profile->metadata.selected_difficulty<0||profile->metadata.selected_difficulty>=3){
  error="Required immutable selected profile and genuine per-difficulty location";return false;
 }
 if(renderer_gs_globals_v27->s_level){error="Original GS current Level already exists; require source transition/unload";return false;}
 const auto difficulty=std::size_t(profile->metadata.selected_difficulty);
 // Source input identity is selected from the exact saved LNAM row. A missing
 // row is not converted into a Crypt, Swamp or default-class fallback.
 auto design=renderer_immutable_tables_v55?renderer_immutable_tables_v55->design:
  (live_script_world?live_script_world->design:load_game_design(assets));
 auto tables=design->borrow();const auto* levels=tables.levels();
 const auto row=profile->metadata.location.levels[difficulty];
 if(!levels||row<0||std::size_t(row)>=levels->levels.size()||levels->level_names.size()!=levels->levels.size()){
  error="Required genuine selected LNAM row or original Settings default-row producer";return false;
 }
 auto state=std::make_unique<RendererSourceCampaignV55>();state->profile=std::move(profile);
 state->world=create_source_world_v55(assets,state->profile,levels->level_names[std::size_t(row)],error);
 if(!state->world)return false;
 std::shared_ptr<application::ApplicationServicesOwnerV5> application;
 if(!borrow_actual_application_services_v5(application,error))return false;
 if(!renderer_level_application_v25||renderer_level_application_v25->application!=application||
    renderer_level_application_v25->files->directory!=state->profile->private_directory)
  renderer_level_application_v25=std::make_shared<RendererLevelApplicationV25>(application,assets,state->profile->private_directory);
 state->application=renderer_level_application_v25;
 auto held_design=std::make_shared<character::CharacterGameDesign::Borrow>(state->world->design->borrow());
 LevelConstructorApplicationV4 app;app.owner=state->application;app.levels=held_design->levels();
 app.lua.owner=held_design;app.lua.design=*held_design->design();app.private_vm_limit=16u*1024u*1024u;
 const auto online=application->get_online_loading_v55();
 app.online_byte5=[online](std::uint8_t& out,std::string& e){out=online->byte5();e.clear();return true;};
 app=state->application->source->bind(std::move(app));
 NativeRootGSBootstrapInputsV55 input;input.globals=renderer_gs_globals_v27;
 input.application=std::move(app);input.request.identity=levels->level_names[std::size_t(row)];
 input.request.definition=levels->levels[std::size_t(row)].file;
 input.request.seed=std::uint32_t(state->profile->metadata.slot);
 const auto& location=state->profile->metadata.location;
 std::uint32_t entry=std::uint32_t(location.entry_points[difficulty]);
 if(!entry)entry=std::uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(
   std::chrono::steady_clock::now().time_since_epoch()).count()); // Native Timer.getRealTime backend.
 input.arguments={input.request.definition,location.seeds[difficulty],input.request.seed,
  entry,entry,1,location.use_spawn_point[difficulty],-1,state->profile->requested_difficulty};
 input.online_state=[](std::uint32_t&,std::string& e){e="Required actual OnlineGameState34 producer";return false;};
 input.constructor.online_byte5=[online](std::uint8_t& out,std::string& e){out=online->byte5();e.clear();return true;};
 input.constructor.online_state34=[](std::int32_t&,std::string& e){e="Required actual OnlineGameState34 producer";return false;};
 input.constructor.flush_animation_sets=[](std::string& e){try{flush_gameplay_animation_sets_v27();e.clear();return true;}catch(const std::exception& failure){e=failure.what();return false;}};
 auto* stable=state.get();const auto weak=std::weak_ptr<WorldScriptContext>(state->world);
 input.bind_loading_callbacks=[stable,weak,application](ui::LoadingMenuStateServicesV1 loading,
   GSLevelServicesV2<CanonicalLevelContextV1>& constructor,std::string& e){
  auto world=weak.lock();if(!world){e="Expired fresh source World before GS bootstrap";return false;}
  auto debug=[weak](const char* key,std::int32_t& out,std::string& e){
   bool value{};if(!source_debug_load_v55(weak,e)||!source_debug_switch_v55(weak,key,value,e))return false;out=value;return true;
  };
  if(!borrow_native_gs_menu_v27(world,loading,std::move(debug),constructor,e)||
     !bind_native_menu_application_ec_v58(world,application,&application->source_loading_v55().byte_ec,e))return false;
  stable->menu=constructor;return true;
 };
 // Constructor services need menu registration before the synchronous GS C1
 // callback. NativeRootGSBootstrap transfers this same mutable service closure.
 input.connect_candidate=[stable](const auto& gs,const auto& level,auto& out,std::string& e){
  return connect_source_campaign_candidate_v55(*stable,gs,level,out,e);
 };
 source_campaign_v55=std::move(state); // Retain failed source prefixes BEFORE begin.
 if(!source_campaign_v55->bootstrap.begin(std::move(input),error)){
  source_campaign_v55->failed=true;source_campaign_v55->failure=error;return false;
 }
 auto& world=*source_campaign_v55->world;
 if(!source_campaign_v55->bootstrap.adopt_into(world.native_gslevel_v27,world.native_loading_v50,error)){
  source_campaign_v55->failed=true;source_campaign_v55->failure=error;return false;
 }
 world.native_level_c1_v25=world.native_gslevel_v27->connection()->level_connection();
 error.clear();return true;
}
bool source_campaign_active_v55(){return bool(source_campaign_v55);}
bool tick_source_campaign_v55(std::string& error){
 if(!source_campaign_v55){error="Required retained source campaign";return false;}
 auto& state=*source_campaign_v55;
 if(state.failed){error=state.failure;return false;}
 if(!state.world||!state.world->native_loading_v50){error="Required SAME actual GS loading connection";return false;}
 const auto result=state.world->native_loading_v50->tick();
 if(result==dh2::loader::GSLevelUpdateResultV44::failed){state.failed=true;state.failure=state.world->native_loading_v50->error();error=state.failure;return false;}
 error.clear();return true;
}
