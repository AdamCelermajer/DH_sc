#pragma once

#include "character_distribute_xp_v1.hpp"
#include "native_player_character_owner_v1.hpp"
#include "../player-info-level/player_manager_friendly_v1.hpp"
#include "object_manager_runtime_owner_v1.hpp"

namespace dh2::character_distribute_xp_player_owners_v1 {

struct Binding {
    const player_manager_friendly_v1::Registry* registry{};
    const player_manager_friendly_v1::Services* friendly{};
    const player_locality_v1::Services* locality{};
    const character::NativePlayerCharacterOwnerV1* character_owner{};
    const object_manager_runtime_owner_v1::Owner* object_manager{};
    const data::PropertyRules* property_rules{};
};

// Owner-only projection for one source friendly-player ordinal. Position is
// read from the same identity's canonical GameObject in ObjectManager.
struct Result {
    player_manager_friendly_v1::PlayerInfo* player{};
    std::uintptr_t character_identity{};
    data::PropertyView properties{};
    data::PlayerSavegameV1* savegame{};
    std::uint32_t is_local{};
    float world_x{};
    float world_y{};
};

enum class Status : std::uint32_t {
    complete, invalid_argument, player_selection_failed, missing_character,
    character_owner_mismatch, invalid_properties, local_query_failed,
    game_object_missing, invalid_position
};

// Joins existing PlayerManager, Character, Save/property and CNet local-player
// owners. It creates no Character, Save, property, roster or locality state.
Status resolve(const Binding*, std::int32_t friendly_ordinal, Result*,
               std::string& error);

} // namespace dh2::character_distribute_xp_player_owners_v1
