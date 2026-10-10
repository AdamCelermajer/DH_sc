#pragma once
#include "../level-world/character_controller_commands.hpp"
#include "../level-world/character_ai_attack.hpp"
#include "../level-world/character_animation_events.hpp"
#include <string>

namespace dh::foundation {
enum class OriginalCommandAdmission { blocked, admitted, failed };
struct OriginalCommandResult {
    OriginalCommandAdmission admission = OriginalCommandAdmission::failed;
    int source_status = -1;
};
// These APIs report admission, not FSM acceptance or a gameplay state change.
// Flags/controller/owner are the caller's fresh LIVE source projections.
OriginalCommandResult original_controller_object_command(
    const dh2::character::ControllerCommandState32&, dh2::character::ControllerCommand,
    std::uintptr_t target, const dh2::character::CharacterControlServices16&,
    std::string& error);
// NPC source point MoveTo: gate -> remote query -> PathTo(point). No invented
// state reset, target clearing or remote/Stop behavior.
OriginalCommandResult original_controller_move_point(
    const dh2::character::ControllerCommandState32&,const float point[3],
    const dh2::character::CharacterControlServices16&,std::string& error);
struct OriginalHeadingBodyServices {
    void* context = nullptr;
    // Entire source Character::Ctrl_HeadTowards body; return1 complete,-1 missing.
    // Must preserve zero-vector-before-skill checks, target facing/clear and event0.
    int(*invoke)(void*,std::uintptr_t owner,const float direction[3]) = nullptr;
};
OriginalCommandResult original_controller_heading(
    const dh2::character::ControllerCommandState32&,const float direction[3],
    const OriginalHeadingBodyServices&,std::string& error);
// Exact source attack gate/network/controllable flow. Host backend owns full
// network, speculative target rollback and actual Character attack/FSM dispatch.
// Void backend errors remain its latched responsibility; do not fabricate success.
OriginalCommandResult original_controller_attack(
    dh2::character::ControllerAttackState32&,dh2::character::AttackState64*,
    std::uintptr_t target,const dh2::character::AttackServices16&,std::string& error);
// Original notification ordering is independent of command admission. In
// particular animation end22/23 still invoke end virtual and state event under locks.
bool original_controller_animation_event(const dh2::character::AnimationEventFacts&,
    const dh2::character::AnimationEventServices&,std::string& error);
} // namespace dh::foundation
