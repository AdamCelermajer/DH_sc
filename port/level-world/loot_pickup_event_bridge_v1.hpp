#pragma once

#include "loot_pickup_quest_tail_v10.hpp"
#include "source_level_owner_v1.hpp"
#include "../game-data/fresh_inventory_owned_v4.hpp"

namespace dh2::character::loot_pickup_event_bridge_v1 {

struct Services {
    void* context{};
    bool (*is_player)(void*, std::uintptr_t character, bool& result,
                      std::string& error){};
    std::uintptr_t event_owner_identity{};
    std::int32_t gather_event_type{};
    bool (*raise_async)(void*, std::uintptr_t event_owner_identity,
                        const LootPickupQuestEventV10&, std::string&){};
};

enum class Status : std::uint8_t { raised, ignored, invalid, failed };

struct Result {
    Status status{Status::invalid};
    std::uintptr_t level_identity{};
    std::uintptr_t event_owner_identity{};
    bool registered_item{};
};

// Source TransferInventoryTo's post-commit GatherLoot tail, bound to the
// current source-level lifecycle and the same Character's sole V4 inventory.
// `event_owner_identity` must be the existing Quest owner whose EventManager
// table is registered for this active Level; the callback is its synchronous
// RaiseAsync thunk. No Level, Quest, EventManager, or inventory is created.
Status after_transfer(const source_level_owner_v1::Owner& level,
                     const data::FreshInventoryOwnedV4& inventory,
                     std::uintptr_t character, std::int32_t item_id,
                     const Services&, Result*, std::string& error);

} // namespace dh2::character::loot_pickup_event_bridge_v1
