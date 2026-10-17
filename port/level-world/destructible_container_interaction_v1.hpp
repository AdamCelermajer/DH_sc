#pragma once

#include "current_level_quest_event_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::level_world::destructible_container_interaction_v1 {

namespace events = current_level_quest_event_v1;

struct State {
    std::uintptr_t identity{};
    std::uintptr_t attacker{};
    std::int32_t network_id{-1};
    std::int32_t data_id{-1};
    std::int32_t initial_hits{};
    std::int32_t remaining_hits{};
    std::int32_t container_state{};
    std::int32_t interact_sound{-1};
    bool keep_physics{};
    bool visual_present{};
};

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    source_callback_failed,
    current_level_missing,
    event_raise_failed,
};

struct Services {
    void* context{};
    bool (*current_level)(void*, bool& present, std::string& error){};
    bool (*destroy_game_object_constant)(void*, std::int32_t& value,
                                         std::string& error){};
    bool (*raise_current_level_event)(void*, events::Event&,
                                      std::string& error){};
    // Called only on the nonterminal hit path and only when the source has a
    // retained VisualObject. `frame` is the source +0x444 - new +0x445 value.
    bool (*set_hit_animation_frame)(void*, std::int32_t frame,
                                    std::string& error){};
    bool (*play_hit_sound)(void*, std::int32_t sound,
                           std::string& error){};
    bool (*set_opener)(void*, std::uintptr_t attacker,
                       std::string& error){};
    bool (*set_container_state)(void*, std::int32_t state,
                                std::string& error){};
    bool (*remove_physical_object)(void*, std::string& error){};
    bool (*play_activate_animation)(void*, std::string& error){};
    // Container::DoOpen: DropLootTable first, then optional ExternalScript
    // OnOpen. The adapter must use the current shared item/loot owners.
    bool (*do_open)(void*, std::string& error){};
    bool (*play_interact_sound)(void*, std::int32_t sound,
                                std::string& error){};
};

// DestructibleContainer::__EventCallback("opened") raises its own
// DestroyGameObject event with a null Character, then calls the base
// Container callback (which reaches DoOpen). This is separate from the
// attacker-bearing event raised by Interact.
struct OpenedEventServices {
    void* context{};
    bool (*current_level)(void*, bool& present, std::string& error){};
    bool (*destroy_game_object_constant)(void*, std::int32_t& value,
                                         std::string& error){};
    bool (*raise_current_level_event)(void*, events::Event&,
                                      std::string& error){};
};

struct Result {
    Status status{Status::invalid_argument};
    bool hit_counter_decremented{};
    bool destroy_event_raised{};
    bool already_open{};
    bool opened{};
};

// DestructibleContainer::Interact (IDA 0x3a0da0) and its reached
// Container::Interact path (IDA 0x3a0b38). The event is raised before the
// base Container state guard, matching source ordering.
Status interact(State&, const Services&, Result&,
                std::string& error) noexcept;

Status raise_opened_event(std::int32_t network_id, std::int32_t data_id,
                          const OpenedEventServices&,
                          std::string& error) noexcept;

const char* status_name(Status) noexcept;

} // namespace dh2::level_world::destructible_container_interaction_v1
