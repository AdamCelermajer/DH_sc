#pragma once

// P16 CONTAINERS2 (T4): rewards for an opened or broken authored container.
//
// Original DoOpen (0x3a0a98) runs DropLootTable(row.loot, opener, -1, false) on
// the container, and every selected item lands in the same world-item store the
// death rewards use (DROPS, Preview 15). This owner adds no store, RNG or
// inventory: it composes the existing source kernel (SourceContainerLootV1), the
// session loot RNG loan and RuntimeWorldItemAdapterV1. Automatic pickup
// (PickUpType 0) and the rest of the DROPS presentation are the existing owners.

#include "../../actor_definitions.hpp"
#include "../../world.hpp"
#include "../../features/interactions/source_container_loot_v1.hpp"
#include "../../features/loot/runtime_world_item_adapter_v1.hpp"
#include "../../../game-data/loot_entry_selection_v8.hpp"
#include "../../../game-data/loot_power_resources_v7.hpp"
#include "../../../game-data/loot_tables_v2.hpp"

#include <cstdint>
#include <string>

namespace dh::foundation::containers {

struct ContainerLootInputsV1 {
    dh2::data::LootTablesV2::Borrow tables;
    dh2::data::LootPowerResourcesV7::Borrow powers;
    dh2::data::LootEntryServicesV8 entry{};
    loot::RuntimeWorldItemAdapterV1* store = nullptr;
    // Session loot RNG loan (PlayableActorWorld::with_loot_random). One loan per open.
    void* rng_context = nullptr;
    bool (*with_rng)(void*, const interactions::SourceContainerLootRngOperationV1&, std::string&) = nullptr;
    // Opener's source value bonus (CharacterProperties resolved[195], fixed point 256).
    // False when the opener has no bonus provider; GoldStack rows are then not priced.
    void* bonus_context = nullptr;
    bool (*opener_bonus256)(void*, ActorId, std::int32_t&, std::string&) = nullptr;
};

struct ContainerLootOutcomeV1 {
    interactions::SourceContainerLootReceiptV1 receipt;
    std::size_t gold_unpriced = 0;   // GoldStack rows selected with no opener bonus (RNG consumed, not published)
};

class ContainerLootV1 {
public:
    // Validates the borrowed source owners. Resets the receipt ledger.
    bool bind(const ContainerLootInputsV1& inputs, std::string& error);
    bool bound() const noexcept { return bound_; }

    // DoOpen for one declaration: rolls the authored loot row once per
    // (declaration, lifecycle) and publishes every selected item at the object.
    // A second call for the same lifecycle fails and publishes nothing.
    bool open(const ActorDefinition& definition, const WorldObject& object, ActorId opener,
              std::uint64_t lifecycle, std::int32_t table, ContainerLootOutcomeV1& outcome,
              std::string& error);

private:
    static bool with_rng(void* context, const interactions::SourceContainerLootRngOperationV1& operation,
                         std::string& error);
    static bool drop_item(void* context, const interactions::SourceContainerDropItemV1& item,
                          dh2::data::LootRandom8V2& random, std::string& error);

    interactions::SourceContainerLootV1 loot_;
    ContainerLootInputsV1 inputs_;
    std::size_t gold_unpriced_ = 0;
    bool bound_ = false;
};

} // namespace dh::foundation::containers
