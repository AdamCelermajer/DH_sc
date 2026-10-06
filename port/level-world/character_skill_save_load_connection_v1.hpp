#pragma once
#include "character_skill_save_reload_v6.hpp"
#include "../game-data/player_save_load_owner_v1.hpp"
#include <functional>
namespace dh2::character::skills {
// The LoadOwner owns the SAME Save profile field. This adapter never creates
// a second Save, inserts a profile, or interprets slot -1 as file-not-found.
class CharacterSkillSaveLoadConnectionV1 {
 data::PlayerSaveLoadOwnerV1& load_;
 std::shared_ptr<void> selected_owner_;
 std::function<bool(std::uintptr_t,const std::vector<std::int32_t>*&,std::string&)> selected_;
 std::string error_;
 static int selected(void*,data::PlayerSavegameV1*,std::uintptr_t,const std::vector<std::int32_t>**);
 static int load(void*,data::PlayerSavegameV1*,std::uint32_t);
public:
 CharacterSkillSaveLoadConnectionV1(data::PlayerSaveLoadOwnerV1&,
  std::shared_ptr<void>,decltype(selected_));
 SkillSaveReloadServicesV6 services() noexcept;
 // Also supplies the outer SG_Load(mask0x20) on the same authority.
 bool load_mask(std::uint32_t,std::string&);
 const std::string& error()const noexcept{return error_;}
};
}
