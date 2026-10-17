#pragma once

#include "character_animation_ai.hpp"
#include "character_state.hpp"

#include <cstdint>
#include <string>

namespace dh2::character_enemy_attack_event_v1 {

enum class Status : std::uint8_t { complete, invalid_argument, provider_failed };

// Joins CharAI's attack-step event to the same Character state owner. All
// non-0x1a animation-AI services remain borrowed from the caller.
struct Binding {
    std::uintptr_t character_identity{};
    character::AnimationAIState96* ai{};
    character::AnimationAIServices16 animation_services{};
    character::State* state{};
    character::Facts* facts{};
    character::Services state_services{};
};

struct Result {
    std::uint32_t animation_service_calls{};
    std::uint32_t forwarded_calls{};
    std::uint32_t character_event_calls{};
    std::int32_t character_event_status{-1};
};

// Executes only the source Attack-begin consumer. Event 0x1a is delivered to
// Character's existing state-event owner; it is not treated as a damage hit.
Status attack_begin(const Binding&, Result*, std::string& error) noexcept;

} // namespace dh2::character_enemy_attack_event_v1
