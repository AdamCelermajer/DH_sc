#pragma once

#include "player_local_selection_v1.hpp"
#include <cstdint>

namespace dh2::player_offline_registration_v1 {
using PlayerInfo = player_manager_host_level::PlayerInfoProjection;
using Registry = player_manager_host_level::PlayerRegistry;

// Views of caller-owned source words. The registry key is NOT internal_id_670.
// The lifecycle owner supplies the stable projection and these same words;
// this module creates no PlayerInfo, map, Save, Character or input owner.
struct SourceFields {
    PlayerInfo* player = nullptr;
    std::int32_t *save_slot_664 = nullptr, *controller_668 = nullptr;
    std::uint8_t* local_66c = nullptr;
    std::int32_t *internal_id_670 = nullptr, *member_674 = nullptr;
    std::int32_t *number_678 = nullptr, *group_number_67c = nullptr;
    // Required only by the reached joining/network controller branches.
    std::uint8_t* joining_state_240 = nullptr;
    const std::int32_t *network_id_178 = nullptr, *network_member_1a0 = nullptr;
};
struct InputDevice {
    std::uintptr_t identity = 0;
    const std::uint8_t *connected_758 = nullptr, *pressed_1e8 = nullptr;
    const float *axis_1d8 = nullptr, *minimum_1e0 = nullptr, *maximum_1e4 = nullptr;
};
enum class Operation : std::uint32_t {
    none, online, game_state_online, acquire_matching, matching_in_room,
    acquire_net_manager, net_initialized, internal_id_player, fields,
    construct_temporary, map_subscript, assign, destroy_temporary,
    is_active, set_enabled, net_ids, input_manager, gamepad_count, gamepad,
    net_local_player, local_player, application, savegame_manager,
    joining_controller, current_level, hud_root, numeric_value, invoke_as,
    drop_as_value, set_state
};
struct Services {
    void* context = nullptr;
    const player_locality_v1::Services* queries = nullptr;
    // Needed only for the network last-slot handoff's exact captured owners.
    const player_local_selection_v1::Services* selection = nullptr;
    std::int32_t (*fields)(void*, PlayerInfo*, SourceFields*) = nullptr;
    // Mandatory source lifecycle deliveries. Temporary storage remains owned
    // by its provider until the reached destroy call; failures add no cleanup.
    std::int32_t (*construct_temporary)(void*, PlayerInfo**) = nullptr;
    std::int32_t (*map_subscript)(void*, Registry*, std::int32_t key, PlayerInfo**) = nullptr;
    std::int32_t (*assign)(void*, PlayerInfo* destination, const PlayerInfo* source) = nullptr;
    std::int32_t (*destroy_temporary)(void*, PlayerInfo*) = nullptr;
    std::int32_t (*is_active)(void*, PlayerInfo*, std::int32_t*) = nullptr;
    std::int32_t (*set_enabled)(void*, PlayerInfo*, std::uint32_t) = nullptr;
    std::int32_t (*input_manager)(void*, std::uintptr_t*) = nullptr;
    std::int32_t (*gamepad_count)(void*, std::uintptr_t, std::int32_t*) = nullptr;
    std::int32_t (*gamepad)(void*, std::uintptr_t, std::uint32_t, InputDevice*) = nullptr;
    std::int32_t (*net_local_player)(void*, std::uintptr_t net_manager,
                                    std::uint32_t ordinal, PlayerInfo**) = nullptr;
    std::int32_t (*joining_controller)(void*, Registry*, std::uint32_t,
                                       std::int32_t internal_id) = nullptr;
    std::int32_t (*current_level)(void*, std::uintptr_t*) = nullptr;
    std::int32_t (*hud_root)(void*, std::uintptr_t*) = nullptr;
    // Platform AS value representation is borrowed. Make a numeric value
    // from the captured +67c word, invoke the exact names, then drop that same
    // value before SetState(2). Missing/nonempty services cannot be no-ops.
    std::int32_t (*numeric_value)(void*, std::int32_t, std::uintptr_t*) = nullptr;
    std::int32_t (*invoke_as)(void*, std::uintptr_t hud, const char* movie,
                            const char* function, std::uintptr_t value) = nullptr;
    std::int32_t (*drop_as_value)(void*, std::uintptr_t) = nullptr;
    std::int32_t (*set_state)(void*, PlayerInfo*, std::uint8_t) = nullptr;
};
enum class Status : std::uint32_t {
    complete, invalid_argument, invalid_registry, service_unavailable,
    service_failed, missing_projection, reentrant
};
struct Result {
    Status status = Status::complete;
    Operation last_operation = Operation::none;
    std::uint32_t service_calls = 0, entries_examined = 0, source_writes = 0;
    std::uint32_t added_players = 0, duplicate_adds = 0, renumber_calls = 0;
    std::uint32_t controllers_examined = 0, joining_calls = 0, notifications = 0;
    PlayerInfo *temporary = nullptr, *destination = nullptr;
};
Status is_player_in_local_map(const Registry*, std::int32_t key, bool* found,
                             Result*);

// Reconstruct the source callers over the sole canonical registry. Providers
// must retain their borrowed storage during synchronous calls. Reached
// failures preserve stores/lifecycle effects; no retry, rollback, synthetic
// destructor, preview association or fallback-slot inheritance is introduced.
class Runtime {
    Registry& registry_;
    Services services_;
    bool active_ = false;
public:
    Runtime(Registry& registry, Services services) : registry_(registry), services_(services) {}
    Runtime(const Runtime&) = delete;
    Runtime& operator=(const Runtime&) = delete;
    Status add_player(std::int32_t internal_id, std::int32_t member_id,
                      std::int32_t controller, std::uint32_t local, Result*);
    Status update_player_numbers(Result*);
    Status check_local_controllers(Result*);
};
} // namespace dh2::player_offline_registration_v1
