#pragma once
#include "character_current_spell_v1.hpp"
namespace dh2::character::skills {
// Same source Character/save/query descriptor. Operation4 delivers the actual
// GetCharFaery(id) row's signed word+8 after its complete required validation.
constexpr std::uint32_t current_spell_faery_element_v3=4;
int equipped_faery_element_v3(void*,const dh2_script_value*,std::uint32_t,
 dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
int equipped_faery_level_v3(void*,const dh2_script_value*,std::uint32_t,
 dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
}
extern "C" int dh2_character_faery_element_v3(std::int32_t*,std::uintptr_t,
 const dh2::character::skills::CurrentSpellServices16V1*);
extern "C" int dh2_character_faery_level_v3(std::int32_t*,std::uintptr_t,
 const dh2::character::skills::CurrentSpellServices16V1*);
