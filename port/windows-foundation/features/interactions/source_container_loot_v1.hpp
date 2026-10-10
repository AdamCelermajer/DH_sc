#pragma once

#include "../../actor_state.hpp"
#include "../../actor_definitions.hpp"
#include "../../../game-data/loot_item_selection_v8.hpp"
#include "../../../game-data/loot_power_resources_v7.hpp"
#include <functional>
#include <map>
#include <memory>

namespace dh::foundation::interactions {

struct SourceContainerDropItemV1 {
    ActorId source_actor{invalid_actor_id};
    ActorId opener_actor{invalid_actor_id};
    std::int32_t loot_table{-1};
    std::int32_t fixed_powers{-1};
    bool source_flag{};
    const ActorDefinition* source_definition{};
    const ActorState* source_state{};
    const WorldObject* source_object{};
    // Exact source position for object-backed drops. This is copied from the
    // existing same-world WorldObject transform; no character sheet/vitals are
    // introduced to adapt neutral containers to death-drop sinks.
    std::array<float,3> source_position{};
    dh2::data::LootItemInfoV8 selected{};
};

using SourceContainerLootRngOperationV1 =
    std::function<bool(dh2::data::LootRandom8V2&,std::string&)>;

enum class SourceContainerLootStateV1 { none, attempted, completed, failed };

struct SourceContainerLootReceiptV1 {
    ActorId source_actor{invalid_actor_id};
    std::uint64_t binding_lifecycle{};
    SourceContainerLootStateV1 state{SourceContainerLootStateV1::none};
    std::size_t selected_items{};
    std::size_t delivered_items{};
};

struct SourceContainerLootServicesV1 {
    // Optional lease for stateful callback context used by a retained
    // SessionOpenable/Destructible owner.
    std::shared_ptr<void> owner;
    dh2::data::LootTablesV2::Borrow tables;
    dh2::data::LootPowerResourcesV7::Borrow powers;
    dh2::data::LootEntryServicesV8 entry{};
    // This must be the same application/session Loot RNG used by source loot.
    dh2::data::LootRandom8V2* gameplay_rng{};
    void* context{};
    // Alternative to a retained RNG pointer: the current session may lend its
    // actual persisted source stream for the complete select-and-publish pass.
    // The operation is synchronous; neither the callback nor its RNG reference
    // may escape the loan.
    bool (*with_gameplay_rng)(void*,const SourceContainerLootRngOperationV1&,
                              std::string&){};
    // Synchronous same-world drop sink. `selected` and its item/entry pointers
    // borrow from `tables` and must be consumed before this callback returns.
    bool (*drop_item)(void*, const SourceContainerDropItemV1&, std::string&){};
    // RNG-aware sink for source values such as type13 GoldStack. This receives
    // the same stream loan used by table and item selection.
    bool (*drop_item_with_rng)(void*,const SourceContainerDropItemV1&,
                               dh2::data::LootRandom8V2&,std::string&){};
};

// Table/entry selection is the original pure source kernel; the host supplies
// the existing session RNG and the same world-item publication sink used by its
// actor lane. This owner creates no inventory, item pool, RNG, or object manager.
class SourceContainerLootV1 {
public:
    bool drop(ActorId source_actor, std::uint64_t binding_lifecycle,
              const ActorDefinition&, const ActorState&, ActorId opener_actor,
              std::int32_t table, std::int32_t fixed_powers, bool source_flag,
              const SourceContainerLootServicesV1&,
              SourceContainerLootReceiptV1&, std::string&);
    bool drop(ActorId source_actor, std::uint64_t binding_lifecycle,
              const ActorDefinition&, const WorldObject&, ActorId opener_actor,
              std::int32_t table, std::int32_t fixed_powers, bool source_flag,
              const SourceContainerLootServicesV1&,
              SourceContainerLootReceiptV1&, std::string&);
    void clear() noexcept { receipts_.clear(); }
private:
    bool drop_impl(ActorId,std::uint64_t,const ActorDefinition&,
                   const ActorState*,const WorldObject*,ActorId,
                   std::int32_t,std::int32_t,bool,
                   const SourceContainerLootServicesV1&,
                   SourceContainerLootReceiptV1&,std::string&);
    struct Key {
        ActorId actor{};
        std::uint64_t lifecycle{};
        bool operator<(const Key& other) const noexcept {
            return actor < other.actor || (actor == other.actor && lifecycle < other.lifecycle);
        }
    };
    std::map<Key, SourceContainerLootReceiptV1> receipts_;
};

} // namespace dh::foundation::interactions
