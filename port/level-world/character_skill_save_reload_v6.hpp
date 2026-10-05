#pragma once
#include "../game-data/player_savegame_v1.hpp"
namespace dh2::character::skills {
struct SkillSaveReloadServicesV6 {
 void* context;
 // Return the actual selected Character list from the SAME property/design
 // owner. No old SavedSkill view may survive the preceding native deletion.
 int(*selected_list)(void*,data::PlayerSavegameV1*,std::uintptr_t,const std::vector<std::int32_t>**);
 // Complete original Save.Load(mask8) on this SAME save, including actual
 // section/file ownership. A missing/rejected delivery preserves native prefix.
 int(*load)(void*,data::PlayerSavegameV1*,std::uint32_t mask);
};
struct SkillSaveReloadOutputV6 {std::uint32_t phase,old_count,new_count,deleted;std::int32_t status;std::uint32_t reserved;};
extern "C" int dh2_character_skill_save_reload_v6(SkillSaveReloadOutputV6*,data::PlayerSavegameV1*,const SkillSaveReloadServicesV6*);
}
