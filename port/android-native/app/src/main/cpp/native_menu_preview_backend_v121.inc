// Retained process UI loans complete the renderer backend. No World is needed
// to borrow the original StringManager, Front save-slot storage or SoundManager.
bool model_renderer::borrow_native_menu_preview_text_v121(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 dh2::ui::HudTextV1*& text,dh2::ui::HudTextEnvironmentV1& environment,
 std::shared_ptr<void>& owner,std::string& e){
 text=nullptr;environment={};owner.reset();
 if(!app||app!=application_services_v5){e="Different process Application in preview text loan";return false;}
 struct Pin {std::shared_ptr<void> text,environment;};auto pin=std::make_shared<Pin>();
 if(!original_ui.process_string_manager_borrow_v101(text,pin->text,e)||
    !original_ui.gameplay_text_environment_v67(environment,pin->environment,e))return false;
 owner=pin;e.clear();return true;
}
bool model_renderer::build_native_menu_preview_services_v121(
 const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 NativeMenuPreviewServicesV121& out,std::string& e){
 out={};if(!app||app!=application_services_v5){e="Different process Application in preview renderer loan";return false;}
 if(!build_native_menu_preview_renderer_services_v121(app,gameplay_assets,front_directory,out,e)||
    !front_ui.borrow_menu_avatar_state_v121(out.avatar_state,out.avatar_state_owner,e))return false;
 const std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> weak=app;
 out.save_exists=[weak](auto slot,auto& exists,auto& e){auto same=weak.lock();
  if(!same||same!=application_services_v5){e="Retired actual preview save-file receiver";return false;}
  return front_ui.menu_save_exists_v121(slot,exists,e);
 };
 out.stop_music=[weak](auto fade,auto& e){auto same=weak.lock();
  if(!same||same!=application_services_v5){e="Retired actual preview SoundManager receiver";return false;}
  dh2::audio::AudioApplicationBorrowV42 audio;
  if(!dh2::android_audio::borrow_application_audio_v42(audio,e)||!audio.manager||!audio.manager->source_producer_ready_v94(e))return false;
  auto& fields=audio.manager->music_fields_on_producer();
  if(fields.current_music_24==-1){e.clear();return true;} //Original StopMusic36bec0 NULL-track branch.
  const auto current=fields.current_music_24;
  if(!audio.manager->stop_sound_v106(current,fade,[](auto,auto& e){e="Required actual nativeStopSoundBig platform endpoint";return false;},e))return false;
  fields.current_music_24=-1;fields.field_28=current;e.clear();return true;
 };
 return true;
}
