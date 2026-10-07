// Current Application/UI resources lent to canonical player bindings. These
// callbacks keep the actual text/settings owners and validate World identity;
// no development settings map or standalone no-player service is substituted.
namespace {
struct NativeGameplayTextBorrowV67 {
 std::weak_ptr<void> world;
 std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 std::shared_ptr<dh2::ui::OwnedHudSettingsV1> settings;
 dh2::ui::HudTextEnvironmentV1 actual;
 std::shared_ptr<void> text_owner;
 static bool open(void* raw,const char* name,bool& found,std::vector<std::uint8_t>& bytes,std::uintptr_t& lease,std::string& e){
  auto& s=*static_cast<NativeGameplayTextBorrowV67*>(raw);auto& p=s.actual.localization;
  if(!p.open){e="Required actual StringManager file-open provider";return false;}
  return p.open(p.context,name,found,bytes,lease,e);
 }
 static bool close(void* raw,std::uintptr_t lease,std::string& e){
  auto& s=*static_cast<NativeGameplayTextBorrowV67*>(raw);auto& p=s.actual.localization;
  if(!p.close){e="Required actual StringManager file-close provider";return false;}
  return p.close(p.context,lease,e);
 }
 static bool debug(void* raw,const char* name,std::string& e){
  auto& s=*static_cast<NativeGameplayTextBorrowV67*>(raw);auto& p=s.actual.localization;
  if(!p.debug){e="Required actual StringManager Debug provider";return false;}
  return p.debug(p.context,name,e);
 }
 static bool constant(void* raw,const char* group,const char* key,std::uint32_t& value,std::string& e){
  auto& s=*static_cast<NativeGameplayTextBorrowV67*>(raw);auto& p=s.actual.localization;
  if(!p.constant){e="Required actual StringManager constants provider";return false;}
  return p.constant(p.context,group,key,value,e);
 }
 static bool player(void* raw,std::uintptr_t& out,std::string& e){
  out=0;auto& s=*static_cast<NativeGameplayTextBorrowV67*>(raw);
  auto world=s.world.lock();auto app=s.application.lock();model_renderer::SourceCampaignCandidateBorrowV55 current;
  if(!world||!app||!model_renderer::borrow_source_campaign_candidate_v55(current,e)||
     current.actual_world!=world||current.application!=app){
   if(e.empty())e="Gameplay localization no longer belongs to same actual World/Application";return false;
  }
  auto manager=app->source_player_manager_v59();dh2::player::PlayerInfoFieldsV1* local{};
  if(!manager||!manager->get_local_player(0,true,local,e)){if(e.empty())e="Required actual localization PlayerManager";return false;}
  out=local?local->character660:0;e.clear();return true;
 }
 static bool name(void* raw,std::uintptr_t character,std::string& text,std::string& e){
  auto& s=*static_cast<NativeGameplayTextBorrowV67*>(raw);
  return model_renderer::source_character_saved_name_v67(s.world.lock(),character,text,e);
 }
 static bool language(void* raw,std::int32_t& value,std::string& e){
  auto& s=*static_cast<NativeGameplayTextBorrowV67*>(raw);
  if(!s.settings){e="Required actual gameplay settings language owner";return false;}
  value=s.settings->language();e.clear();return true;
 }
};
}
bool model_renderer::borrow_actual_gameplay_presentation_v67(const std::shared_ptr<void>& world,
 std::shared_ptr<dh2::ui::OwnedHudSettingsV1>& settings,dh2::ui::HudTextEnvironmentV1& environment,
 std::shared_ptr<void>& text_owner,std::string& error){
 settings.reset();environment={};text_owner.reset();std::shared_ptr<void> manager;
 if(!borrow_native_menu_manager_v58(world,manager,error)||!front_ui.gameplay_settings_borrow_v67(settings,error))return false;
 auto borrow=std::make_shared<NativeGameplayTextBorrowV67>();borrow->world=world;
 borrow->application=application_services_v5;borrow->settings=settings;
 if(!original_ui.gameplay_text_environment_v67(borrow->actual,borrow->text_owner,error))return false;
 environment=borrow->actual;
 environment.localization={borrow.get(),NativeGameplayTextBorrowV67::open,NativeGameplayTextBorrowV67::close,
  NativeGameplayTextBorrowV67::debug,NativeGameplayTextBorrowV67::constant,NativeGameplayTextBorrowV67::player,
  NativeGameplayTextBorrowV67::name,NativeGameplayTextBorrowV67::language,nullptr};
 text_owner=std::move(borrow);error.clear();return true;
}
bool model_renderer::borrow_actual_audio_settings_v68(const std::shared_ptr<void>& world,
 std::shared_ptr<dh2::ui::OwnedHudSettingsV1>& settings,std::string&error){
 std::shared_ptr<void> actual_menu_manager;
 if(!borrow_native_menu_manager_v58(world,actual_menu_manager,error))return false;
 return front_ui.gameplay_settings_borrow_v67(settings,error);
}
