#pragma once
#include "character_player_skills_v6.hpp"
#include "character_skill_save_load_connection_v1.hpp"
#include "player_equipment_render_owner_v1.hpp"
#include "../engine-ui/character_menu_reload_v1.hpp"
namespace dh2::character::skills {
struct CharacterMenuLiveReloadServicesV1 {
 std::shared_ptr<void> owner;
 // Original CharProperties::Recalc(1), on the SAME PropertyView including
 // live Buff groups, saved sheet and gear sheet. No raw PropertyState clone.
 std::function<bool(data::PropertyView&,std::uint32_t,std::string&)> recalculate;
 // Original Character::CheckItems3a9d10, not _RefreshEffects substitution.
 std::function<bool(player::PlayerEquipmentRenderOwnerV1&,std::string&)> check_items;
 // Actual current MultiMenuManager RenderFX, reread after earlier callbacks.
 std::function<bool(std::uintptr_t&,std::string&)> menu_fx;
 // Original RenderFX Invoke(path,method, one boolean argument). Missing AS
 // method semantics must be preserved by the actual movie invoke owner.
 std::function<bool(std::uintptr_t,const char*,const char*,bool,std::string&)> invoke;
};
class CharacterMenuLiveReloadConnectionV1 {
 std::uintptr_t character_;
 CharacterPlayerSkillsV6& skills_;
 CharacterSkillSaveLoadConnectionV1& saved_;
 data::PlayerSavegameV1& save_;
 player::PlayerEquipmentRenderOwnerV1& gear_;
 data::PropertyView& properties_;
 CharacterMenuLiveReloadServicesV1 services_;
 std::string error_;
 static int deliver(void*,const ui::MenuReloadRequest32V1*,ui::MenuReloadResponse16V1*);
public:
 CharacterMenuLiveReloadConnectionV1(std::uintptr_t,CharacterPlayerSkillsV6&,
  CharacterSkillSaveLoadConnectionV1&,data::PlayerSavegameV1&,
  player::PlayerEquipmentRenderOwnerV1&,data::PropertyView&,
  CharacterMenuLiveReloadServicesV1);
 ui::MenuReloadServices16V1 services()noexcept{return {this,deliver};}
 int reload(ui::MenuReloadResult16V1&);
 const std::string& error()const noexcept{return error_;}
};
}
