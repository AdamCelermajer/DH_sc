#pragma once

#include "character_level_member.hpp"

#include <cstdint>

namespace dh2::player_manager_host_level {

// Small named projection for the PlayerInfo identity selected by the source
// manager. The +0x330 value is always read through the source member leaf at
// `character_level_member`; it is never substituted directly from Character
// properties.
struct PlayerInfoProjection {
    std::uintptr_t identity;
    std::int32_t internal_id;
    character_level_member::IntMember* character_level_member;
};

// The owned native analogue of PlayerManager's internal-ID tree. Source
// entries have unique integer keys; sorted order is an invariant of this
// projection. `manager_plus_8` models GetPlayerByInternalID's exact fallback.
struct PlayerRegistry {
    std::uintptr_t manager_identity;
    PlayerInfoProjection* const* entries;
    std::uint32_t entry_count;
    PlayerInfoProjection* manager_plus_8;
};

struct Services {
    void* context;
    // Each call is a fresh source read. GetHostingPlayer and its nested
    // GetPlayerByInternalID call independently re-check these globals.
    std::int32_t (*read_online_byte_5)(void*, std::uint8_t* raw_value);
    std::int32_t (*read_online_game_state_byte_24)(void*, std::uint8_t* raw_value);
    std::int32_t (*matching_is_host)(void*, std::int32_t* raw_result);
    std::int32_t (*get_net_player_manager)(void*, std::uintptr_t* manager);
    std::int32_t (*net_player_manager_is_initialized)(void*, std::uintptr_t manager,
                                                      std::int32_t* raw_result);
    std::int32_t (*read_net_host_internal_id)(void*, std::uintptr_t manager,
                                               std::int32_t* internal_id);
    std::int32_t (*get_net_player_info)(void*, std::uintptr_t manager,
                                        std::int32_t internal_id,
                                        std::uint32_t lookup_flag,
                                        PlayerInfoProjection** player);
};

enum class Status : std::uint32_t {
    complete = 0,
    invalid_argument = 1,
    invalid_registry = 2,
    service_unavailable = 3,
    service_failed = 4,
    no_player_projection = 5,
    no_level_member = 6,
};

enum class Route : std::uint32_t {
    none = 0,
    local_id_tree = 1,
    manager_plus_8_fallback = 2,
    net_player_info = 3,
};

struct Result {
    Status status;
    Route route;
    std::uint32_t service_calls;
    std::uint32_t local_entries_examined;
    std::int32_t requested_internal_id;
    std::uintptr_t player_identity;
    std::int32_t character_level_330;
};

// Reconstruct PlayerManager::GetHostingPlayer plus its nested
// GetPlayerByInternalID source selection. Services provide the live online /
// network facts; offline or non-network routes search the owned ID registry.
// On a source miss, the selected value is manager+8, not null. A missing
// projection or +0x330 member is reported as a port error instead of being
// dereferenced as the original code would do.
Status get_hosting_level(const PlayerRegistry*, const Services*, Result*);

// Public pointer selection over the same source GetPlayerByInternalID body.
// This does not query the Level member. On success selected receives the
// canonical registry/fallback/network projection; failed delivery leaves it
// unchanged. lookup_flag is forwarded to the network callee unchanged.
Status get_player_by_internal_id(const PlayerRegistry*, const Services*,
                                std::int32_t internal_id,
                                std::uint32_t lookup_flag,
                                PlayerInfoProjection** selected, Result*);

struct ReconcileState {
    PlayerInfoProjection* player;
    std::uintptr_t character_identity;
    std::uintptr_t character_properties_identity;
};

struct ReconcileServices {
    void* context;
    // CharProperties::PROPS_GetInt(property_id=0x13, include_bonus=false).
    // That source method already returns the signed fixed-point word shifted
    // right by eight; this bridge does not shift it again.
    std::int32_t (*get_property_int)(void*, std::uintptr_t character_properties,
                                     std::uint32_t property_id,
                                     std::uint32_t include_bonus,
                                     std::int32_t* result);
    // Source PlayerManager::_ManageCharacters calls PlayerInfo's setter only
    // after a mismatch and obtains its argument from a second fresh GetInt.
    std::int32_t (*set_character_level)(void*, PlayerInfoProjection*,
                                        std::int32_t value);
};

enum class ReconcileStatus : std::uint32_t {
    complete = 0,
    skipped_unbound_character = 1,
    invalid_argument = 2,
    service_unavailable = 3,
    service_failed = 4,
    no_level_member = 5,
};

struct ReconcileResult {
    ReconcileStatus status;
    std::uint32_t property_reads;
    std::uint32_t setter_calls;
    std::int32_t level_before;
    std::int32_t first_property_value;
    std::int32_t setter_argument;
    std::int32_t level_after;
};

// Bounded Level field synchronization slice of PlayerManager::_ManageCharacters.
// The caller must already have proven the source player/Character/CharProperties
// associations and outer loop eligibility; broader manager iteration is not
// reconstructed here.
ReconcileStatus reconcile_character_level(const ReconcileState*,
                                           const ReconcileServices*,
                                           ReconcileResult*);

static_assert(sizeof(PlayerInfoProjection) == 24, "PlayerInfo projection layout");
static_assert(sizeof(PlayerRegistry) == 32, "owned player registry layout");
static_assert(sizeof(Result) == 40, "host-level result layout");
static_assert(sizeof(ReconcileState) == 24, "level reconcile state layout");
static_assert(sizeof(ReconcileResult) == 28, "level reconcile result layout");

}  // namespace dh2::player_manager_host_level
