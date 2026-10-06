namespace {
struct RendererCharacterApplicationV4 {
 std::shared_ptr<WorldScriptContext> world;
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> source_application;
 dh2::ui::HudTextV1* text;
 std::shared_ptr<void> text_lease;
 dh2::ui::SettingsNativeFilesV1 native_files;
 dh2::ui::SettingsFileServicesV1 files;
 dh2::ui::SettingsLanguageServicesV1 language;
 dh2::ui::SettingsDeviceFactsV1 device;
 dh2::ui::SettingsStartupBindingV1 binding;
 dh2::ui::HudStartupState48 state{};
 std::weak_ptr<dh2::ui::AuthoredCharacterApplicationV1> application;
 explicit RendererCharacterApplicationV4(std::shared_ptr<WorldScriptContext> w,
  std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> actual_application,
  dh2::ui::HudTextV1& actual_text,std::shared_ptr<void> actual_text_lease,
  const dh2::ui::MenuDeviceFactsV1& actual_device):world(std::move(w)),source_application(std::move(actual_application)),text(&actual_text),
  text_lease(std::move(actual_text_lease)),native_files(world->files_directory),files(native_files.services()){
  language.context=this;language.refresh_scene=refresh_scene;
  language.text_context=this;language.switch_text_pack_v4=switch_pack;
  device.sharp_devices=actual_device.sharp;device.htc_devices=actual_device.htc;
  device.no_igp=actual_device.no_igp;device.in_multiplayer_mode=actual_device.multiplayer_mode;
  device.capabilities=actual_device.driver_type;
  // This original NativeLoadSettings wrapper only calls load(false). Its
  // Korean/Japanese/platform-language-only branches are not delivered here.
  binding.owner=world->saved_options.get();binding.files=&files;binding.language=&language;binding.device=&device;
  binding.application_identity=source_application->identity();
  binding.savegame_identity=reinterpret_cast<std::uintptr_t>(world->saved_options.get());
  state.application=binding.application_identity;state.savegame=binding.savegame_identity;
  // Native Application currently retains no complete audio settings receiver.
  // This is its genuine absent sound field, not a successful volume provider.
  state.sound=0;state.sharp_devices=device.sharp_devices;state.htc_devices=device.htc_devices;state.no_igp=device.no_igp;
 }
 static bool switch_pack(void* p,std::int32_t pack,bool reload,std::string& error){
  auto& self=*static_cast<RendererCharacterApplicationV4*>(p);
  if(!self.text||!self.text_lease){error="Required same retained HUD TextManager";return false;}
  return self.text->switch_pack(pack,reload,error);
 }
 static bool refresh_scene(void* p,dh2::ui::OwnedHudSettingsV1& owner,std::int32_t,std::string& error){
  auto& self=*static_cast<RendererCharacterApplicationV4*>(p);
  if(!world_mode||live_script_world!=self.world||!self.world->canonical_world||
     !player_skills_runtime||player_skills_runtime->world!=self.world||!player_skills_runtime->targets||
     self.world->saved_options.get()!=&owner){error="Required active same World settings/language/Character owners";return false;}
  auto& runtime=*player_skills_runtime;
  auto query=runtime.targets->targets().query_services();
  dh2::character::CharacterLanguageSceneServicesV1 services;
  services.owner=self.world;services.scene=&self.world->canonical_world->language_registry.scene();
  services.actors=runtime.targets.get();services.ai_types=&runtime.targets->targets().source_ai_types_borrow_v4();services.target_services=&query;
  services.gear=[&self](std::uintptr_t id,dh2::player::PlayerEquipmentRenderOwnerV1*& gear,std::string& e){
   gear=nullptr;if(!self.world->player_object){e="Required same World source player";return false;}
   if(id==self.world->player_object->identity){
    if(!player_equipment||!player_equipment->ready()){e="Required actual player Gear language owner";return false;}
    gear=player_equipment.get();
   }
   return true;
  };
  services.other_inventory=[&runtime](std::uintptr_t id,std::string& e){
   const auto i=runtime.attack_npc_inventories.find(id);
   if(i==runtime.attack_npc_inventories.end()||!i->second){e="Required same NPC owned inventory language receiver";return false;}
   if(!i->second->inventory().items().empty()){e="Required actual populated NPC ItemText/powered-localization owner";return false;}
   return true; // Genuine original zero-iteration inventory, not an empty substitute.
  };
  services.is_game_object=[&self](std::uintptr_t id,bool& value,std::string& e){
   if(id==self.world->player_object->identity){value=true;return true;}
   if(!player_skills_runtime){e="Required same canonical language object dispatch";return false;}
   const auto found=player_skills_runtime->world_actors.find(id);
   if(found!=player_skills_runtime->world_actors.end()&&found->second.npc){value=true;return true;}
   e="Required actual canonical class IsGameObject language receiver";return false;
  };
  services.item_object=[](std::uintptr_t,std::string& e){e="Required actual same canonical Item localization receiver";return false;};
  // Player initial source inventory has no powers. The complete existing
  // UpdateLocalization kernel preserves name/stats/requirements prefixes and
  // reports its actual missing PowerInfo owner if a powered item is reached.
  dh2::character::CharacterLanguageSceneConnectionV1 connection(std::move(services));
  return connection.refresh(error);
 }
 static int startup(void* p,dh2::ui::HudStartupState48* state,
  const dh2::ui::HudStartupRequest40* request,dh2::ui::HudStartupResponse16* response){
  auto& self=*static_cast<RendererCharacterApplicationV4*>(p);
  const int status=dh2::ui::settings_startup_v1_service(&self.binding,state,request,response);
  if(status)__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Character settings source required | %s",self.binding.error.c_str());
  return status;
 }
};
}
bool borrow_character_menu_application_v4(dh2::ui::HudTextV1& text,std::shared_ptr<void> text_lease,
 std::shared_ptr<dh2::ui::AuthoredCharacterApplicationV1>& out,std::string& error){
 out.reset();if(!world_mode||!live_script_world||!live_script_world->saved_options||
    !live_script_world->canonical_world||!text_lease){error="Required actual live character Application/settings/text owners";return false;}
 auto& world=live_script_world;
 if(auto existing=world->character_application.lock())if(existing->text==&text){
  if(auto application=existing->application.lock()){out=std::move(application);return true;}
 }
 dh2::ui::MenuDeviceFactsV1 device;if(!borrow_actual_menu_device_v1(device,error))return false;
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> actual_application;
 if(!borrow_actual_application_services_v5(actual_application,error))return false;
 auto owner=std::make_shared<RendererCharacterApplicationV4>(world,std::move(actual_application),text,std::move(text_lease),device);
 out=std::make_shared<dh2::ui::AuthoredCharacterApplicationV1>(owner,owner->state,dh2::ui::HudStartupServices16{owner.get(),RendererCharacterApplicationV4::startup});
 owner->application=out;world->character_application=owner;return true;
}
