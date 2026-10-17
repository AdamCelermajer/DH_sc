#pragma once

#include "character_animation_ai.hpp"
#include "character_animation_events.hpp"
#include "character_controller_commands.hpp"
#include "character_enemy_attack_event_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::character_enemy_pursuit_v1 {

enum class Status : std::uint8_t { complete, invalid_argument, provider_failed };

// Borrows the single canonical AI/controller state and their selected services.
// The adapter only connects the recovered Move-begin decision to the existing
// controller -> Character::Ctrl_MoveTo -> PathTo service chain.
struct Binding {
    character::AnimationAIState96* ai{};
    character::AnimationAIServices16 ai_services{};
    character::ControllerCommandState32* controller{};
    character::CharacterControlServices16 control_services{};
};

struct Result {
    std::uint32_t ai_service_calls{};
    std::uint32_t move_to_calls{};
    std::uint32_t controller_status{};
    bool path_dispatch_attempted{};
};

Status move_begin(const Binding&, Result*, std::string& error) noexcept;

// Source Character animation-event route, with only its Move-begin consumer
// owned here. State lookup and event delivery remain borrowed from the same
// Character event owner. This keeps movement gates/order in the source router
// and sends its accepted Move-begin through the existing PathTo adapter.
struct EventBinding {
    const character::AnimationEventFacts* facts{};
    Binding pursuit{};
    character::AnimationEventServices character_events{};
    const character_enemy_attack_event_v1::Binding* attack_begin{};
};

struct EventResult {
    std::uint32_t route_calls{};
    std::uint32_t move_begin_calls{};
    std::uint32_t forwarded_calls{};
    std::uint32_t state_event_calls{};
    Result pursuit{};
    std::uint32_t attack_begin_calls{};
    character_enemy_attack_event_v1::Result attack_begin{};
};

Status animation_event(const EventBinding&, EventResult*, std::string& error) noexcept;

} // namespace dh2::character_enemy_pursuit_v1
