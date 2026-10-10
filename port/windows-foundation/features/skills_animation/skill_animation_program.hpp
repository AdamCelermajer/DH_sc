#pragma once
#include "../../original_attack_sequence.hpp"
#include "../../../game-data/animation_tables.hpp"
namespace dh::foundation::skills_animation {
struct SkillAnimationPrograms {
 OriginalCombatVisualPlan plan;
 OriginalSequencePolicies policies;
 // Exact source step producers retained for FX/camera/sound/swoosh providers.
 std::map<std::pair<std::string,std::vector<std::size_t>>,dh2::data::AnimationStep> steps;
};
// Caller supplies exact resolved AnimTable root IDs (including actual stance),
// SAME visual config and role. All authored redirect/loop/type metadata survives.
// Adds clips to config for loading once BEFORE Session takes pose ownership.
bool build_skill_animation_programs(const AssetCatalog&,const dh2::data::AnimationTables&,
 const dh2::data::Dictionary&,const CharacterVisualConfig& same_visual,
 const std::vector<std::int32_t>& sequence_ids,const std::string& role,
 SkillAnimationPrograms&,std::string&);
std::string skill_sequence_state(std::int32_t sequence_id);
}
