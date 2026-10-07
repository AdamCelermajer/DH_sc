// Include after renderer_source_candidate_v55.inc. This replaces only the
// Android development Play transport. Whole NativeStartGame/Application.LoadLevel
// save/global/GameSM prefixes remain separately required source work.
namespace {
struct RendererSourceCampaignV55 {
 std::shared_ptr<WorldScriptContext> world;
 std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50> profile;
 std::shared_ptr<RendererLevelApplicationV25> application;
 std::shared_ptr<RendererSourceCandidateV55> candidate;
 std::shared_ptr<dh2::camera::GameplayCameraApplicationV23> camera_application;
 std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59> player_manager;
 dh2::loader::GSLevelServicesV2<dh2::loader::CanonicalLevelContextV1> menu;
 dh2::loader::NativeRootGSBootstrapV55 bootstrap;
 bool failed{};std::string failure;
};
std::unique_ptr<RendererSourceCampaignV55> source_campaign_v55;
struct SourceApplicationDebugTransportV55 {
 std::shared_ptr<RendererLevelFilesV25> files;
 std::shared_ptr<dh2::character::DebugSwitches> actual;
 dh2::character::DebugFileServices24 service{this,open,close};
 static int open(void* raw,const char* name,std::uintptr_t* out){
  auto& self=*static_cast<SourceApplicationDebugTransportV55*>(raw);
  if(!self.files||!name||std::strcmp(name,"DebugSwitches.savegame")||!out)return 1;
  *out=0;errno=0;const auto path=self.files->directory+"/"+name;
  auto* file=std::fopen(path.c_str(),"rb");
  if(!file)return errno==ENOENT?0:1;
  *out=reinterpret_cast<std::uintptr_t>(file);return 0;
 }
 static int close(void*,std::uintptr_t file){return !file||std::fclose(reinterpret_cast<std::FILE*>(file))?1:0;}
 bool trace(std::string& e){
  std::uint32_t ignored{};
  if(!actual||dh2_character_debug_load(actual.get(),&service)!=1||
     dh2_character_debug_get(&ignored,actual.get(),"isTracingAnimSetManager",&service)!=1){
   e="Required same retained Application Debug resource delivery";return false;
  }
  e.clear();return true;
 }
};

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
 candidate.roots=state.camera_application?state.camera_application->roots():nullptr;
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
 if(!model_renderer::bind_root_stage0_sound_v44(out.stage0,error))return false;
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

bool start_source_campaign_impl_v55(AAssetManager* assets,
 std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50> profile,std::string& error)try{
 using namespace dh2;using namespace dh2::loader;
 if(source_campaign_v55){error="Source campaign already retained; require whole source unload before a new launch";return false;}
 if(!assets||!profile||profile->metadata.slot<0||profile->metadata.selected_difficulty<0||profile->metadata.selected_difficulty>=3){
  error="Required immutable selected profile and genuine per-difficulty location";return false;
 }
 if(renderer_gs_globals_v27->s_level){error="Original GS current Level already exists; require source transition/unload";return false;}
 if(world_mode){error="Prior development gameplay still owns live actors; require genuine ordered Level unload before campaign launch";return false;}
 const auto difficulty=std::size_t(profile->metadata.selected_difficulty);
 // Source input identity is selected from the exact saved LNAM row. A missing
 // row is not converted into a Crypt, Swamp or default-class fallback.
 const auto immutable=source_immutable_tables_v55(assets);const auto& design=immutable->design;
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
 // The actual Application PM C1/local PlayerInfo precedes GS and Character.
 // Controller count/slot are source InputManager queries, not phone-device
 // guesses, Hero-preview fields or a forced Character publication.
 std::shared_ptr<input::SourceInputManagerV60> source_input;
 if(!borrow_actual_input_manager_v60(source_input,error)||!source_input||
    !prepare_source_application_player_manager_v59(application,state->player_manager,error)){
  if(error.empty())error="Required actual Application InputManager/PlayerManager startup";
  state->failed=true;state->failure=error;source_campaign_v55=std::move(state);return false;
 }
 // Helper performs source count/GetGamepad/GetOnline/map checks; an existing
 // receiver is preserved rather than having C1/AddPlayer replayed.
 if(!state->player_manager->source_first_local_add_prefix(source_input->first_local_services(),error)){
  state->failed=true;state->failure=error;source_campaign_v55=std::move(state);return false;
 }
 player::PlayerInfoFieldsV1* actual_local{};
 if(!state->player_manager->get_local_player(0,false,actual_local,error)||!actual_local||actual_local->character660){
  if(error.empty())error="Prior source Character still exists; require whole ordered Level unload";
  state->failed=true;state->failure=error;source_campaign_v55=std::move(state);return false;
 }
 camera::CameraApplicationBindingsV23 camera_bindings;
 if(!application->native_camera_services_v20()){
  camera_bindings.actual_roots=std::make_shared<world::GameObjectSceneRootRegistryV1>();
  camera_bindings.actual_dictionary=&actor_clip_table;
  camera_bindings.actual_dictionary_lease=std::shared_ptr<void>(&actor_clip_table,[](void*){}); // Existing process-static dictionary.
  camera_bindings.actual_file_system=std::shared_ptr<void>(state->application->files,assets);
  camera_bindings.backend.configured_backend=camera_bindings.actual_file_system;
  camera_bindings.backend.viewport=borrow_actual_camera_viewport_v20;
  camera_bindings.actual_lg_devices=borrow_actual_camera_lg_device_v20;
  camera_bindings.read=[files=state->application->files](const std::string& name,std::vector<std::uint8_t>& bytes,std::string& e){
   bool found{};if(!files->cache.read(name,found,bytes,e))return false;
   if(!found){e="Original source camera resource absent: "+name;return false;}e.clear();return true;
  };
  auto source_debug=std::make_shared<SourceApplicationDebugTransportV55>();
  source_debug->files=state->application->files;source_debug->actual=state->world->debug;
  camera_bindings.debug_after_add=[source_debug](std::string& e){return source_debug->trace(e);};
 }
 if(!borrow_gameplay_camera_services_v20(application,std::move(camera_bindings),state->camera_application,error))return false;
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
 // Source __LoadLevelName468b78 writes LNAM seeds at Save+5c; LEPT is
 // Save+40. NativeStartGame passes LEPT as Application's signed argument2;
 // Application32c008 reads seed5c for BOTH GS unsigned24/28 cells.
 std::uint32_t source_seed=std::uint32_t(location.seeds[difficulty]);
 if(!source_seed)source_seed=std::uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(
   std::chrono::steady_clock::now().time_since_epoch()).count()); // Native Timer.getRealTime backend.
 input.arguments={input.request.definition,location.entry_points[difficulty],input.request.seed,
  source_seed,source_seed,1,location.use_spawn_point[difficulty],-1,state->profile->requested_difficulty};
 input.online_state=[](std::uint32_t&,std::string& e){e="Required actual OnlineGameState34 producer";return false;};
 input.constructor.online_byte5=[online](std::uint8_t& out,std::string& e){out=online->byte5();e.clear();return true;};
 input.constructor.online_state34=[](std::int32_t&,std::string& e){e="Required actual OnlineGameState34 producer";return false;};
 input.constructor.flush_animation_sets=[camera=state->camera_application](std::string& e){
  if(!camera||!camera->ready()||!camera->animation_manager()){e="Required SAME initialized Application AnimSetManager";return false;}
  camera->animation_manager()->source_flush_v17();e.clear();return true;
 };
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
 last_frame=std::chrono::steady_clock::now();
 error.clear();return true;
}catch(const std::exception& failure){
 error=failure.what();
 if(source_campaign_v55){source_campaign_v55->failed=true;source_campaign_v55->failure=error;}
 return false;
}catch(...){
 error="Source campaign startup provider threw";
 if(source_campaign_v55){source_campaign_v55->failed=true;source_campaign_v55->failure=error;}
 return false;
}
bool source_campaign_active_impl_v55(){return bool(source_campaign_v55);}
bool borrow_source_campaign_candidate_impl_v55(SourceCampaignCandidateBorrowV55& out,std::string& error){
 if(!source_campaign_v55||!source_campaign_v55->world||!source_campaign_v55->candidate||
    !source_campaign_v55->application){error="Required actually produced source campaign graph";return false;}
 auto& state=*source_campaign_v55;const auto& candidate=state.candidate;
 const auto& gs=state.bootstrap.gs();
 if(!gs||!gs->connection()||!gs->connection()->level_connection()->complete()||
    !candidate->preparation||candidate->preparation->level()!=gs->fields().level34||
    renderer_gs_globals_v27->s_level!=gs->fields().level34){
  error="Required SAME completed GS/C1/global/candidate lifetime";return false;
 }
 out={state.world,state.application->application,gs->fields().level34,
   std::shared_ptr<dh2::world::CanonicalObjectManagerV1>(candidate->canonical,&candidate->canonical->manager),
   &candidate->canonical->properties,state.profile};
 error.clear();return true;
}
bool bind_source_campaign_class_factory_impl_v60(SourceCampaignClassFactoryV60 factory,std::string& error){
 SourceCampaignCandidateBorrowV55 actual;
 if(!factory||!borrow_source_campaign_candidate_impl_v55(actual,error)){
  if(error.empty())error="Required real canonical source class factory";return false;
 }
 auto& state=*source_campaign_v55;
 const auto& fields=actual.level->constructor_fields_v3();
 if(state.failed||fields.field130>=7||!state.candidate->factory||state.candidate->factory->remaining){
  error="Source class factory must bind once before XML construction; failed/loading prefix cannot be replayed";return false;
 }
 state.candidate->factory->remaining=std::move(factory);error.clear();return true;
}
bool tick_source_campaign_impl_v55(std::string& error){
 if(!source_campaign_v55){error="Required retained source campaign";return false;}
 auto& state=*source_campaign_v55;
 if(state.failed){error=state.failure;return false;}
 if(!state.world||!state.world->native_loading_v50){error="Required SAME actual GS loading connection";return false;}
 const auto now=std::chrono::steady_clock::now();
 const auto now_ms=std::uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());
 const auto old_ms=std::uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(last_frame.time_since_epoch()).count());
 const auto dt=now_ms-old_ms;last_frame=now;
 current_application_dt=dt;current_application_tick=dt<=2000;
 if(!current_application_tick){error.clear();return true;} // Original Application long-gap skip, no clamp.
 const auto result=state.world->native_loading_v50->tick();
 if(result==dh2::loader::GSLevelUpdateResultV44::failed){state.failed=true;state.failure=state.world->native_loading_v50->error();error=state.failure;return false;}
 error.clear();return true;
}
