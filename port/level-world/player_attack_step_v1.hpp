#pragma once

#include "character_animation_ai.hpp"
#include "character_animation_events.hpp"
#include "character_state.hpp"

namespace dh2::character::player_attack_step_v1 {
// Borrowed live owners for one original Animator event 0x26 while the
// canonical Player is in Attack. The caller supplies the same CharAI service
// graph used by Character::RaiseEvent (including nested event 0x1a).
struct Binding {
    State* character_state = nullptr;
    AnimationAIState96* animation_ai = nullptr;
    const AnimationAIServices16* animation_ai_services = nullptr;
    void* event_context = nullptr;
    // The owning Character::RaiseEvent route, including its CharAI dispatch.
    // It is used for the final Animator event 0x26; AnimationAI's nested 0x1a
    // calls the same borrowed owner through animation_ai_services.
    std::int32_t (*raise_character_event)(void*, std::uint32_t, std::uintptr_t) = nullptr;
    std::uint32_t global_blocked = 0;
    std::uint32_t controller_forced = 0;
};
}

// Runs the source Character animation event router for Attack step-begin
// event 0x26. AttackBegin owns its source AI effects; only after it accepts
// does the router forward 0x26 to the same canonical Character state machine.
// Returns 1 on complete delivery and -1 on malformed owners or failed service.
extern "C" int dh2_player_attack_step_begin_v1(
    const dh2::character::player_attack_step_v1::Binding*);
