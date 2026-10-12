#pragma once
#include "character_target_providers.hpp"
namespace dh2::character::skills {
// The source cached sheet is borrowed as its actual int32 array. This avoids
// creating a CombatProperties896 copy or type-punning the live PropertyState.
struct SkillTargetCharacterV6 {
 std::uintptr_t identity;const std::int32_t* resolved;const char* name;
 std::uint32_t flags520;std::uint8_t dead1449,disabled81,visible8a,interactive415;
};
static_assert(sizeof(SkillTargetCharacterV6)==32);
extern "C" int dh2_character_skill_target_query_v6(std::int32_t*,std::uint32_t,
 SkillTargetCharacterV6*,SkillTargetCharacterV6*,const target_providers::Types16*,const target_providers::Services16*);
// Same kernel with a live State.flags borrow exactly when source reaches +520.
extern "C" int dh2_character_skill_target_query_live_flags_v6(std::int32_t*,std::uint32_t,
 SkillTargetCharacterV6*,SkillTargetCharacterV6*,const target_providers::Types16*,const target_providers::Services16*);
}
