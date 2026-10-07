#pragma once
#include "character_panel_reload_v3.hpp"
#include "character_menu_recalc_owner_v1.hpp"
#include "player_manager_combat_runtime_v2.hpp"
#include "player_save_load_owner_v1.hpp"
namespace dh2::android_ui {
struct CharacterPanelMovieRuntimeV3 {
 std::shared_ptr<void> owner;
 std::function<bool(double,std::int32_t&,std::string&)> player_index;
 std::function<bool(std::uintptr_t&,std::string&)> current_menu_fx;
 std::function<bool(std::uintptr_t,const char*,const char*,bool,std::string&)> invoke;
};
// ONE Save+8 profile authority is passed in, not constructed per menu call.
// Startup main selection and this panel must publish through the same owner.
class CharacterPanelRuntimeV3 {
 std::shared_ptr<void> world_;
 character::skills::CharacterPlayerSkillsV6& skills_;
 player::PlayerEquipmentRenderOwnerV1& gear_;
 player::PlayerManagerOwnerV1& manager_;
 data::PlayerSaveLoadOwnerV1& saved_;
 character::CharacterMenuRecalcOwnerV1 recalc_;
 CharacterPanelMovieRuntimeV3 movie_;
public:
 CharacterPanelRuntimeV3(std::shared_ptr<void>,character::skills::CharacterPlayerSkillsV6&,
  player::PlayerEquipmentRenderOwnerV1&,player::PlayerManagerOwnerV1&,
  data::PlayerSaveLoadOwnerV1&,character::CharacterGameDesign::Borrow&&,
  CharacterPanelMovieRuntimeV3);
 bool bind(const model_renderer::PlayerGameplayBinding&,
           ui::CharacterMenuReloadActionGraphV1&,std::string&);
 bool source(const model_renderer::PlayerGameplayBinding&,
             const ui::MenuReloadRequest32V1&,ui::MenuReloadResponse16V1&,std::string&);
};
}
