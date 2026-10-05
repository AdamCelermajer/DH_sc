#pragma once
#include "character_skill_target_queries_v6.hpp"
#include "character_player_skills_v6.hpp"
namespace dh2::character {
// Character owns inline CharAI at+3c8. Its ctor byte+4d is Character+415.
// The canonical World target record is the sole backing for that actual field.
class CharacterTargetabilityOwnerV1 {
 skills::SkillTargetCharacterV6* actor_{};
public:
 explicit CharacterTargetabilityOwnerV1(skills::SkillTargetCharacterV6& actor):actor_(&actor){}
 bool valid()const noexcept{return actor_&&actor_->identity&&actor_->resolved;}
 // Call once during actual Character construction, never each refresh/frame.
 int construct();
 std::uint8_t* source_byte415()const noexcept{return valid()?&actor_->interactive415:nullptr;}
 // Complete source _SetIsTargetable argument gates. Original caller supplies
 // actual argument count/type and Value::getBool result, no fabricated default.
 int set_is_targetable(std::uint32_t count,std::uint32_t first_lua_type,bool first_boolean);
 // Calls the same retained player's existing complete native CancelSneaking.
 // It preserves failed-owner diagnostics and all reached buff/script effects.
 int cancel_sneaking(skills::CharacterPlayerSkillsV6& same_player);
};
}
