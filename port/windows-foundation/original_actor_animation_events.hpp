#pragma once
#include "retained_animation_owner.hpp"
#include "../level-world/character_ai_events.hpp"
#include "original_controller_commands.hpp"

namespace dh::foundation {
enum class OriginalNamedAnimationKind { ignored, relay, object_animation, mesh_fx, sound_fx,
    mainhand, offhand, projectile, skill, spell, interaction };
OriginalNamedAnimationKind classify_original_named_animation_event(const std::string&,
    std::int32_t original_state,bool can_range) noexcept;
// Every authored name is event28, retaining lag/name payload. The exact source
// dispatcher routes it BEFORE global/controller gates. Services supply actual
// AIS_ANIM_EVENT helper and state consumer; this function introduces no FSM.
bool route_original_named_animation_event(dh2::character::AIEventState64&,
    const RetainedAnimationEvent&,const dh2::character::AIEventServices24&,std::string& error);
} // namespace dh::foundation
