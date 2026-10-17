#pragma once

#include "player_locality_v1.hpp"
#include <cstdint>

namespace dh2::player_local_selection_v1 {
using PlayerInfo = player_locality_v1::PlayerInfo;
using Registry = player_locality_v1::Registry;

// A borrow of the actual SavegameManager identity and its last-slot +8 word.
// This descriptor owns no save/profile/player state.
struct SavegameManager {
    std::uintptr_t identity = 0;
    std::int32_t* last_slot_8 = nullptr;
};
enum class Operation : std::uint32_t {
    none, online, game_state_online, acquire_matching, matching_in_room,
    acquire_net_manager, net_initialized, local_ids_6b4, internal_id_player,
    local_controller_66c, character_660, internal_id_670, application,
    savegame_manager_4c, player_manager_40, save_slot_664
};
struct Services {
    // Reuse the same canonical online/Matching/internal-ID/Character services.
    // internal_id_player is the actual GetPlayerByInternalID callee and must
    // preserve its own fresh source queries. GetLocalPlayer always passes 0.
    const player_locality_v1::Services* queries = nullptr;
    void* context = nullptr;
    std::int32_t (*local_controller_66c)(void*, PlayerInfo*, std::uint8_t*) = nullptr;
    // The source returns PlayerInfo+670, not the registry's map key. Keeping
    // this as a mandatory field read preserves their distinct identities.
    std::int32_t (*internal_id_670)(void*, PlayerInfo*, std::int32_t*) = nullptr;
    // Manager+6b4 local-player network vector. It is distinct from the all-ID
    // vector used by GetPlayerByCharacter. Each span is borrowed for one read.
    std::int32_t (*local_ids_6b4)(void*, const Registry*, const std::int32_t**,
                                 std::uint32_t*) = nullptr;
    std::int32_t (*application)(void*, std::uintptr_t*) = nullptr;
    std::int32_t (*savegame_manager_4c)(void*, std::uintptr_t, SavegameManager**) = nullptr;
    std::int32_t (*player_manager_40)(void*, std::uintptr_t, const Registry**) = nullptr;
    std::int32_t (*save_slot_664)(void*, PlayerInfo*, std::int32_t**) = nullptr;
};
enum class Status : std::uint32_t {
    complete, invalid_argument, invalid_registry, service_unavailable,
    service_failed, missing_projection
};
enum class IdRoute : std::uint32_t {none, local_tree, network_vector};
struct Result {
    Status status = Status::complete;
    IdRoute id_route = IdRoute::none;
    Operation last_operation = Operation::none;
    std::uint32_t service_calls = 0, entries_examined = 0;
    std::int32_t internal_id = -1;
    PlayerInfo* player = nullptr;
    std::uintptr_t application = 0, savegame_manager = 0;
    const Registry* manager = nullptr;
    std::uint32_t last_slot_writes = 0, player_slot_writes = 0;
};
// Local ordinal is unsigned-bounds-checked and is not an internal ID. Offline
// selection scans canonical map order, counting only local-controller entries;
// require_character additionally filters null Character+660 associations.
Status get_internal_id_by_local_id(const Registry*, const Services*,
                                   std::int32_t ordinal,
                                   std::uint32_t require_character, Result*);
Status get_local_player(const Registry*, const Services*, std::int32_t ordinal,
                        std::uint32_t require_character, Result*);
// Original native helper accepts its ordinary signed arguments unchanged.
// The SWF wrapper's nonnegative argument check is a separate caller boundary.
// Capture Application once, write actual SavegameManager+8, then freshly read
// that captured Application's manager+40, GetLocalPlayer(ordinal,false), and
// store the actual selected PlayerInfo+664. Reached failures retain each store.
// No Character registration or profile loading is inferred by this operation.
Status assign_save_slot_to_player(const Services*, std::int32_t slot,
                                  std::int32_t ordinal, Result*);
} // namespace dh2::player_local_selection_v1
