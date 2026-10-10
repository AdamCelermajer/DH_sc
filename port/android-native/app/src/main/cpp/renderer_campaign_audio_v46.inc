// Audio-only publication. Include after renderer_source_boundary_v61.inc.
namespace {std::weak_ptr<dh2::audio::AudioCampaignBridgeV46> campaign_audio_v46;
 std::weak_ptr<dh2::audio::AudioLevelGameplayV67> campaign_level_audio_v101;}
static bool prepare_campaign_audio_services_v46(dh2::audio::AudioCampaignServicesV46&services,std::string&error){
 SourceCampaignCandidateBorrowV55 actual;
 if(!borrow_source_campaign_candidate_v55(actual,error))return false;
 auto world=std::static_pointer_cast<WorldScriptContext>(actual.actual_world);
 if(!world||!world->native_gslevel_v27){error="Required selected source World and SAME active GS";return false;}
 if(services.actual_world&&services.actual_world!=actual.actual_world){error="Foreign campaign audio World";return false;}
 if(services.selected_level&&services.selected_level!=actual.level){error="Foreign campaign audio Level";return false;}
 services.actual_world=actual.actual_world;services.actual_gs=world->native_gslevel_v27;services.selected_level=actual.level;
 if(!services.camera_owner&&actual.application)services.camera_owner=actual.application->native_camera_services_v20();
 services.current_level=[](std::shared_ptr<dh2::loader::CanonicalLevelContextV1>&out,std::string&e){
  dh2::loader::CanonicalCurrentLevelBorrowV1 actual;
  if(!model_renderer::borrow_current_native_level_v27(actual,e))return false;
  out=actual.level();e.clear();return true;
 };
 services.actual_debug=[weak=std::weak_ptr<WorldScriptContext>(world)](const char*key,bool&out,std::string&e){
  auto current=weak.lock();std::uint32_t value{};
  if(!current||!key||!current->debug||dh2_character_debug_load(current->debug.get(),&current->debug_files)!=1||
     dh2_character_debug_get(&value,current->debug.get(),key,&current->debug_files)!=1){e="Required actual source campaign Debug getter";return false;}
  out=value!=0;e.clear();return true;
 };
 services.actual_online=[application=actual.application](bool&out,std::string&e){
  const auto online=application?application->get_online_loading_v55():nullptr;
  if(!online){e="Required SAME actual Application Online byte5 owner";return false;}
  out=online->byte5()!=0;e.clear();return true;
 };
 // Original trace operation only queries this exact Debug key; its returned
 // value is not a readiness predicate or extra logging command.
 services.actual_trace=[debug=services.actual_debug](const dh2::character::CombatSoundPlayV1&,int,std::string&e){bool traced{};return debug("isTracingSFX",traced,e);};
 if(!services.exact_legacy_listener_stream){
  if(!renderer_level_application_v25||renderer_level_application_v25->application!=actual.application||
     renderer_level_application_v25->files->directory!=world->files_directory){error="Required SAME campaign source file owner";return false;}
  auto bytes=std::make_shared<std::vector<std::uint8_t>>();bool found=false;
  //Arrays stage21 loads pydata/sounds_pyarray.bin beneath RES_PATH data/.
  //The APK cache key is exact; the loose legacy fixture omits pydata/.
  //Use the same original stream as CharacterCandidateCache CharSounds;
  //audio_listener_rows_v38 advances over those rows to serialized Listener.
  constexpr const char* listener_uri="data/pydata/sounds_pyarray.bin";
  if(!renderer_level_application_v25->files->cache.read(listener_uri,found,*bytes,error))return false;
  if(!found){error=std::string("Required exact legacy Listener stream: ")+listener_uri;return false;}
  services.exact_legacy_listener_stream=std::move(bytes);
 }
 error.clear();return true;
}
bool publish_campaign_audio_v46(dh2::audio::AudioCampaignServicesV46 services,std::string&error){
 if(!prepare_campaign_audio_services_v46(services,error))return false;
 auto owner=dh2::audio::AudioCampaignBridgeV46::publish(std::move(services),error);
 if(!owner)return false;
 campaign_audio_v46=owner;error.clear();return true;
}
bool compose_campaign_level_audio_v67(dh2::audio::AudioCampaignServicesV46 services,
 dh2::audio::AudioLevelBindingsV67 bindings,dh2::loader::LevelGameplayServicesV66&gameplay,
 std::shared_ptr<dh2::audio::AudioLevelGameplayV67>&out,std::string&error){
 SourceCampaignCandidateBorrowV55 actual;
 if(!borrow_source_campaign_candidate_v55(actual,error)||!prepare_campaign_audio_services_v46(services,error))return false;
 if((bindings.camera&&bindings.camera!=actual.camera_application)||(bindings.roots&&bindings.roots!=actual.roots)){
  error="Foreign Level audio camera/SceneManager";return false;
 }
 bindings.camera=actual.camera_application;bindings.roots=actual.roots;
 auto owner=dh2::audio::AudioLevelGameplayV67::compose(std::move(services),std::move(bindings),gameplay,error);
 if(!owner)return false;campaign_audio_v46=owner->bridge();campaign_level_audio_v101=owner;out=std::move(owner);error.clear();return true;
}
bool submit_campaign_audio_v46(dh2::audio::AudioCategoryV46 category,const dh2::audio::AudioApplicationBorrowV42&captured,const std::shared_ptr<void>&actual_world,
 const dh2::character::CombatSoundPlayV1&play,std::string&error){
 auto owner=campaign_audio_v46.lock();
 if(owner&&(!actual_world||actual_world!=owner->world())){error="Sound producer belongs to another campaign World";return false;}
 if(owner&&dh2::audio::AudioAuthoredEventScopeV46::current()<=0)return owner->source_prefix_without_clock(play,error);
 if(!owner||dh2::audio::AudioAuthoredEventScopeV46::current()<=0){
  // A genuine source early return does not require a device/event clock.
  // Run its real Debug/current-Level prefix once, then require publication
  // if a playing branch is actually reached. No legacy phase copy is used.
  struct Early {std::shared_ptr<WorldScriptContext>world;std::string&error;};
  Early early{std::static_pointer_cast<WorldScriptContext>(actual_world),error};
  if(!early.world||!captured.manager||play.manager!=captured.identity()){error="Required actual early source audio receiver";return false;}
  dh2::sound::VoxPlay3DOwnerV2 prefix({&early,[](void*raw,const dh2::sound::VoxPlay3DRequestV2&q,dh2::sound::VoxPlay3DResponseV2&out){
   auto&ctx=*static_cast<Early*>(raw);using O=dh2::sound::VoxPlay3DOperationV2;
   if(q.operation==O::disabled){std::uint32_t flag{};
    if(!ctx.world->debug){ctx.error="Early source Vox Play3D disabled gate lost SAME World Debug owner";return -1;}
    const int loaded=dh2_character_debug_load(ctx.world->debug.get(),&ctx.world->debug_files);
    if(loaded!=1){ctx.error="Early source Vox Play3D Debug.Load(IsDisablingSounds) status "+std::to_string(loaded);return -1;}
    const int queried=dh2_character_debug_get(&flag,ctx.world->debug.get(),"IsDisablingSounds",&ctx.world->debug_files);
    if(queried!=1){ctx.error="Early source Vox Play3D Debug.GetSwitch(IsDisablingSounds) status "+std::to_string(queried);return -1;}
    out.value=flag;return 0;
   }
   if(q.operation==O::current_level){dh2::loader::CanonicalCurrentLevelBorrowV1 current;
    if(!borrow_current_native_level_v27(current,ctx.error))return -1;
    out.identity=current.identity();out.value=0;
    if(current){dh2::loader::CanonicalLevelContextV1::LoadingFieldsV26 fields;
     if(!current.level()->loading_fields_v26(fields,ctx.error)||!fields.state130)return -1;
     out.value=std::int32_t(*fields.state130);
    }
    return 0;
   }
   ctx.error="Required published playing-branch providers and actual event time";return -1;
  }});
  const int status=prefix.play(play);
  if(status){if(error.empty())error=prefix.error();return false;}
  error.clear();return true;
 }
 if(!actual_world||actual_world!=owner->world()){error="Sound producer belongs to another campaign World";return false;}
 if(!captured.manager||captured.identity()!=owner->manager().identity()){error="Foreign/cancelled actual campaign SoundManager";return false;}
 return owner->submit(category,play,dh2::audio::AudioAuthoredEventScopeV46::current(),error);
}
