#pragma once

#include "fresh_inventory_owned_v4.hpp"

#include <cstdint>
#include <string>

namespace dh2::data::player_add_loot_v1 {

// One source ItemInventory::AddLoot invocation over the existing Character's
// V4 inventory. This adapter only projects arguments and retained owners; it
// creates no inventory, PropertyState, RNG, text, presentation, or Debug owner.
struct Request {
    std::uintptr_t character{};
    // Original AddLoot arguments: LootTable, value bonus, power bonus,
    // requested power count, and source boolean. The initial-equipment caller
    // at Character::_InitEquipment supplies (resolved[9], 0, 0, -1, false).
    std::int32_t arguments[5]{};
};

struct Bindings {
    std::uintptr_t character{};
    FreshInventoryOwnedV4* inventory{};
    // Must point to inventory.properties()->resolved[9], the same live
    // Character property consumed by _InitEquipment.
    const std::int32_t* resolved_loot_property{};
    const LootEntrySelectionContextV1* selection{};
    std::int32_t difficulty{};
    RetainedItemSlotV4 pending{};
    const OwnedInventoryServicesV4* inventory_services{};
    LootPowerCreationV7* creation{};
    ItemPowerTablesV5::Borrow powers;
    ItemTextServicesV5 text;
};

struct Result {
    std::int32_t loot_table{-1};
    std::uint32_t items_before{};
    std::uint32_t items_after{};
    bool attempted{};
    bool pending_item{};
};

enum class Status { complete, invalid_argument, failed };

// Executes the source AddLoot-table branch using the same V4 owner and shared
// V7 RNG/power/text dependencies. Class-count and Debug facts are borrowed
// from the caller's live owners. Missing reached services remain failures.
Status invoke(const Bindings&, const Request&, Result*, std::string& error);

} // namespace dh2::data::player_add_loot_v1
