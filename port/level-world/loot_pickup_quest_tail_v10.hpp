#pragma once

#include <cstdint>
#include <string>

namespace dh2::character {

// Semantic value passed to the source current-level EventManager after a
// successful item transfer. Defaults match QE_GatherLoot's unset identifiers.
struct LootPickupQuestEventV10 {
    std::int32_t objective_type{};
    std::uintptr_t character{};
    std::int32_t network_id{-1};
    std::uint8_t flag0{};
    std::uint8_t flag1{};
    std::int32_t subject_id{-1};
    std::int32_t item_id{};
    // Native 32-bit IEvent payloads can carry an ObjectBase* in the word at
    // +0x18 (QuestMoveInZone). Keep that identity pointer-width on the host;
    // do not overload the independent network_id/item_id semantics.
    std::uintptr_t object_identity{};
};

struct LootPickupQuestServicesV10 {
    void* context{};
    bool (*is_player)(void*, std::uintptr_t character, bool& result,
                      std::string& error){};
    // This is the registered gathering-ID list at ItemInventory+0x30, not
    // the current inventory count or the number of copies of the item.
    bool (*registered_gathering_id)(void*, std::uintptr_t character,
                                    std::int32_t item_id, bool& result,
                                    std::string& error){};
    bool (*current_level_event_manager)(void*, std::uintptr_t& event_manager,
                                        std::string& error){};
    // Preserve the two literal source arguments in their original order.
    bool (*constant)(void*, const char* group, const char* key,
                     std::int32_t& value, std::string& error){};
    // Source call is EventManager::RaiseAsync(const IEvent&), which is a thunk
    // to synchronous Raise in this ELF (IDA 0x339090 -> 0x338ebc). The same
    // event may be mutated by receivers before this callback returns.
    bool (*raise_async)(void*, std::uintptr_t event_manager,
                        const LootPickupQuestEventV10& event,
                        std::string& error){};
};

// Adapted from AdamCelermajer/DH_sc@11fa5242's
// port/level-world/loot_pickup_quest_tail_v10.{hpp,cpp}. The source path is
// TransferInventoryTo's reached quest tail: player check, registered-list
// membership, Application::GetCurrentLevel(), GatherLoot constant, then
// EventManager::RaiseAsync(QE_GatherLoot).
bool loot_pickup_quest_tail_v10(std::uintptr_t actual_inventory_character,
                                std::int32_t actual_item_id,
                                const LootPickupQuestServicesV10& services,
                                std::string& error);

}  // namespace dh2::character
