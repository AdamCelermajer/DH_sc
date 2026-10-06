// Root integration: include after PlayerSkillsRuntime, all function definitions
// and equipment_platform declarations. Add runtime members in destruction order:
// unique_ptr<PlayerSaveLoadOwnerV1> player_save_load;
// unique_ptr<CharacterPanelRuntimeV3> character_panel_runtime;
// (panel declared after Save so it dies first). Header dependencies below.
// This creates the sole source profile owner around the SAME existing Save.
// Do not call if canonical startup has already published a SaveLoadOwner;
// reuse that owner instead. Default development Save slot-1/profile-null takes
// genuine Load(0x20) null-profile branch. A real slot/profile requires real
// filename/constructor/PROP services through saved_services.
static void bind_player_character_panel_v3(PlayerSkillsRuntime& t,
 dh2::data::PlayerSaveLoadServicesV1 saved_services,
 dh2::android_ui::CharacterPanelMovieRuntimeV3 movie){
 if(!t.player||!t.player_manager||!player_equipment||!t.world)
  throw std::runtime_error("Required same player manager/Gear/world for Character panel");
 if(!t.player_save_load)t.player_save_load=std::make_unique<dh2::data::PlayerSaveLoadOwnerV1>(
  t.player->native_saved_owner(),std::move(saved_services));
 t.character_panel_runtime=std::make_unique<dh2::android_ui::CharacterPanelRuntimeV3>(
  t.world,*t.player,*player_equipment,t.player_manager->manager(),*t.player_save_load,
  t.world->design->borrow(),std::move(movie));
}
// Root export must retrieve current PlayerSkillsRuntime (do not store a copied
// PlayerGameplayBinding). Supply CharacterPanelGameplayServicesV2.reload:
// [](const auto& p,auto& graph,auto& error) {
//   auto* t=current retained runtime;
//   return t&&t->character_panel_runtime&&t->character_panel_runtime->bind(p,graph,error);
// }
// Existing session action/query owners already perform actual train/equip/stat
// mutations. This supplement wires the missing whole reload, not a second panel.
