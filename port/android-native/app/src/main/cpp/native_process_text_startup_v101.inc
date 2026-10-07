//Include at native_app global scope after original_ui/front_ui declarations.
//Call after successful OriginalUiSession.initialize, before native startup
//settings/language or front movies. Does not construct a HUD or a World.
bool bind_process_text_startup_v101(dh2::android_ui::GSInitStageServicesV100& stages,std::string& error){
 dh2::ui::HudTextV1* text{};std::shared_ptr<void> owner;
 if(!original_ui.process_string_manager_borrow_v101(text,owner,error)||
    !front_ui.bind_process_text_v101(text,owner,error))return false;
 stages.string_manager=[text,owner](dh2::ui::HudTextV1*& out,std::shared_ptr<void>& pin,std::string& e){
  out=text;pin=owner;e.clear();return true;
 };
 stages.constants_load=[](const std::function<bool(const char*,std::string&)>& debug,bool& complete,std::string& e){
  return original_ui.load_process_constants_stage_v101(debug,complete,e);
 };
 stages.arrays_load=[](bool& complete,std::string& e){return original_ui.load_process_arrays_stage_v101(complete,e);};
 stages.settings_load=[](bool language_only,std::string& e){return front_ui.load_process_settings_v105(language_only,e);};
 stages.settings_loaded=[](bool& loaded,std::string& e){
  std::shared_ptr<dh2::ui::OwnedHudSettingsV1> actual;
  if(!front_ui.gameplay_settings_borrow_v67(actual,e))return false;
  loaded=actual->loaded();e.clear();return true;
 };
 error.clear();return true;
}
