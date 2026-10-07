// Positive source music/state/Level producers on SAME V42 manager and V67 owner.
namespace {
std::vector<std::weak_ptr<CampaignAudioRetirementV102>> campaign_audio_retirements_v106;
bool borrow_campaign_music_owner_v101(const std::shared_ptr<void>&world,
 std::shared_ptr<dh2::audio::AudioLevelGameplayV67>&out,std::string&e){
 out=campaign_level_audio_v101.lock();SourceCampaignCandidateBorrowV55 actual;
 if(!world||!out||!out->bridge()||out->bridge()->world()!=world||!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world!=world){
  if(e.empty())e="Required SAME retained current Level audio owner";return false;
 }
 dh2::audio::AudioApplicationBorrowV42 captured;
 if(!borrow_actual_application_audio_v42(captured,e)||captured.identity()!=out->bridge()->manager().identity()){
  if(e.empty())e="Required SAME process SoundManager for music transition";return false;
 }
 e.clear();return true;
}
}
bool set_campaign_music_state_v101(const std::shared_ptr<void>&world,const char*state,std::string&e){
 SourceCampaignCandidateBorrowV55 actual;dh2::audio::AudioApplicationBorrowV42 audio;
 if(!world||!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world!=world||
    !borrow_actual_application_audio_v42(audio,e)||!audio.manager){if(e.empty())e="Required SAME campaign/Application SetMusicState receiver";return false;}
 return audio.manager->set_music_state_v101(state,e);
}
bool play_campaign_music_v101(const std::shared_ptr<void>&world,int music,bool loop,bool stop,int fade,std::string&e){
 std::shared_ptr<dh2::audio::AudioLevelGameplayV67>owner;
 return borrow_campaign_music_owner_v101(world,owner,e)&&owner->play_music(music,loop,stop,fade,e);
}
bool stop_campaign_music_v117(const std::shared_ptr<void>& world,std::int32_t fade,std::string& e){
 std::shared_ptr<dh2::audio::AudioLevelGameplayV67> owner;
 return borrow_campaign_music_owner_v101(world,owner,e)&&owner->stop_music_v117(fade,e);
}
bool campaign_sound_index_v115(const std::shared_ptr<void>& world,const char* name,std::int32_t& index,std::string& e){
 std::shared_ptr<dh2::audio::AudioLevelGameplayV67> owner;
 if(!name){e="Required actual generated sound name";return false;}
 if(!borrow_campaign_music_owner_v101(world,owner,e))return false;
 auto* runtime=owner->bridge()->manager().manager->runtime_on_producer();
 if(!runtime||!runtime->source_data_initialized()){e="Required SAME initialized source sound bindings";return false;}
 index=runtime->bindings().source_id(name);e.clear();return true;
}
bool play_campaign_plain_sound_v115(const std::shared_ptr<void>& world,std::int32_t id,bool enabled,
 std::int32_t fade,std::int32_t group,bool bypass_online,std::string& e){
 std::shared_ptr<dh2::audio::AudioLevelGameplayV67> owner;
 return borrow_campaign_music_owner_v101(world,owner,e)&&owner->play_plain_v115(id,enabled,fade,group,bypass_online,e);
}
bool set_campaign_safe_zone_music_v101(const std::shared_ptr<void>&world,bool safe,std::string&e){
 std::shared_ptr<dh2::audio::AudioLevelGameplayV67>owner;
 return borrow_campaign_music_owner_v101(world,owner,e)&&owner->set_in_safe_zone_music(safe,e);
}
bool bind_campaign_music_aggro_v101(const std::shared_ptr<void>&world,SourceDeathRewardsLeavesV84&leaves,std::string&e){
 SourceCampaignCandidateBorrowV55 actual;if(!world||!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world!=world){if(e.empty())e="Required current canonical music producer World";return false;}
 auto previous=std::move(leaves.player_aggro);const std::weak_ptr<void>weak=world;
 leaves.player_aggro=[weak,previous=std::move(previous)](const auto&q,auto&out,std::string&e){
  using namespace dh2::character;auto world=weak.lock();
  if(!world){e="Released actual music/aggro producer World";return false;}
  if(q.service==player_aggro_current_level_v1){
   dh2::loader::CanonicalCurrentLevelBorrowV1 level;if(!borrow_current_native_level_v27(level,e))return false;
   if(!level){out.level={};e.clear();return true;}
   std::shared_ptr<dh2::audio::AudioLevelGameplayV67>owner;
   return borrow_campaign_music_owner_v101(world,owner,e)&&owner->borrow_aggro_level_v101(out.level,e);
  }
  if(q.service==player_aggro_set_music_state_v1)return set_campaign_music_state_v101(world,q.name,e);
  if(q.service==player_aggro_play_music_v1)return play_campaign_music_v101(world,q.music_id,q.loop!=0,q.flag!=0,q.fade,e);
  if(q.service==player_aggro_trace_v1){
   __android_log_print(ANDROID_LOG_INFO,"DH2Audio","AISPlayer aggro %s | player %llu | other %llu | count %d | weight %d",
    q.name?q.name:"",static_cast<unsigned long long>(q.player),static_cast<unsigned long long>(q.other),q.count,q.weight);
   e.clear();return true;
  }
  if(previous)return previous(q,out,e);
  e="Required existing non-music AISPlayer service "+std::to_string(q.service);return false;
 };
 e.clear();return true;
}
bool stop_application_sounds_v101(const dh2::audio::AudioApplicationBorrowV42&captured,int fade,std::string&e){
 dh2::audio::AudioStage0SoundV43 source;source.captured=captured;
 return source.stop_all(captured.identity(),fade,e);
}
bool stop_campaign_sound_v106(const std::shared_ptr<void>&world,int sound,int fade,std::string&e){
 return stop_campaign_sound_3d_v112(world,sound,fade,nullptr,0.f,e);
}
bool stop_campaign_sound_3d_v112(const std::shared_ptr<void>&world,int sound,int fade,const float* center,float radius,std::string&e){
 if(sound<0){e.clear();return true;} // original signed branch before any query
 if(!world){e="Required actual per-object Stop World";return false;}
 std::shared_ptr<CampaignAudioRetirementV102> retirement;
 for(auto i=campaign_audio_retirements_v106.begin();i!=campaign_audio_retirements_v106.end();){
  auto candidate=i->lock();if(!candidate){i=campaign_audio_retirements_v106.erase(i);continue;}
  if(!candidate->detached_&&candidate->world_.lock()==world)retirement=std::move(candidate);
  ++i;
 }
 if(retirement){
  auto owner=retirement->level_.lock();auto expected=retirement->level_bridge_.lock();
  if(owner&&expected&&owner->bridge()==expected)return owner->stop_3d_v112(sound,fade,center,radius,e);
  if(!retirement->manager_.manager){e="Required actual captured D2 SoundManager";return false;}
  return retirement->manager_.manager->stop_3d_v112(sound,fade,center,radius,{},e);
 }
 auto owner=campaign_level_audio_v101.lock();auto level_bridge=owner?owner->bridge():nullptr;
 if(level_bridge&&level_bridge->world()==world)return owner->stop_3d_v112(sound,fade,center,radius,e);
 auto bridge=campaign_audio_v46.lock();
 if(bridge){if(bridge->world()!=world){e="Per-object Stop requires captured old-world SoundManager";return false;}
  return bridge->manager().manager->stop_3d_v112(sound,fade,center,radius,{},e);}
 // Genuine prepublication D2 still reads the process SoundManager. This
 // fallback is its actual nullable global, not another World/playing provider.
 dh2::audio::AudioApplicationBorrowV42 audio;
 if(!borrow_actual_application_audio_v42(audio,e)||!audio.manager){if(e.empty())e="Required actual prepublication D2 SoundManager";return false;}
 return audio.manager->stop_3d_v112(sound,fade,center,radius,{},e);
}
bool take_application_audio_receipt_v101(const dh2::audio::AudioApplicationBorrowV42&captured,
 dh2::audio::AudioReceiptV34&receipt,bool&found,std::string&e){
 auto*runtime=captured.manager?captured.manager->runtime_on_producer():nullptr;
 if(!runtime){e="Required SAME producer runtime for sound retirement";return false;}
 runtime->pump_receipts();found=runtime->take_receipt(receipt);e.clear();return true;
}
bool application_audio_retirement_pending_v101(const dh2::audio::AudioApplicationBorrowV42&captured,bool&pending,std::string&e){
 auto*runtime=captured.manager?captured.manager->runtime_on_producer():nullptr;
 if(!runtime){e="Required SAME producer runtime for sound retirement query";return false;}
 runtime->pump_receipts();pending=runtime->owned_voices_v101()!=0||runtime->pinned_samples()!=0||runtime->mixer().active_voices()!=0;
 e.clear();return true;
}
bool release_campaign_audio_providers_v101(const std::shared_ptr<void>&world,std::string&e){
 auto owner=campaign_level_audio_v101.lock();auto bridge=campaign_audio_v46.lock();
 if(!owner||!bridge||bridge->world()!=world){e="Required SAME published Level audio providers for unload";return false;}
 if(!owner->release_source_providers_v101(world,e))return false;
 campaign_level_audio_v101.reset();campaign_audio_v46.reset();e.clear();return true;
}
bool capture_campaign_audio_retirement_v102(const std::shared_ptr<void>&world,
 std::shared_ptr<CampaignAudioRetirementV102>&out,std::string&e){
 if(!world){e="Required actual World at audio retirement capture";return false;}
 auto capture=std::shared_ptr<CampaignAudioRetirementV102>(new CampaignAudioRetirementV102);
 capture->world_=world;
 auto playback=campaign_audio_v46.lock();auto level=campaign_level_audio_v101.lock();
 if(playback&&playback->world()==world){
  capture->playback_=playback;capture->had_playback_=true;capture->manager_=playback->manager();
 }
 const auto level_bridge=level?level->bridge():nullptr;
 if(level_bridge&&level_bridge->world()==world){
  if(capture->manager_.manager&&capture->manager_.manager!=level_bridge->manager().manager){e="Captured audio providers belong to different Applications";return false;}
  capture->level_=level;capture->level_bridge_=level_bridge;capture->had_level_=true;capture->manager_=level_bridge->manager();
 }
 // No matching providers is a genuine prepublication absence, even if some
 // other World's providers are current. Record it without querying that World.
 if(!capture->providers_absent_at_capture()&&(!capture->manager_.manager||!capture->manager_.manager->runtime_on_producer())){
  e="Required SAME audio producer at provider retirement capture";return false;
 }
 if(capture->providers_absent_at_capture()){
  // Preserve the actual nullable manager for later ordinary D2 calls. Failure
  // to borrow it cannot turn genuinely absent providers into a capture error.
  dh2::audio::AudioApplicationBorrowV42 actual;std::string ignored;
  if(borrow_actual_application_audio_v42(actual,ignored))capture->manager_=std::move(actual);
 }
 campaign_audio_retirements_v106.emplace_back(capture);
 out=std::move(capture);e.clear();return true;
}
bool CampaignAudioRetirementV102::final_detach(bool&done,std::string&e){
 done=false;
 if(std::this_thread::get_id()!=producer_){e="Required captured owning audio retirement thread";return false;}
 if(detached_||providers_absent_at_capture()){detached_=done=true;e.clear();return true;}
 bool pending{};if(!application_audio_retirement_pending_v101(manager_,pending,e))return false;
 if(pending){e.clear();return true;}
 auto playback=playback_.lock();auto level=level_.lock();auto level_bridge=level_bridge_.lock();
 bool removed{};
 if(playback&&!dh2::android_audio::detach_expected_playback_v102(manager_,playback,removed,e))return false;
 if(level&&level_bridge&&!level->detach_captured_providers_v102(level_bridge,e))return false;
 // Compare captured control blocks, not current Level. Never reset a newer
 // V46/V67 registry entry; expired captured entries already require no work.
 if(had_playback_&&!campaign_audio_v46.owner_before(playback_)&&!playback_.owner_before(campaign_audio_v46))campaign_audio_v46.reset();
 if(had_level_&&!campaign_level_audio_v101.owner_before(level_)&&!level_.owner_before(campaign_level_audio_v101))campaign_level_audio_v101.reset();
 playback_.reset();level_bridge_.reset();level_.reset();manager_={};detached_=done=true;e.clear();return true;
}
namespace {
struct CampaignVoxOperationBorrowV101 {
 std::weak_ptr<void> world;
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 std::weak_ptr<dh2::audio::AudioApplicationManagerV42> manager;
 dh2::sound::VoxPlay3DServicesV2 prefix;
 std::uintptr_t identity{};
 std::string diagnostic;
 static int invoke(void*raw,const dh2::sound::VoxPlay3DRequestV2&q,dh2::sound::VoxPlay3DResponseV2&out){
  auto&self=*static_cast<CampaignVoxOperationBorrowV101*>(raw);using O=dh2::sound::VoxPlay3DOperationV2;
  auto manager=self.manager.lock();auto world=self.world.lock();auto&error=self.diagnostic;error.clear();
  if(!manager||!world||!q.play||q.play->manager!=self.identity||manager->identity()!=self.identity){error="Foreign or released Item Vox manager/World";return -1;}
  auto*runtime=manager->runtime_on_producer();if(!runtime){error="Required SAME Item Vox producer";return -1;}
  // Existing Item/loot receiver owns these source prefix operations. They
  // run once here, before any playing branch; never restart Play3D in submit.
  if(q.operation==O::disabled||q.operation==O::current_level)return self.prefix.invoke(self.prefix.context,q,out);
  if(q.operation==O::online){auto app=self.application.lock();if(!app)return -1;
   auto online=app->get_online_loading_v55();if(!online)return -1;out.value=online->byte5();return 0;}
  if(q.operation==O::platform_route){out.value=manager->disabled();return 0;}
  if(q.operation==O::sound_row||q.operation==O::bank_info)return runtime->source_operation_v101(q,out,0,error);
  auto bridge=campaign_audio_v46.lock();
  if(!bridge||bridge->world()!=world||bridge->manager().identity()!=self.identity){error="Required SAME published playing campaign audio providers";return -1;}
  const auto&services=bridge->services_on_producer();
  if(q.operation==O::trace)return services.actual_trace&&services.actual_trace(*q.play,q.selected_sound,error)?0:-1;
  if(q.operation==O::network_muted){bool muted{};if(!services.actual_network_muted||!services.actual_network_muted(muted,error))return -1;out.value=muted;return 0;}
  if(q.operation==O::platform_play)return services.actual_platform_play&&services.actual_platform_play(*q.play,error)?0:-1;
  if(q.operation==O::emit||q.operation==O::native_play)return runtime->source_operation_v101(q,out,dh2::audio::AudioAuthoredEventScopeV46::current(),error);
  return -1;
 }
};
}
bool borrow_campaign_vox_services_v101(const std::shared_ptr<void>&world,
 dh2::sound::VoxPlay3DServicesV2 prefix,dh2::audio::AudioApplicationBorrowV42&captured,
 dh2::sound::VoxPlay3DServicesV2&out,std::shared_ptr<void>&provider,std::string&e){
 if(!world||!prefix.invoke){e="Required actual World and existing Item Debug/current-Level prefix";return false;}
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>app;
 if(!borrow_actual_application_services_v5(app,e)||!borrow_actual_application_audio_v42(captured,e)||!captured.manager||!captured.manager->runtime_on_producer()){
  if(e.empty())e="Required SAME process Application SoundManager for Item Vox";return false;
 }
 auto owner=std::make_shared<CampaignVoxOperationBorrowV101>();owner->world=world;owner->application=app;
 owner->manager=captured.manager;owner->identity=captured.identity();owner->prefix=prefix;
 out={owner.get(),CampaignVoxOperationBorrowV101::invoke};provider=std::move(owner);e.clear();return true;
}
bool campaign_vox_diagnostic_v101(const std::shared_ptr<void>&exact_borrowed_provider,std::string&e){
 // The caller passes the exact lease returned by borrow_campaign_vox_services,
 // not its containing World/pool or the existing prefix context.
 if(!exact_borrowed_provider){e="Expired Item Vox provider diagnostic";return false;}
 e=std::static_pointer_cast<CampaignVoxOperationBorrowV101>(exact_borrowed_provider)->diagnostic;return true;
}
