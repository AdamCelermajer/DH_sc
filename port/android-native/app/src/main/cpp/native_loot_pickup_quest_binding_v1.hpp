#pragma once

#include "native_quest_owner.hpp"
#include "../../../../../../port/level-world/loot_pickup_event_bridge_v1.hpp"

namespace dh2::native::loot_pickup_quest_binding_v1 {

// Borrowed view of the exact production owners already selected for this
// Character and active Level. This helper creates no Level, Quest, EventManager,
// inventory, or event queue; the callback can be called from V4's committed
// ItemObject::Interact tail.
struct Binding {
    const source_level_owner_v1::Owner* level{};
    quests::Owner* quest{};
    data::FreshInventoryOwnedV4* inventory{};
    std::int32_t gather_event_type{};
    void* is_player_context{};
    bool (*is_player)(void*, std::uintptr_t, bool&, std::string&){};

    character::loot_pickup_event_bridge_v1::Status after_transfer(
        std::uintptr_t character, std::int32_t item_id,
        character::loot_pickup_event_bridge_v1::Result*, std::string&) const;
};

} // namespace dh2::native::loot_pickup_quest_binding_v1
