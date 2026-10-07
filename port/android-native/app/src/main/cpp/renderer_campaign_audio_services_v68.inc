// Concrete source audio providers on the SAME persistent Application manager.
// Include after renderer_campaign_audio_v46; no player/HUD projection is used.
namespace {
struct CampaignAudioSourceLeaseV68 {
 std::weak_ptr<void> world;
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 dh2::audio::AudioApplicationBorrowV42 audio;
 std::shared_ptr<dh2::ui::OwnedHudSettingsV1> settings;
 bool current(std::string&e)const{
  SourceCampaignCandidateBorrowV55 actual;auto app=application.lock();auto owner=world.lock();
  if(!app||!owner||!borrow_source_campaign_candidate_v55(actual,e)||actual.application!=app||actual.actual_world!=owner||
     !audio.manager||!audio.manager->runtime_on_producer()){
   if(e.empty())e="Required SAME current campaign/settings/audio producer";return false;
  }
  e.clear();return true;
 }
 static bool random(void*raw,int&out){auto&self=*static_cast<CampaignAudioSourceLeaseV68*>(raw);std::string e;
  if(!self.current(e))return false;
  // Original Vox event and pitched paths call process libc rand. Do not seed
  // or substitute Random::s_seed, a copied combat stream or a private engine.
  out=std::rand();return out>=0;
 }
 bool emitter(const dh2::audio::AudioSoundV34&sound,const dh2::audio::AudioGroupV34&group,
  bool positioned,float&gain,float&pitch,int&native,std::string&e){
  using namespace dh2::audio;
  if(!current(e))return false;
  auto*runtime=audio.manager->runtime_on_producer();
  if(runtime->catalog().sound(sound.uid)!=&sound||runtime->catalog().group(group.uid)!=&group){e="Required SAME selected emitter XML rows";return false;}
  gain=original_fresh_emitter_gain_v40();pitch=original_fresh_emitter_pitch_v40();native=-1;
  // GetGroupUid is source table identity, not a substring/name heuristic.
  int pitched=-1,relative=-1;
  for(const auto&row:runtime->catalog().groups()){
   if(row.name=="sfx_3D_pitched")pitched=row.uid;
   if(row.name=="sfx_3D_pitched_relative")relative=row.uid;
  }
  if(positioned&&(group.uid==pitched||group.uid==relative)){
   const auto fx=settings->saved_option("VolumeFX");int draw{};
   if(!random(this,draw)){e="Required actual process Vox rand draw";return false;}
   const auto percentage=float(double(float(fx))*(double(draw%123)*.001+.96));
   if(!audio.manager->set_source_volume_v68(group.uid,percentage,e))return false;
  }
  if(sound.format==2&&!runtime->fresh_native_state_v68(sound.uid,native,e))return false;
  e.clear();return true;
 }
 bool plain(bool enabled,int group_gate,const dh2::audio::AudioSoundV34&sound,
  const dh2::audio::AudioGroupV34&group,dh2::audio::AudioCommandV34&command,std::string&e){
  using namespace dh2::audio;(void)enabled; // Source fp only used on platform route.
  float gain{},pitch{};int native{};if(!emitter(sound,group,false,gain,pitch,native,e))return false;
  OriginalVoxGeneralV40 general;bool completed{};
  if(!audio.manager->source_general_v68(general,completed,e)||!completed){if(e.empty())e="Required reached source Initialize general operation";return false;}
  auto fresh=original_fresh_emitter_fields_v40(general);fresh.relative=true;
  VoxListenerAuthorityV38 unused_listener; // Relative origin never reads camera basis.
  if(!vox_source_spatial_command_v38(unused_listener,fresh,gain,pitch,command,e))return false;
  command.native_state=native;command.source_dsp_bus=group.bus.c_str();
  command.volume_group=group_gate>=1&&group_gate<=30?group.volume_group:3;
  std::array<float,3>volumes{};if(!audio.manager->source_bus_volumes_v68(volumes,e))return false;
  for(unsigned bus=0;bus<volumes.size();++bus){AudioCommandV34 volume;volume.kind=AudioCommandKindV34::volume;
   volume.volume_group=int(bus);volume.left=volumes[bus];if(!audio.manager->runtime_on_producer()->mixer().post(volume)){e="Actual regular Play group command queue full";return false;}}
  e.clear();return true;
 }
};
}
bool borrow_campaign_audio_services_v68(const SourceCampaignCandidateBorrowV55&candidate,
 dh2::audio::AudioCampaignServicesV46&services,dh2::audio::AudioLevelBindingsV67&bindings,std::string&error){
 using namespace dh2::audio;
 SourceCampaignCandidateBorrowV55 actual;
 if(!borrow_source_campaign_candidate_v55(actual,error)||!candidate.actual_world||candidate.actual_world!=actual.actual_world||
    candidate.application!=actual.application||candidate.level!=actual.level){if(error.empty())error="Required SAME canonical audio campaign candidate";return false;}
 auto lease=std::make_shared<CampaignAudioSourceLeaseV68>();lease->world=actual.actual_world;lease->application=actual.application;
 if(!borrow_actual_application_audio_v42(lease->audio,error)||!lease->audio.manager||
    !borrow_actual_audio_settings_v68(actual.actual_world,lease->settings,error))return false;
 if(!lease->settings->has_option("VolumeFX")||!lease->settings->has_option("VolumeMusic")){error="Required actual initialized VolumeFX/VolumeMusic option descriptors";return false;}
 if(!prepare_campaign_audio_services_v46(services,error))return false;
 if(!lease->audio.manager->source_initialize_complete_v100()){error="Required whole process VoxSoundManager.Initialize at GSInit phase9 before campaign audio";return false;}
 services.settings_owner=lease->settings;services.rng_owner=lease;services.camera_owner=actual.camera_application;
 services.actual_vox_random={lease.get(),CampaignAudioSourceLeaseV68::random};
 services.actual_general=[lease](auto&out,bool&complete,std::string&e){return lease->current(e)&&lease->audio.manager->source_general_v68(out,complete,e);};
 services.actual_group_volumes=[lease](auto&out,std::string&e){return lease->current(e)&&lease->audio.manager->source_bus_volumes_v68(out,e);};
 services.actual_emitter_modifiers=[lease](const auto&,const auto&sound,const auto&group,float&gain,float&pitch,int&native,std::string&e){return lease->emitter(sound,group,true,gain,pitch,native,e);};
 // Source platform branch is JNI nativePlaySoundBig, not an ignored request.
 services.actual_platform_play=[](const auto&,std::string&e){e="Required original nativePlaySoundBig JNI backend for process platform flag";return false;};
 services.actual_network_muted=[](bool&,std::string&e){e="Required actual OnlineGameState network mute global9a4863";return false;};
 bindings.application_byte_b4=[lease](bool&out,std::string&e){if(!lease->current(e))return false;auto app=lease->application.lock();out=app->source_loading_v55().byte_b4!=0;e.clear();return true;};
 bindings.source_music_volume=[lease](float&out,std::string&e){return lease->current(e)&&lease->audio.manager->get_source_volume_v68(2,out,e);};
 bindings.plain_command=[lease](bool enabled,int gate,const auto&sound,const auto&group,auto&out,std::string&e){return lease->plain(enabled,gate,sound,group,out,e);};
 bindings.platform_play=[](int,bool,int,std::string&e){e="Required original nativePlaySoundBig JNI backend";return false;};
 bindings.platform_stop=[](int,std::string&e){e="Required original nativeStopSoundBig JNI backend";return false;};
 error.clear();return true;
}
