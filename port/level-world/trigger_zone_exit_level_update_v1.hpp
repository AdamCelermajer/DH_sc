#pragma once

#include "object_manager_runtime_owner_v1.hpp"
#include "source_level_owner_v1.hpp"
#include "../game-data/level_tables.hpp"

#include <cstdint>
#include <string>

namespace dh2::trigger_zone_exit_level_update_v1 {

enum class Status : std::uint8_t {
    complete, skipped_level_transition_gate, invalid_state,
    provider_unavailable, provider_failed
};

struct Services {
    void* context{};
    bool (*set_fasttravel_unlocked)(void*, std::uintptr_t character,
                                    const std::string& fasttravel_id,
                                    bool unlocked, std::int32_t source_index,
                                    std::string& error){};
    bool (*is_player_touching)(void*, std::uintptr_t trigger,
                               std::uintptr_t character, bool& touching,
                               std::string& error){};
    bool (*is_local_player_hosting)(void*, bool& hosting,
                                    std::string& error){};
    bool (*meet_condition)(void*, std::uintptr_t trigger, bool& met,
                          std::string& error){};
    bool (*require_online_update)(void*, std::uintptr_t trigger,
                                  std::string& error){};
    bool (*resolve_localized_level_name)(void*, std::int32_t level_row,
                                         std::string& result,
                                         std::string& error){};
    bool (*display_fast_travel)(void*, bool show,
                                const std::string& localized_name,
                                const std::string& level_name,
                                std::int32_t entry_point,
                                std::string& error){};
};

struct Request {
    std::uintptr_t level_identity{};
    std::uint32_t level_generation{};
    std::uintptr_t trigger_identity{};
    std::int32_t trigger_source_handle{};
    std::uintptr_t local_character{};
};

// One portable TriggerZoneExitLevel::Update step (libDungeonHunter2.so
// 0x39c33c). It resolves the trigger through the retained Level's canonical
// ObjectManager and mutates only that row's source +2072/+2073 state.
// Character, online, localization and HUD effects borrow existing owners.
Status update(source_level_owner_v1::Owner&, const data::LevelTables&,
              const Request&, const Services&, std::string& error);

// Source TriggerZoneExitLevel inherits GameObject::Enabled/Disabled and
// TriggerZone::setUpdating. Apply their same-row mutable bytes after the
// canonical ObjectManager condition evaluator changes +0x8a.
Status set_enabled(object_manager_runtime_owner_v1::Owner&,
                   std::uintptr_t identity,bool enabled,
                   std::string& error);

} // namespace dh2::trigger_zone_exit_level_update_v1
