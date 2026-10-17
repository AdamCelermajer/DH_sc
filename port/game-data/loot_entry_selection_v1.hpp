#pragma once

#include "loot_tables_v2.hpp"

namespace dh2::data {

struct LootPlayerClassCountsV1 {
    std::int32_t mage{};
    std::int32_t rogue{};
    std::int32_t warrior{};
};

// Source caller facts supplied by the existing PlayerManager and
// DebugSwitches owners. The loot selector creates neither owner nor defaults.
struct LootEntrySelectionContextV1 {
    LootPlayerClassCountsV1 player_counts{};
    bool infinite_loot_drops{};
};

// The source reads this switch in DebugSwitches and class counts from the
// existing PlayerManager. The caller supplies those facts and the existing
// inventory-owned RNG service; this module creates no parallel owners.
bool loot_entry_uses_percent_v1(const LootEntry32V2&, bool infinite_drops) noexcept;
std::int32_t loot_entry_effective_probability_v1(
    const LootEntry32V2&, const LootPlayerClassCountsV1&,
    bool infinite_drops) noexcept;

// Returns false for a required RNG provider failure. `accepted` is committed
// only on success. Percentage checks preserve the source's inclusive <= roll.
bool loot_entry_do_percent_roll_v1(
    const LootEntry32V2&, bool infinite_drops,
    const InventoryRandomServiceV4&,
    bool& accepted, std::string& error) noexcept;

// Selects the source index from a contiguous projection of raw 32-byte
// LootEntry payloads. Empty/all-percentage/zero-total input returns index 0
// without a draw, matching both original _GetRandomLootEntry overloads.
bool loot_entries_choose_weighted_v1(
    const LootEntry32V2*, std::uint32_t count,
    const LootPlayerClassCountsV1&, bool infinite_drops,
    const InventoryRandomServiceV4&,
    std::uint32_t& selected_index, std::string& error) noexcept;

}

extern "C" {
// Frozen fixture adapters only. They borrow the existing LootRandom8V2 state
// through dh2_loot_v2_random; production callers use the Inventory RNG API.
int dh2_loot_entry_is_percent_v1(std::uint32_t*,
    const dh2::data::LootEntry32V2*, std::uint32_t infinite_drops) noexcept;
int dh2_loot_entry_effective_probability_v1(std::int32_t*,
    const dh2::data::LootEntry32V2*, const dh2::data::LootPlayerClassCountsV1*,
    std::uint32_t infinite_drops) noexcept;
int dh2_loot_entry_do_percent_roll_v1(std::uint32_t*,
    const dh2::data::LootEntry32V2*, std::uint32_t infinite_drops,
    dh2::data::LootRandom8V2*) noexcept;
int dh2_loot_entries_choose_weighted_v1(std::uint32_t*,
    const dh2::data::LootEntry32V2*, std::uint32_t count,
    const dh2::data::LootPlayerClassCountsV1*, std::uint32_t infinite_drops,
    dh2::data::LootRandom8V2*) noexcept;
}
