#pragma once

#include "current_level_quest_event_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::character_interact_v1 {

struct Request {
    std::uintptr_t target_character{};
    std::uintptr_t interactor{};
    std::int32_t talk_to_npc_event_type{-1};
    std::int32_t target_property_id{-1};
    std::int32_t interaction_animation{-1};
    std::uint8_t target_is_interacting{};
    std::uint8_t target_flag_2fa{};
};

struct Services {
    void* context{};
    bool (*raise_talk_to_npc)(void*,
        level_world::current_level_quest_event_v1::Event&, std::string&){};
    bool (*set_interact_state)(void*, std::uintptr_t target_character,
        std::int32_t animation, std::uint8_t suppressed,
        std::uintptr_t interactor, std::uint8_t direct_set,
        std::string&){};
    bool (*raise_character_event)(void*, std::uintptr_t target_character,
        std::uint32_t event, std::uintptr_t interactor, std::string&){};
    // Character::Interact reads target +0x2fa after TalkToNPC and
    // SM_SetInteractState have both run; those callbacks may update the byte.
    bool (*read_target_flag_2fa)(void*, std::uintptr_t target_character,
        std::uint8_t& value, std::string&){};
};

struct Result {
    bool talk_event_raised{};
    bool state_set{};
    bool character_event_raised{};
};

enum class Status : std::uint8_t {
    complete, invalid_argument, provider_unavailable, provider_failed
};

// Character::Interact at 0x3a4d78 is invoked on the NPC (`target_character`)
// with the Player GameObject as `interactor`. It raises TalkToNPC only when
// the NPC is not already interacting, then always calls
// SM_SetInteractState(3, true, interactor, false), followed by
// Character::RaiseEvent(5, interactor) only when the target's +0x2fa
// TalkToNPC marker is clear.
Status interact(const Request&, const Services&, Result&, std::string&);

} // namespace dh2::character_interact_v1
