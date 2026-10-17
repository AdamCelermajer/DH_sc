#pragma once

#include "character_combat_queries.hpp"
#include "character_attack_geometry.hpp"
#include "../game-data/combat_events.hpp"
#include "../game-data/fresh_inventory_owned_v4.hpp"

namespace dh2::character::combat_event_v1 {
struct Request {
    const CombatProperties896* properties = nullptr;
    const CombatInventory16* inventory = nullptr;
    const CombatItemRecord164* item_rows = nullptr;
    std::uint32_t item_count = 0;
    data::CombatEventContext event{};
    const char* authored_name = nullptr;
};

struct Result {
    std::int32_t can_range = 0;
    std::int32_t range_min = 0;
    std::int32_t range_max = 0;
    std::int32_t projectile = -1;
    data::CombatEventAction action{};
};

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    range_query_failed,
    route_failed
};

// Derives the capability/projectile from Character::CanRangeAttack(out params)
// before routing the authored event. Non-attack states do not query inventory.
Status route(const Request*, Result*) noexcept;
// Scalar compatibility view used by the canonical V4 bridge and regression.
// IDs address the complete ItemTable span without compression.
Status route_snapshot(const std::int32_t resolved[224], std::int32_t selected_set,
                      const std::int32_t main_hand_ids[2],
                      const bool main_hand_present[2],
                      const data::ItemTable*,
                      const data::CombatEventContext*, const char* authored_name,
                      Result*) noexcept;

// Player-side call-scoped bridge. Uses the canonical PropertyState, V4
// equipment sets and full indexed ItemTable. The temporary scalar view keeps
// original equipment-set indices and Item IDs unchanged; no owner is kept.
Status route_character(const data::PropertyState*,
                       data::FreshInventoryOwnedV4*,
                       const data::CombatEventContext*, const char* authored_name,
                       Result*) noexcept;
}
