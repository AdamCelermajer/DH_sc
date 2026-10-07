#pragma once
#include "character_skills.hpp"
#include "../game-data/player_savegame_v1.hpp"
#include "../script-runtime/script_runtime.h"
namespace dh2::character::skills {
struct CurrentSkillView32V2 {const List16* lists;std::uint32_t list_count,skill_count;std::int32_t selected_list;std::uint32_t reserved;const data::SavedSkillsView16V1* saved;};
static_assert(sizeof(CurrentSkillView32V2)==32);
struct CurrentSkillBindingsV2 {
 data::PropertyView* properties{};const data::SkillTables::Borrow* tables{};
 const data::PlayerSavegameV1* saved{};
 void* context{};
 // Required original Value::getNumber conversion for string/native identity.
 // Caller retains conversion providers; no pointer truncation/default numeric.
 int(*number)(void*,const dh2_script_value*,float*){};
};
int current_skill_info_v2(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
}
// Bounded source numeric-index path of _GetCurrentSkillInfo3b8f9c, source
// GetCharSkillListId fallback3, genuine table row validation and saved u16level.
// 1 emits one integer,0 missing arg,-1 malformed,-2 unsafe original index.
extern "C" int dh2_character_current_skill_level_v2(std::int32_t*,
 const dh2::character::skills::CurrentSkillView32V2*,float,std::uint32_t present);
