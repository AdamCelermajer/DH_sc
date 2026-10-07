#pragma once
#include "actor_blended_playback.hpp"
namespace dh2::data {struct CombatResult;}
namespace model_renderer {
// Same canonical record is the callback context, retained by its visual.
bool source_campaign_animation_step_v100(void*,dh2::actor::BlendedPlayback&,
 const dh2::data::AnimationStep&,std::string&);
// Genuine SkillApply/DOT sound leaf, after its target life/damage stores.
bool source_campaign_combat_sound_v112(const std::shared_ptr<void>& actual_world,
 std::uintptr_t attacker,std::uintptr_t target,bool character_attacker,
 const dh2::data::CombatResult&,std::string&);
}
