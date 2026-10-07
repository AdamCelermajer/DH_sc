// Process Application receiver for the original character callbacks. The
// actual App4c/settings/audio/device owners survive graphical/menu binding.
struct NativeCharacterApplicationV109 {
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> app;
 std::shared_ptr<dh2::ui::OwnedHudSettingsV1> settings;
 dh2::audio::AudioApplicationBorrowV42 audio;
 dh2::ui::MenuDeviceFactsV1 device;
 dh2::ui::HudStartupState48 state{};std::string error;
 static int invoke(void* raw,dh2::ui::HudStartupState48* state,const dh2::ui::HudStartupRequest40* q,dh2::ui::HudStartupResponse16* out){
  auto& s=*static_cast<NativeCharacterApplicationV109*>(raw);auto app=s.app.lock();
  if(!app||state!=&s.state||!q||!out||q->reserved||app->source_settings4c_v67()!=s.settings)return 1;
  *out={};s.error.clear();using O=dh2::ui::HudStartupOperation;
  switch(q->operation){
  case O::load_settings:return q->subject==s.state.savegame&&q->argument==0&&front_ui.load_process_settings_v105(false,s.error)?0:1;
  case O::update_saved_values:(void)s.settings->saved_option("AutoOrientation");return 0; // Android ResetOrientation literal bx lr.
  case O::get_saved_option:if(q->subject!=app->identity()||!q->name)return 1;out->value=s.settings->saved_option(q->name);return 0;
  case O::get_language:if(q->subject!=s.state.savegame)return 1;out->value=s.settings->language();return 0;
  case O::set_language:return q->subject==s.state.savegame&&front_ui.set_process_language_v109(q->argument,s.error)?0:1;
  case O::is_high_performance:{dh2::ui::HudDevicePipeline16 facts{{s.device.sharp,s.device.htc,s.device.multiplayer_mode},s.device.driver_type};out->value=dh2_hud_device_pipeline(&facts);return out->value<0?1:0;}
  case O::set_initial_volume:
   if(!s.audio.manager||q->subject!=s.audio.identity())return 1;
   return s.audio.manager->set_source_volume_v68(1,q->values[1],s.error)&&s.audio.manager->set_source_volume_v68(2,q->values[0],s.error)?0:1;
  default:return 1;
  }
 }
 static bool create(std::shared_ptr<NativeCharacterApplicationV109>& owner,
                    std::shared_ptr<dh2::ui::AuthoredCharacterApplicationV1>& application,std::string& e){
  std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> app;
  if(!model_renderer::borrow_actual_application_services_v5(app,e))return false;
  if(owner){if(owner->app.lock()!=app||app->source_settings4c_v67()!=owner->settings){e="Character process Application receiver replaced";return false;}return bool(application);}
  auto s=std::make_shared<NativeCharacterApplicationV109>();s->app=app;
  if(!front_ui.gameplay_settings_borrow_v67(s->settings,e)||app->source_settings4c_v67()!=s->settings||
     !front_ui.menu_device_borrow_v4(s->device,e)||!model_renderer::borrow_actual_application_audio_v42(s->audio,e))return false;
  s->state.application=app->identity();s->state.savegame=reinterpret_cast<std::uintptr_t>(s->settings.get());s->state.sound=s->audio.identity();
  s->state.sharp_devices=s->device.sharp;s->state.htc_devices=s->device.htc;s->state.no_igp=s->device.no_igp;
  application=std::make_shared<dh2::ui::AuthoredCharacterApplicationV1>(s,s->state,dh2::ui::HudStartupServices16{s.get(),invoke});owner=std::move(s);e.clear();return true;
 }
};
