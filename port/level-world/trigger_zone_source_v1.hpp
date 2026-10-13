#pragma once

#include "object_manager_runtime_owner_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::trigger_zone_source_v1 {

// Port of the source TriggerZone class (libDungeonHunter2.so): the authored
// `script` / `script_move_out` / `script_all_player` / `script_all_player_move_out`
// properties are resolved to ScriptManager ids by TriggerZone::InitPost
// (0x39c0a8), and TriggerZone::Update (0x39b458) starts them when the local
// Player's absolute AABB touches the zone. The Trigger base words live on the
// canonical ObjectManager row (Trigger::Trigger 0x398f44): +0x3a8 triggercount
// (trigger_count_3a8), +0x3b4 activation counter (trigger_max_count_3b4) and
// +0x3b8 delay countdown (trigger_pending_3b8), the same words
// Trigger::Serialize writes. Nothing here owns a second object registry.

struct Definition {
    // Property values (empty strings keep the source defaults).
    std::int32_t trigger_delay_ms{0};                 // +0x3ac `triggerdelay`
    std::string script, script_move_out, script_all_player,
                script_all_player_move_out, effect_one_player, is_door_closed;
};

struct State {
    // TriggerZone::InitPost results (ScriptManager::GetIDFromName(name, false)).
    std::int32_t script_id{-1};                        // +1852
    std::int32_t script_move_out_id{-1};               // +1880
    std::int32_t script_all_player_id{-1};             // +1908
    std::int32_t script_all_player_move_out_id{-1};    // +1936
    // +956: set by InitPost when the triggered script carries a StartDialog
    // with DialogStyles.EnterLocationDialog (ScriptManager entry flag +4).
    std::uint8_t touch_required_956{0};
    std::uint8_t inside_1972{0};                       // TriggerZone::setUpdating clears it
    std::uint8_t all_players_1973{0};
    std::int32_t activating_players_960{0};
    bool resolved{false};
    // Door named by `is_door_closed`, resolved by InitPost (type 2 only).
    std::uintptr_t door_identity{0};
};

enum class Status : std::uint8_t {
    complete, skipped, invalid_state, provider_unavailable, provider_failed
};

struct Services {
    void* context{};
    // Character+0x1480 of the local Player (MarkCharacterAsScripted); false
    // when the local Player has no Character.
    bool (*local_player_scripted)(void*, bool& scripted, std::string& error){};
    // GameObject::IsTouching(local Player's Character) against the zone AABB.
    // `has_character` is false when the local Player has no Character.
    bool (*local_player_touching)(void*, std::uintptr_t trigger, bool& has_character,
                                  bool& touching, std::string& error){};
    // PlayerManager+1732: the number of players (offline: 1) and
    // GameObject::GetNumPlayerTouching over them.
    bool (*player_counts)(void*, std::uintptr_t trigger, std::int32_t& players,
                          std::int32_t& touching, std::string& error){};
    // Application::GetDt, milliseconds for Trigger::UpdateTimer.
    bool (*delta_ms)(void*, std::int32_t& dt, std::string& error){};
    // TriggerZone::SafeStartScriptOnlyOnce: IsScriptRunning(id) then
    // StartScript(id, module id, false) when it is not running.
    bool (*start_script)(void*, std::int32_t script_id, std::int32_t argument,
                         std::string& error){};
    // Door row state (+936) of the linked door.
    bool (*door_state)(void*, std::uintptr_t door, std::int32_t& state,
                       std::string& error){};
};

// One TriggerZone::Update (0x39b458) step on the canonical row.
Status update(object_manager_runtime_owner_v1::GameObject& row,
              const Definition& definition, State& state,
              const Services& services, std::string& error);

// TriggerZone::setUpdating(false) (0x39b26c) clears the inside byte.
inline void set_updating(State& state, bool updating) {
    if (!updating) state.inside_1972 = 0;
}

} // namespace dh2::trigger_zone_source_v1
