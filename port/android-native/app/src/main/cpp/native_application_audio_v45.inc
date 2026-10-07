// Native Application audio leaves only; included at native_app global scope.
bool model_renderer::borrow_actual_application_audio_v42(dh2::audio::AudioApplicationBorrowV42&out,std::string&error){
 return dh2::android_audio::borrow_application_audio_v42(out,error);
}
bool model_renderer::initialize_process_audio_v100(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>&application,
 dh2::audio::AudioInitializeServicesV100 services,std::string&error){
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> actual;
 if(!model_renderer::borrow_actual_application_services_v5(actual,error)||!application||actual!=application){
  if(error.empty())error="Required SAME actual GSInit Application for SoundManager.Initialize";return false;
 }
 if(services.actual_application&&services.actual_application!=application){error="Foreign process audio Initialize Application";return false;}
 services.actual_application=application;
 if(!services.saved_option){std::shared_ptr<dh2::ui::OwnedHudSettingsV1> settings;
  if(!front_ui.gameplay_settings_borrow_v67(settings,error))return false;
  services.saved_option=[settings](const char*key,std::int32_t&out,std::string&e){
   if(!key){e="Required original process saved-option name";return false;}
   out=settings->saved_option(key);e.clear();return true;
  };
 }
 if(!services.high_performance)services.high_performance=model_renderer::borrow_actual_device_high_performance_v54;
 auto flag=[](unsigned selector,bool&out,std::string&e){dh2::ui::MenuDeviceFactsV1 facts;
  if(!model_renderer::borrow_actual_menu_device_v1(facts,e))return false;
  out=(selector==0?facts.htc:selector==1?facts.sharp:facts.multiplayer_mode)!=0;e.clear();return true;
 };
 if(!services.htc_devices)services.htc_devices=[flag](bool&out,std::string&e){return flag(0,out,e);};
 if(!services.sharp_devices)services.sharp_devices=[flag](bool&out,std::string&e){return flag(1,out,e);};
 if(!services.multiplayer_mode)services.multiplayer_mode=[flag](bool&out,std::string&e){return flag(2,out,e);};
 // Debug.load/GetSwitch remains supplied by the actual process Debug owner;
 // no campaign World or desired preload boolean is substituted at phase9.
 dh2::audio::AudioApplicationBorrowV42 audio;
 if(!model_renderer::borrow_actual_application_audio_v42(audio,error)||!audio.manager){if(error.empty())error="Required actual GSInit SoundManager global";return false;}
 return audio.manager->initialize_source_v100(services,error);
}
extern "C" JNIEXPORT jlong JNICALL Java_com_example_dh2_NativeBridge_audioApplicationReserveV42(JNIEnv*,jclass){
 return static_cast<jlong>(dh2::android_audio::reserve_application_audio_v42());
}
extern "C" JNIEXPORT jlong JNICALL Java_com_example_dh2_NativeBridge_audioApplicationOwnerV42(JNIEnv*,jclass){
 return static_cast<jlong>(dh2::android_audio::application_audio_owner_v42());
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_audioApplicationCloseV42(JNIEnv*,jclass,jlong owner){
 if(owner>0)dh2::android_audio::request_application_audio_close_v42(static_cast<std::uint64_t>(owner));
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_NativeBridge_audioApplicationShutdownV42(JNIEnv*env,jclass,jlong owner){
 std::string error;
 return dh2::android_audio::shutdown_application_audio_v42(owner>0?static_cast<std::uint64_t>(owner):0,error)?nullptr:result(env,error);
}
